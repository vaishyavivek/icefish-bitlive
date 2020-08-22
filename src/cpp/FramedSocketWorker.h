/* FrameSocketWorker.h
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

#ifndef FRAMEDSOCKETWORKER_H
#define FRAMEDSOCKETWORKER_H

#include <QObject>
#include <QVideoFrame>
#include <QUdpSocket>
#include <QJsonObject>
#include <QThread>

#include "CustomVideoOutput.h"
#include "CustomChatOutput.h"

class FramedSocketWorker : public CustomVideoOutput
{
    Q_OBJECT

    Q_PROPERTY(bool IsActive READ IsActive WRITE setIsActive NOTIFY IsActiveChanged)

public:

    explicit FramedSocketWorker(QString ServerIp, QString PeerId, QString Password, QObject *parent = nullptr);

    bool IsActive() const { return isActive;}
    void setIsActive(bool IsActive) {
        if(isActive != IsActive) {
            isActive = IsActive;
            emit IsActiveChanged();
        }
    }

public slots:

    bool isConnectedToPeer();

    void initiateConnection(QString seedId, QString password);

    void getSeedName(const QString &seedname);

    void sendText(QString data);

    void sendJsonedFrame(QJsonValue frame);

    void sendJsonedAudio(QJsonValue buffer, int size);


private slots:
    void ReadMessage();


signals:

    void IsActiveChanged();

    void connectionFinished();

    void startAudioOutput();

    void setJsonedAudio(QJsonValue, int);

    void setTextReceived(CustomChatOutput *cco);

    void setDebugMessages(const QString &message);


private: //members
    QString peerId;
    int iconId;
    QString password;
    QString seedName;

    QUdpSocket *socket;
    QString serverIp;
    int tries;

    QJsonObject otherPeer;
    bool isConnected = false;
    bool isActive = false;

    QThread voiceReceiver;

    //methods

    void SendReHello();

    void SendPeerConnectRequest();

    void SendPeerConnectAccept(QJsonObject peerData);

    void SendPeerConnectAcceptAgain();

    void SendPeerConnectDummy();

    void SendPeerConnectAckowledge();

    void SendPeerConnectFinish();
};

#endif // FRAMEDSOCKETWORKER_H
