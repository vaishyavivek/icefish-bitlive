/* $Id: CustomeChatOutput.h
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

#ifndef CUSTOMCHATOUTPUT_H
#define CUSTOMCHATOUTPUT_H

#include <QObject>
#include <QDateTime>

class CustomChatOutput: public QObject {

    Q_OBJECT

    Q_PROPERTY(QString Text READ Text NOTIFY TextChanged)

    Q_PROPERTY(QString PeerName READ PeerName NOTIFY PeerNameChanged)

    Q_PROPERTY(QString PeerId READ PeerId NOTIFY PeerIdChanged)

    Q_PROPERTY(int IconId READ IconId NOTIFY IconIdChanged)

    Q_PROPERTY(QString TimeGenerated READ TimeGenerated NOTIFY TimeGeneratedChanged)

    Q_PROPERTY(bool Direction READ Direction NOTIFY DirectionChanged)

public:
    CustomChatOutput(QString Text, QString PeerName, QString PeerId, int IconId, bool Direction, QObject* parent = nullptr)
        :QObject(parent), text(Text), peerName(PeerName), peerId(PeerId), iconId(IconId), direction(Direction) {
        timeGenerated = QDateTime::currentDateTime();
    }

    QString Text() const { return text;}

    QString PeerName() const { return peerName;}

    QString PeerId() const { return peerId;}

    int IconId() const { return iconId;}

    QString TimeGenerated() const { return timeGenerated.toString("hh:mm");}

    bool Direction() const { return direction;}


signals:

    void TextChanged();
    void PeerNameChanged();
    void PeerIdChanged();
    void IconIdChanged();
    void TimeGeneratedChanged();
    void DirectionChanged();

private:
    QString text;
    QString peerName;
    QString peerId;
    int iconId;
    QDateTime timeGenerated;
    bool direction;
};

#endif // CUSTOMCHATOUTPUT_H
