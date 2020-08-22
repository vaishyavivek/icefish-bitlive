/* $Id: FramedSocketWorker.cpp
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

#include "FramedSocketWorker.h"

#include <QDebug>
#include <QJsonDocument>
#include <QTimer>
#include <QThreadPool>
#include <QDebug>
#include <QRandomGenerator>

#include "utilities/AudioOutputGenerator.h"

#include "utilities/JsonToFrameRunnable.h"
//#include "utilities/JsonToAudioBufferRunnable.h"

/*
 * On Sender Side
 * PeerConnectRequest [to Tracker] (0-2) -> PeerConnectResponse [from Tracker]
 * PeerConnectDummy (3-5) [to Peer] -> PeerConnectAccept [from Peer]
 * PeerConnectAckownledge (6-8) [to Peer] -> PeerConnectFinish [from Peer]
 *
 *
 * On Receiver Side
 * PeerConnectInitiate [from Tracker] -> PeerConnectAccept (0-2) [to Peer]
 * PeerConnectAckownledge [from Peer] -> PeerConnectFinish [to Peer]

*/


FramedSocketWorker::FramedSocketWorker(QString ServerIp, QString PeerId, QString Password, QObject *parent)
    : CustomVideoOutput("User" , parent), peerId(PeerId), password(Password), serverIp(ServerIp) {

    socket = new QUdpSocket();
    socket->bind(QHostAddress("0.0.0.0"), 0, QAbstractSocket::ReuseAddressHint);
    connect(socket, &QUdpSocket::readyRead, this, &FramedSocketWorker::ReadMessage);

    iconId = QRandomGenerator::global()->generate() % 6;

    SendReHello();


    AudioOutputGenerator *aog = new AudioOutputGenerator();
    aog->moveToThread(&voiceReceiver);
    connect(&voiceReceiver, &QThread::finished, aog, &QObject::deleteLater);
    connect(this, &FramedSocketWorker::startAudioOutput, aog, &AudioOutputGenerator::start);
    connect(this, &FramedSocketWorker::setJsonedAudio, aog, &AudioOutputGenerator::setAudioBuffer);

    voiceReceiver.start();
}


void FramedSocketWorker::SendReHello() {

    QJsonObject obj;
    obj["type"] = "ReHello";
    obj["peerId"] = peerId;

//    qDebug() << "Creating New Socket connection with Server";
    emit setDebugMessages("Creating New Socket connection with Server");

    QJsonDocument doc(obj);
    socket->writeDatagram(doc.toJson(QJsonDocument::Compact), QHostAddress(serverIp), 4855);
}


bool FramedSocketWorker::isConnectedToPeer() {

    return isConnected;
}


void FramedSocketWorker::initiateConnection(QString seedId, QString password) {

    otherPeer["peerId"] = seedId;
    otherPeer["password"] = password;

    tries = 0;
    SendPeerConnectRequest();
}


void FramedSocketWorker::getSeedName(const QString &seedname) {

    seedName = seedname;
}



/* Methods invoked at sender sides*/

void FramedSocketWorker::SendPeerConnectRequest() {

    if(tries >= 0 && tries < 3) {

        QJsonObject obj;
        obj["type"] = "PeerConnectRequest";
        obj["peerId"] = peerId;
        obj["peerName"] = seedName;
        obj["destinationPeerId"] = otherPeer["peerId"];
        obj["password"] = otherPeer["password"];

//        qDebug() << ("Sending PeerConnectRequest to " + otherPeer["peerId"].toString() + "\n");
        emit setDebugMessages("Sending PeerConnectRequest to " + otherPeer["peerId"].toString());

        QJsonDocument doc(obj);
        tries++;
        socket->writeDatagram(doc.toJson(QJsonDocument::Compact), QHostAddress(serverIp), 4855);

        QTimer::singleShot(3000, this, &FramedSocketWorker::SendPeerConnectRequest);
    }
}


void FramedSocketWorker::SendPeerConnectDummy() {

    if(tries >= 3 && tries < 6) {

        QJsonObject obj;
        obj["type"] = "PeerConnectDummy";

//        qDebug() << ("Sending PeerConnectDummy to " + otherPeer["peerId"].toString() + "\n");
        emit setDebugMessages("Sending PeerConnectDummy to " + otherPeer["peerId"].toString() +
                " > " + otherPeer["publicIp"].toString() + ":" + QString::number(otherPeer["nextPort"].toInt()));

        QJsonDocument doc(obj);
        tries++;
        socket->writeDatagram(doc.toJson(QJsonDocument::Compact),
                              QHostAddress(otherPeer["publicIp"].toString()),
                              otherPeer["nextPort"].toInt());

        QTimer::singleShot(3000, this, &FramedSocketWorker::SendPeerConnectDummy);
    }
}


