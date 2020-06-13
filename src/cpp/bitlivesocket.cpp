#include "bitlivesocket.h"

#include <QJsonDocument>
#include <QRandomGenerator>
#include <QDebug>
#include <QTimer>

BitliveSocket::BitliveSocket(QObject *parent)
    : QObject(parent),
        socket(new QUdpSocket){

    socket->bind(QHostAddress("0.0.0.0"), 0, QAbstractSocket::ReuseAddressHint);

    connect(socket, &QUdpSocket::readyRead, this, &BitliveSocket::ReadMessage);
    tries = 0;
}

void BitliveSocket::SendHello() {
    QJsonObject obj;
    obj["type"] = "Hello";

    message.append("Registering With Bitlive Tracker\n");
    emit DebugMessageChanged();

    QJsonDocument doc(obj);
    socket->writeDatagram(doc.toJson(QJsonDocument::Compact), QHostAddress("15.206.134.153"), 4855);
}


void BitliveSocket::SendReHello(QString peerId) {
    QJsonObject obj;
    obj["type"] = "Hello";
    obj["peerId"] = myId = peerId;

    message.append("Creating New Connection With Bitlive Tracker\n");
    emit DebugMessageChanged();

    QJsonDocument doc(obj);
    socket->writeDatagram(doc.toJson(QJsonDocument::Compact), QHostAddress("15.206.134.153"), 4855);
}


void BitliveSocket::RegistrationFinished(QString peerId) {
    myId = peerId;
    password = QString::number(QRandomGenerator::global()->generate(), 16);

    message.append("Successfully Registered With Bitlive Tracker\n");
    message.append("Your Id is " + myId + " and Password is " + password + "\n");
    emit DebugMessageChanged();
}


void BitliveSocket::initiateConnection(QString destinationPeerId, QString password) {
    otherPeer["peerId"] = destinationPeerId;
    otherPeer["password"] = password;

    SendPeerConnectRequest();
}


void BitliveSocket::SendPeerConnectRequest() {
    if(tries >= 0 && tries < 4) {

        QJsonObject obj;
        obj["type"] = "PeerConnectRequest";
        obj["peerId"] = myId;
        obj["destinationPeerId"] = otherPeer["peerId"];
        obj["password"] = otherPeer["password"];

        message.append("Sending PeerConnectRequest to " + otherPeer["peerId"].toString() + "\n");
        emit DebugMessageChanged();

        QJsonDocument doc(obj);
        tries++;
        socket->writeDatagram(doc.toJson(QJsonDocument::Compact), QHostAddress("15.206.134.153"), 4855);

        QTimer::singleShot(3000, this, &BitliveSocket::SendPeerConnectRequest);
    }
}


void BitliveSocket::SendPeerConnectDummy() {

    if(tries >= 4 && tries < 8) {

        QJsonObject obj;
        obj["type"] = "PeerConnectDummy";

        message.append("Sending PeerConnectDummy to " + otherPeer["peerId"].toString() + "\n");
        emit DebugMessageChanged();

        QJsonDocument doc(obj);
        tries++;
        socket->writeDatagram(doc.toJson(QJsonDocument::Compact),
                              QHostAddress(otherPeer["publicIp"].toString()),
                              otherPeer["nextPort"].toInt());

        QTimer::singleShot(3000, this, &BitliveSocket::SendPeerConnectDummy);
    }

}


void BitliveSocket::SendPeerConnectAccept(QJsonObject peerData) {

    if( peerData["password"].toString().compare(password) == 0) {
        peerData.remove("password");
        peerData.remove("type");
        otherPeer = peerData;

        QJsonObject obj;
        obj["type"] = "PeerConnectAccept";

        message.append("Sending PeerConnectAccept to " + otherPeer["peerId"].toString() + "\n");
        emit DebugMessageChanged();

        QJsonDocument doc(obj);
        socket->writeDatagram(doc.toJson(QJsonDocument::Compact),
                              QHostAddress(otherPeer["publicIp"].toString()),
                              otherPeer["nextPort"].toInt());
    }
}


void BitliveSocket::SendPeerConnectAckowledge() {

    if(tries >= 8 && tries < 12) {

        QJsonObject obj;
        obj["type"] = "PeerConnectAcknowledge";

        message.append("Sending PeerConnectAcknowledge to " + otherPeer["peerId"].toString() + "\n");
        emit DebugMessageChanged();

        QJsonDocument doc(obj);
        tries++;
        socket->writeDatagram(doc.toJson(QJsonDocument::Compact),
                              QHostAddress(otherPeer["publicIp"].toString()),
                              otherPeer["nextPort"].toInt());

        QTimer::singleShot(3000, this, &BitliveSocket::SendPeerConnectAckowledge);
    }
}


void BitliveSocket::SendPeerConnectFinish() {

    QJsonObject obj;
    obj["type"] = "PeerConnectFinish";

    message.append("Sending PeerConnectFinish to " + otherPeer["peerId"].toString() + "\n");
    emit DebugMessageChanged();

    QJsonDocument doc(obj);
    tries++;
    socket->writeDatagram(doc.toJson(QJsonDocument::Compact),
                          QHostAddress(otherPeer["publicIp"].toString()),
                          otherPeer["nextPort"].toInt());

//    QTimer::singleShot(3000, this, &BitliveSocket::SendPeerConnectFinish);
}


void BitliveSocket::ReadMessage() {

    while ( socket->hasPendingDatagrams()) {
        int size = socket->pendingDatagramSize();
        char *data = new char[ size];
        socket->readDatagram( data, size);

//        message.append( data);
//        message.append( "\n");
//        emit DebugMessageChanged();

        QString datastr( data);
        datastr.truncate( datastr.lastIndexOf( '}') + 1);
        qDebug() << datastr;

        auto json = ( QJsonDocument::fromJson( datastr.toUtf8() ) ).object();

        if( json["type"].toString() == "PeerRegistered") {

            RegistrationFinished( QString::number( json["peerId"].toInt()) );
        }

        else if( json["type"].toString() == "PeerConnectResponse") {

            tries = 4;

            json.remove("type");
            otherPeer = json;
            SendPeerConnectDummy();
        }

        else if( json["type"].toString() == "PeerConnectInitiate") {

            SendPeerConnectAccept(json);
        }

        else if( json["type"].toString() == "PeerConnectAccept") {

            tries = 8;

            SendPeerConnectAckowledge();
        }

        else if( json["type"].toString() == "PeerConnectAcknowledge") {

            tries = 12;

            SendPeerConnectFinish();

            message.append( "Successfully connected with peer " + otherPeer["peerId"].toString() + "\n");
            emit DebugMessageChanged();
        }

        else if( json["type"].toString() == "PeerConnectFinish") {

            tries = 16;

            message.append( "Successfully connected with peer " + otherPeer["peerId"].toString() + "\n");
            emit DebugMessageChanged();
        }

        else if( json["type"].toString() == "Data") {
            message.append("Received " + json["payload"].toString() + "\n");
            emit DebugMessageChanged();
        }
    }
}


void BitliveSocket::sendData(QByteArray data) {
    QJsonObject obj;
    obj["type"] = "Data";
    obj["payload"] = QString::fromUtf8(data);

    QJsonDocument doc(obj);
    socket->writeDatagram(doc.toJson(QJsonDocument::Compact),
                          QHostAddress(otherPeer["publicIp"].toString()),
                          otherPeer["nextPort"].toInt());
}
