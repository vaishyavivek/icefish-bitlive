/* $Id: AudioOutputGenerator.h
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