void FramedSocketWorker::SendPeerConnectAckowledge() {

    if(tries >= 6 && tries < 9) {

        QJsonObject obj;
        obj["type"] = "PeerConnectAcknowledge";

//        qDebug() << ("Sending PeerConnectAcknowledge to " + otherPeer["peerId"].toString() + "\n");
        emit setDebugMessages("Sending PeerConnectAcknowledge to " + otherPeer["peerId"].toString());

        QJsonDocument doc(obj);
        tries++;
        socket->writeDatagram(doc.toJson(QJsonDocument::Compact),
                              QHostAddress(otherPeer["publicIp"].toString()),
                              otherPeer["nextPort"].toInt());

        QTimer::singleShot(3000, this, &FramedSocketWorker::SendPeerConnectAckowledge);
    }
}



/* Methods implementated at Receiver's side*/

void FramedSocketWorker::SendPeerConnectAccept(QJsonObject peerData) {

    if( peerData["password"].toString().compare(password) == 0) {
        peerData.remove("password");
        peerData.remove("type");
        otherPeer = peerData;
        setUsername(otherPeer["peerName"].toString());

        tries = 1;

        QJsonObject obj;
        obj["type"] = "PeerConnectAccept";
        obj["peerName"] = seedName;

//        qDebug() << ("Sending PeerConnectAccept to " + otherPeer["peerId"].toString() + "\n");
        emit setDebugMessages("Sending PeerConnectAccept to " + otherPeer["peerId"].toString() +
                " > " + otherPeer["publicIp"].toString() + ":" + QString::number(otherPeer["nextPort"].toInt()));

        QJsonDocument doc(obj);
        socket->writeDatagram(doc.toJson(QJsonDocument::Compact),
                              QHostAddress(otherPeer["publicIp"].toString()),
                              otherPeer["nextPort"].toInt());

        QTimer::singleShot(3000, this, &FramedSocketWorker::SendPeerConnectAcceptAgain);
    }
}


void FramedSocketWorker::SendPeerConnectAcceptAgain() {

    if(tries >= 1 && tries < 3) {

        tries++;

        QJsonObject obj;
        obj["type"] = "PeerConnectAccept";

//        qDebug() << ("Sending PeerConnectAcceptAgain to " + otherPeer["peerId"].toString() + "\n");
        emit setDebugMessages("Sending PeerConnectAcceptAgain to " + otherPeer["peerId"].toString()+
                " > " + otherPeer["publicIp"].toString() + ":" + QString::number(otherPeer["nextPort"].toInt()));

        QJsonDocument doc(obj);
        socket->writeDatagram(doc.toJson(QJsonDocument::Compact),
                              QHostAddress(otherPeer["publicIp"].toString()),
                              otherPeer["nextPort"].toInt());

        QTimer::singleShot(3000, this, &FramedSocketWorker::SendPeerConnectAcceptAgain);
    }
}


void FramedSocketWorker::SendPeerConnectFinish() {

    QJsonObject obj;
    obj["type"] = "PeerConnectFinish";

//    qDebug() << ("Sending PeerConnectFinish to " + otherPeer["peerId"].toString() + "\n");
    emit setDebugMessages("Sending PeerConnectFinish to " + otherPeer["peerId"].toString());

    QJsonDocument doc(obj);
    tries++;
    socket->writeDatagram(doc.toJson(QJsonDocument::Compact),
                          QHostAddress(otherPeer["publicIp"].toString()),
                          otherPeer["nextPort"].toInt());

//    QTimer::singleShot(3000, this, &FramedSocketWorker::SendPeerConnectFinish);
}



/* Common Read method*/

