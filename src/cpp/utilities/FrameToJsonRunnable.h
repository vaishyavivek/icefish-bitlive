/* $Id: FrameToJsonRunnable.h
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

#ifndef FRAMETOIMAGERUNNABLE_H
#define FRAMETOIMAGERUNNABLE_H

#include <QRunnable>
#include <QJsonValue>
#include <QVideoFrame>
#include <QBuffer>
#include <QImageWriter>
//#include <QTransform>
//#include <QDebug>


class FrameToJsonRunnable: public QObject, public QRunnable {
    Q_OBJECT

    void run() override {

        if (frame.isValid()) {

            QByteArray barray;
            QBuffer qbuffer(&barray);
            qbuffer.open(QIODevice::WriteOnly);

            QImageWriter writer(&qbuffer, "JPG");
            writer.setQuality(qualityBar*10);

            QImage img = frame.image();
            if (!img.isNull()) {

                QTransform rot;
                rot.rotate(-rotation);
                img = img.transformed(rot);

                img.scaled(240, 360);

                writer.write(img);

                QString str = QString::fromLatin1(barray.toBase64().data());
                emit setJsonValue(str);
            }

        }

    }


signals:
    void setJsonValue(QJsonValue value);

private:
    QVideoFrame frame;
    int rotation;
    int qualityBar;

public:
    void setFrame(const QVideoFrame &buffer, int Rotation, int QualityBar) {
        frame = buffer;
        rotation = Rotation;
        qualityBar = QualityBar;
    }

};


#endif // FRAMETOIMAGERUNNABLE_H
