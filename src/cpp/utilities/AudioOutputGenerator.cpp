/* $Id: AudioOutputGenerator.cpp
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

#include "AudioOutputGenerator.h"

AudioOutputGenerator::AudioOutputGenerator(QObject *parent) : QObject(parent) {

}


void AudioOutputGenerator::start() {

    dec = opus_decoder_create(SAMPLE_RATE,
                              CHANNEL_COUNT,
                              &error);

    QAudioFormat format;
    // Set up the format, eg.
    format.setSampleRate(SAMPLE_RATE);
    format.setChannelCount(CHANNEL_COUNT);
    format.setSampleSize(SAMPLE_SIZE);
    format.setByteOrder(BYTE_ORD);
    format.setSampleType(SAMPLE_TYPE);
    format.setCodec(CODEC);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format))
        format = info.nearestFormat(format);

    audioOut = new QAudioOutput(format);
    audioBuffer = audioOut->start();
    audioBuffer->open(QIODevice::ReadWrite);

    initiliazed = true;
}


void AudioOutputGenerator::setAudioBuffer(QJsonValue json, int size) {

    if (initiliazed) {

        QByteArray encoded = QByteArray::fromBase64(json.toString().toLatin1());
        processAudioBuffer(encoded, size);
    }

}


void AudioOutputGenerator::processAudioBuffer(QByteArray &audio_buffer, int compressedSize) {

    Q_UNUSED(compressedSize)

    int size = int(sizeof (opus_int16)) * CHANNEL_COUNT * FRAME_SIZE;

    QByteArray output = QByteArray(size, char(0));

    opus_int32 decompressedBytes = opus_decode(dec,
                                               reinterpret_cast<const uchar*>(audio_buffer.constData()),
                                               audio_buffer.size(),
                                               reinterpret_cast<opus_int16*>(output.data()),
                                               MAX_FRAME_SIZE,
                                               0);

    if(decompressedBytes < 0) {
        qDebug() << QString("Opus encoder failed: %0").arg(opus_strerror(decompressedBytes));
        return;
    }

    size = int(sizeof (opus_int16)) * CHANNEL_COUNT * decompressedBytes;
    output.resize(size);

    audioBuffer->write(output, size);
    output.clear();
}
