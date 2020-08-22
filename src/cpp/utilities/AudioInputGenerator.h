/* $Id: AudioInputGenerator.h
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

#ifndef AUDIOINPUTGENERATOR_H
#define AUDIOINPUTGENERATOR_H

#include <QObject>
#include <QAudioInput>
#include <QJsonValue>
#include <opus/opus.h>

#include <QAudioOutput>

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


    OpusDecoder *dec;
    QIODevice *audioBuffer2;
    QAudioOutput *audioOut;
};

#endif // AUDIOINPUTGENERATOR_H


