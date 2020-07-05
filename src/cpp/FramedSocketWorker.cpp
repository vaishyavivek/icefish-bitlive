#include "FramedSocketWorker.h"

#include <QDebug>
#include <QJsonDocument>
#include <QTimer>
#include <QThreadPool>

#include "utilities/JsonToFrameRunnable.h"

FramedSocketWorker::FramedSocketWorker(QString ServerIp, QString PeerId, QString Password, QObject *parent)
    : CustomVideoOutput("" , parent), peerId(PeerId), password(Password), serverIp(ServerIp) {

    tries = 0;

    socket = new QUdpSocket();
    socket->bind(QHostAddress("0.0.0.0"), 0, QAbstractSocket::ReuseAddressHint);
    connect(socket, &QUdpSocket::readyRead, this, &FramedSocketWorker::ReadMessage);

    SendReHello();
}


void FramedSocketWorker::SendReHello() {

    QJsonObject obj;
    obj["type"] = "ReHello";
    obj["peerId"] = peerId;

    qDebug() << "Creating New Socket connection with Server";

    QJsonDocument doc(obj);
    socket->writeDatagram(doc.toJson(QJsonDocument::Compact), QHostAddress(serverIp), 4855);
}


void FramedSocketWorker::initiateConnection(QString destinationPeerId, QString password) {
    otherPeer["peerId"] = destinationPeerId;
    otherPeer["password"] = password;

    SendPeerConnectRequest();
}


void FramedSocketWorker::SendPeerConnectRequest() {

    if(tries >= 0 && tries < 3) {

        QJsonObject obj;
        obj["type"] = "PeerConnectRequest";
        obj["peerId"] = peerId;
        obj["destinationPeerId"] = otherPeer["peerId"];
        obj["password"] = otherPeer["password"];

        qDebug() << ("Sending PeerConnectRequest to " + otherPeer["peerId"].toString() + "\n");

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

        qDebug() << ("Sending PeerConnectDummy to " + otherPeer["peerId"].toString() + "\n");

        QJsonDocument doc(obj);
        tries++;
        socket->writeDatagram(doc.toJson(QJsonDocument::Compact),
                              QHostAddress(otherPeer["publicIp"].toString()),
                              otherPeer["nextPort"].toInt());

        QTimer::singleShot(3000, this, &FramedSocketWorker::SendPeerConnectDummy);
    }
}


void FramedSocketWorker::SendPeerConnectAccept(QJsonObject peerData) {

    if( peerData["password"].toString().compare(password) == 0) {
        peerData.remove("password");
        peerData.remove("type");
        otherPeer = peerData;

        QJsonObject obj;
        obj["type"] = "PeerConnectAccept";

        qDebug() << ("Sending PeerConnectAccept to " + otherPeer["peerId"].toString() + "\n");

        QJsonDocument doc(obj);
        socket->writeDatagram(doc.toJson(QJsonDocument::Compact),
                              QHostAddress(otherPeer["publicIp"].toString()),
                              otherPeer["nextPort"].toInt());
    }
}


void FramedSocketWorker::SendPeerConnectAckowledge() {

    if(tries >= 6 && tries < 9) {

        QJsonObject obj;
        obj["type"] = "PeerConnectAcknowledge";

        qDebug() << ("Sending PeerConnectAcknowledge to " + otherPeer["peerId"].toString() + "\n");

        QJsonDocument doc(obj);
        tries++;
        socket->writeDatagram(doc.toJson(QJsonDocument::Compact),
                              QHostAddress(otherPeer["publicIp"].toString()),
                              otherPeer["nextPort"].toInt());

        QTimer::singleShot(3000, this, &FramedSocketWorker::SendPeerConnectAckowledge);
    }
}


void FramedSocketWorker::SendPeerConnectFinish() {

    QJsonObject obj;
    obj["type"] = "PeerConnectFinish";

    qDebug() << ("Sending PeerConnectFinish to " + otherPeer["peerId"].toString() + "\n");

    QJsonDocument doc(obj);
    tries++;
    socket->writeDatagram(doc.toJson(QJsonDocument::Compact),
                          QHostAddress(otherPeer["publicIp"].toString()),
                          otherPeer["nextPort"].toInt());

//    QTimer::singleShot(3000, this, &FramedSocketWorker::SendPeerConnectFinish);
}


void FramedSocketWorker::ReadMessage() {

    while ( socket->hasPendingDatagrams()) {

        int size = socket->pendingDatagramSize();
        char *data = new char[ size];
        socket->readDatagram( data, size);

        QString datastr( data);
        datastr.truncate( datastr.lastIndexOf( '}') + 1);

        auto json = ( QJsonDocument::fromJson( datastr.toUtf8() ) ).object();

        if( json["type"].toString().compare("ReHelloAcknowledge") == 0) {

            qDebug() << "Received ReHello Ack";
        }

        else if( json["type"].toString() == "PeerConnectResponse") {

            tries = 3;

            json.remove("type");
            otherPeer = json;
            SendPeerConnectDummy();
        }

        else if( json["type"].toString() == "PeerConnectInitiate") {

            SendPeerConnectAccept(json);
        }

        else if( json["type"].toString() == "PeerConnectAccept") {

            tries = 6;

            SendPeerConnectAckowledge();
        }

        else if( json["type"].toString() == "PeerConnectAcknowledge") {

            tries = 9;

            SendPeerConnectFinish();

            qDebug() << ( "Successfully connected with peer " + otherPeer["peerId"].toString() + "\n");
            emit connectionFinished();
        }

        else if( json["type"].toString() == "PeerConnectFinish") {

            tries = 12;

            qDebug() << ( "Successfully connected with peer " + otherPeer["peerId"].toString() + "\n");
            emit connectionFinished();
        }

        else if( json["type"].toString() == "Data") {

            if(json["dataType"].toString() == "Video") {

                JsonToFrameRunnable *ftjr = new JsonToFrameRunnable();
                ftjr->setJsonValue(json["payload"]);
                connect(ftjr, &JsonToFrameRunnable::setFrame, this, &FramedSocketWorker::setNetworkFrame);

                QThreadPool::globalInstance()->start(ftjr);
            }
            else {

                textReceived = json["payload"].toString();
                emit TextReceivedChanged();
            }
        }
    }
}


void FramedSocketWorker::sendText(QString data) {

    if(!otherPeer.isEmpty() && !data.isEmpty()) {

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

    if(!otherPeer.isEmpty() && !frame.isNull()) {

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
