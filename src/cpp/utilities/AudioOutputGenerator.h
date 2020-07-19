#ifndef AUDIOOUTPUTGENERATOR_H
#define AUDIOOUTPUTGENERATOR_H

#include <QObject>
#include <QAudioOutput>
#include <QJsonValue>
#include <opus/opus.h>

class AudioOutputGenerator : public QObject
{
    Q_OBJECT
public:
    explicit AudioOutputGenerator(QObject *parent = nullptr);


public slots:

    void start();

    void setAudioBuffer(QJsonValue, int);


private slots:

    void processAudioBuffer(QByteArray &, int);

private:

    bool initiliazed = false;

    const int SAMPLE_RATE = 48000;
    const int SAMPLE_SIZE = 16;
    const int CHANNEL_COUNT = 1;
    const QAudioFormat::Endian BYTE_ORD = QAudioFormat::LittleEndian;
    const QAudioFormat::SampleType SAMPLE_TYPE = QAudioFormat::SignedInt;
    const QString CODEC = "audio/pcm";


    OpusDecoder *dec;
    int error;
    const int OPUS_INT_SIZE = 2; //sizeof(opus_int16)
    const int FRAME_SIZE = 960;
    const int MAX_FRAME_SIZE = 1276;

    QIODevice *audioBuffer;
    QAudioOutput *audioOut;

};

#endif // AUDIOOUTPUTGENERATOR_H
