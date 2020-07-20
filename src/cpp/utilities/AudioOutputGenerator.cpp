#include "AudioOutputGenerator.h"
//#include <cstring>
//#include <QtEndian>

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

}