void FramedSocketWorker::ReadMessage() {

    while ( socket->hasPendingDatagrams()) {

        int size = socket->pendingDatagramSize();
        char *data = new char[ size];
        socket->readDatagram( data, size);

        QString datastr( data);
        datastr.truncate( datastr.lastIndexOf( '}') + 1);

        auto json = ( QJsonDocument::fromJson( datastr.toUtf8() ) ).object();

        delete[] data;

        QString typeOfPacket = json["type"].toString();

        if( typeOfPacket  == "ReHelloAcknowledge") {

//            qDebug() << "Received ReHello Ack";
            emit setDebugMessages("Received ReHello Ack");
        }

        /* Received by Sender */
        else if( typeOfPacket == "PeerConnectResponse") {

            emit setDebugMessages("Received PeerConnectResponse for "
                                  + json["publicIp"].toString() + ":" + QString::number(json["nextPort"].toInt()));
            tries = 3;

            json.remove("type");
            otherPeer = json;
            SendPeerConnectDummy();
        }

        else if( typeOfPacket == "PeerConnectAccept") {

            emit setDebugMessages("Received PeerConnectAccept");

            otherPeer["peerName"] = json["peerName"];
            setUsername(json["peerName"].toString());
            tries = 6;

            SendPeerConnectAckowledge();
        }

        else if( typeOfPacket == "PeerConnectFinish") {

            emit setDebugMessages("Received PeerConnectFinish");
            tries = 10;
            emit startAudioOutput();

//            qDebug() << ( "Successfully connected with peer " + otherPeer["peerId"].toString() + "\n");
            emit setDebugMessages( "Successfully connected with peer " + otherPeer["peerId"].toString());

            isConnected = true;
            emit connectionFinished();
        }


        /* Received by Receiver (room creator) */
        else if( typeOfPacket == "PeerConnectInitiate") {

            emit setDebugMessages("Received PeerConnectInitiate for "
                                  + json["publicIp"].toString() + ":" + QString::number(json["nextPort"].toInt()));
            SendPeerConnectAccept(json);
        }

        else if( typeOfPacket == "PeerConnectAcknowledge") {

            tries = 3;
            SendPeerConnectFinish();

            emit startAudioOutput();

//            qDebug() << ( "Successfully connected with peer " + otherPeer["peerId"].toString() + "\n");
            emit setDebugMessages( "Successfully connected with peer " + otherPeer["peerId"].toString());

            isConnected = true;
            emit connectionFinished();
        }


        /* For both*/
        else if( typeOfPacket == "Data") {

            QString typeOfData = json["dataType"].toString();

            if( typeOfData == "Video") {

                JsonToFrameRunnable *ftjr = new JsonToFrameRunnable();
                ftjr->setJsonValue(json["payload"]);
                connect(ftjr, &JsonToFrameRunnable::setFrame, this, &FramedSocketWorker::setNetworkFrame);

                QThreadPool::globalInstance()->start(ftjr);
            }

            else if( typeOfData == "Audio") {

                emit setJsonedAudio(json["payload"], json["size"].toInt());
                setIsActive(true);
            }

            else if (typeOfData == "Text") {

                CustomChatOutput *cco = new CustomChatOutput(json["payload"].toString(),
                        otherPeer["peerName"].toString(),
                        otherPeer["peerId"].toString(),
                        iconId, false);
                emit setTextReceived(cco);
            }
        }
    }
}


void FramedSocketWorker::sendText(QString data) {

    if(isConnectedToPeer() && !data.isEmpty()) {

        QJsonObject obj;
        obj["type"] = "Data";
        obj["dataType"] = "Text";
        obj["payload"] = data;

        QJsonDocument doc(obj);
        socket->writeDatagram(doc.toJson(QJsonDocument::Compact),
                              QHostAddress(otherPeer["publicIp"].toString()),
                              otherPeer["nextPort"].toInt());
    }
}


void FramedSocketWorker::sendJsonedFrame(QJsonValue frame) {

    if(isConnectedToPeer() && !frame.isNull()) {

        QJsonObject obj;
        obj["type"] = "Data";
        obj["dataType"] = "Video";
        obj["payload"] = frame;

        QJsonDocument doc(obj);
        socket->writeDatagram(doc.toJson(QJsonDocument::Compact),
                            QHostAddress(otherPeer["publicIp"].toString()),
                            otherPeer["nextPort"].toInt());
    }
}


void FramedSocketWorker::sendJsonedAudio(QJsonValue buffer, int size) {

    if(isConnectedToPeer() && !buffer.isNull()) {

        QJsonObject obj;
        obj["type"] = "Data";
        obj["dataType"] = "Audio";
        obj["payload"] = buffer;
        obj["size"] = QString::number(size);

        QJsonDocument doc(obj);
        socket->writeDatagram(doc.toJson(QJsonDocument::Compact),
                            QHostAddress(otherPeer["publicIp"].toString()),
                            otherPeer["nextPort"].toInt());
    }
}
