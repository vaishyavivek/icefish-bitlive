#ifndef AUDIOINPUTGENERATOR_H
#define AUDIOINPUTGENERATOR_H

#include <QObject>
#include <QAudioInput>
#include <QJsonValue>
#include <opus/opus.h>

//#include <QThread>
//#include <QAudioOutput>

class AudioInputGenerator : public QObject {

    Q_OBJECT

public:
    explicit AudioInputGenerator(QObject *parent = nullptr);


public slots:

    void start();

    void pause();

private slots:

    void processAudioBuffer();


signals:

    void setAudioBuffer(QJsonValue, int);


private:

    const int SAMPLE_RATE = 48000;
    const int SAMPLE_SIZE = 16;
    const int CHANNEL_COUNT = 1;
    const QAudioFormat::Endian BYTE_ORD = QAudioFormat::LittleEndian;
    const QAudioFormat::SampleType SAMPLE_TYPE = QAudioFormat::SignedInt;
    const QString CODEC = "audio/pcm";
    const int NOTIFY_INTERVAL = 20;


    OpusEncoder *enc;
    int error;
    const int OPUS_INT_SIZE = 2; //sizeof(opus_int16)
    const int FRAME_SIZE = 960;
    const int MAX_FRAME_SIZE = 1276;


    QIODevice *audioBuffer;
    QAudioInput *audioIn;


//    OpusDecoder *dec;
//    QIODevice *audioBuffer2;
//    QAudioOutput *audioOut;
};

#endif // AUDIOINPUTGENERATOR_H


