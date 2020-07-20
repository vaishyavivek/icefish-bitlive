#include "AudioInputGenerator.h"

//#include <QtEndian>

//#include <cstring>

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

        }

    }
}
