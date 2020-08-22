/* $Id: AudioInputGenerator.cpp
 * IceFish bitLive aims to solve end to end connectivity issue
 * without any dependency on a centralized server
 * by using a complete P2P driven approach.
 * © IceFish Community, 2020
 *
 * This file is part of IceFish bitLive.
 *
 * IceFish bitLive is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * IceFish bitLive is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with IceFish bitLive.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This project is hosted here,
 * https://github.com/vaishyavivek/icefish-bitlive
*/

#include "AudioInputGenerator.h"

#include <QAudioOutput>

AudioInputGenerator::AudioInputGenerator(QObject *parent) : QObject(parent) {

}


void AudioInputGenerator::start() {

    enc = opus_encoder_create(SAMPLE_RATE,
                              CHANNEL_COUNT,
                              OPUS_APPLICATION_VOIP,
                              &error);


    QAudioFormat format;
    format.setSampleRate(SAMPLE_RATE);
    format.setChannelCount(CHANNEL_COUNT);
    format.setSampleSize(SAMPLE_SIZE);
    format.setByteOrder(BYTE_ORD);
    format.setSampleType(SAMPLE_TYPE);
    format.setCodec(CODEC);


    QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
    if (!info.isFormatSupported(format))
        format = info.nearestFormat(format);

//    qDebug() << format;

    audioIn = new QAudioInput(info, format);
//    audioIn->setBufferSize(50);
    audioIn->setNotifyInterval(NOTIFY_INTERVAL);


    audioBuffer = audioIn->start();
    connect(audioBuffer, &QIODevice::readyRead, this, &AudioInputGenerator::processAudioBuffer);


    dec = opus_decoder_create(SAMPLE_RATE,
                              CHANNEL_COUNT,
                              &error);

//    QAudioFormat format;
    // Set up the format, eg.
    format.setSampleRate(SAMPLE_RATE);
    format.setChannelCount(CHANNEL_COUNT);
    format.setSampleSize(SAMPLE_SIZE);
    format.setByteOrder(BYTE_ORD);
    format.setSampleType(SAMPLE_TYPE);
    format.setCodec(CODEC);

    info = QAudioDeviceInfo::defaultOutputDevice();
    if (!info.isFormatSupported(format))
        format = info.nearestFormat(format);

    audioOut = new QAudioOutput(format);
    audioBuffer2 = audioOut->start();
    audioBuffer2->open(QIODevice::ReadWrite);

}


void AudioInputGenerator::pause() {

    if (audioIn->state() == QAudio::ActiveState) {
        audioIn->stop();
    }
    else {
        audioBuffer = audioIn->start();
    }
}


void AudioInputGenerator::processAudioBuffer() {

    auto len = audioIn->bytesReady();

    if (len > 0) {

        int size = int(sizeof (opus_int16)) * CHANNEL_COUNT * FRAME_SIZE;
        int readSize = len - len % size;

        QByteArray audioInput;
        audioInput.resize(readSize);

        audioBuffer->read(audioInput.data(), readSize);


        while (!audioInput.isEmpty()) {

            QByteArray input = audioInput.mid(0, size);
            audioInput.remove(0, size);

            QByteArray output = QByteArray(MAX_FRAME_SIZE, char(0));
            opus_int32 compressedBytes = opus_encode(enc,
                                                     reinterpret_cast<const opus_int16*>(input.constData()),
                                                     FRAME_SIZE,
                                                     reinterpret_cast<uchar*>(output.data()),
                                                     MAX_FRAME_SIZE);

            if(compressedBytes < 0) {
                qDebug() << QString("Opus encoder failed: %0").arg(opus_strerror(compressedBytes));
                return;
            }

            output.resize(compressedBytes);
            QString str = QString::fromLatin1(output.toBase64().data());

            emit setAudioBuffer(str, compressedBytes);
            output.clear();


            QByteArray encoded = QByteArray::fromBase64(str.toLatin1());
//            processAudioBuffer(encoded, size);

            int size = int(sizeof (opus_int16)) * CHANNEL_COUNT * FRAME_SIZE;

            output = QByteArray(size, char(0));

            opus_int32 decompressedBytes = opus_decode(dec,
                                                       reinterpret_cast<const uchar*>(encoded.constData()),
                                                       encoded.size(),
                                                       reinterpret_cast<opus_int16*>(output.data()),
                                                       MAX_FRAME_SIZE,
                                                       0);

            if(decompressedBytes < 0) {
                qDebug() << QString("Opus encoder failed: %0").arg(opus_strerror(decompressedBytes));
                return;
            }

            size = int(sizeof (opus_int16)) * CHANNEL_COUNT * decompressedBytes;
            output.resize(size);

            audioBuffer2->write(output, size);
            output.clear();

        }

    }
}
