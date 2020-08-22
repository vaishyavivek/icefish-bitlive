/* $Id: JsonToFrameRunnable.h
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
#ifndef JSONTOFRAMERUNNABLE_H
#define JSONTOFRAMERUNNABLE_H

#include <QObject>
#include <QRunnable>
#include <QVideoFrame>
#include <QJsonValue>
#include <QDebug>

class JsonToFrameRunnable: public QObject, public QRunnable {
    Q_OBJECT

    void run() override {

        auto const encoded = json.toString().toLatin1();
        QImage img;
        img.loadFromData(QByteArray::fromBase64(encoded), "JPG");

        if (!img.isNull()) {

            QVideoFrame frame(img);
            if (frame.isValid()) {

                emit setFrame(frame);

            }
        }
    }

signals:
    void setFrame(const QVideoFrame &frame);

private:
    QJsonValue json;

public:
    void setJsonValue(QJsonValue jsonBuffer) {
        json = jsonBuffer;
    }
};


#endif // JSONTOFRAMERUNNABLE_H
