#include "GuiInterface.h"

#include <QDebug>
#include <QCamera>
#include <QThread>
#include <QVideoProbe>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUdpSocket>
#include <QRandomGenerator>

#include "FramedSocketWorker.h"

GuiInterface::GuiInterface(QString ServerIp, QObject *parent)
    : QObject(parent), serverIp(ServerIp) {

    PrepareMyFeed();

    SendHello();
}


void GuiInterface::PrepareMyFeed() {

    myFeed = new CustomVideoOutput("You");
    emit MyFeedChanged();

    auto camera = new QCamera();
    camera->setCaptureMode(QCamera::CaptureVideo);

    auto probe = new QVideoProbe();
    if(probe->setSource(camera)) {
        connect(probe, &QVideoProbe::videoFrameProbed, myFeed, &CustomVideoOutput::setLocalFrame);
    }

    camera->start();
}


void GuiInterface::SendHello() {

    QJsonObject obj;
    obj["type"] = "Hello";
    QJsonDocument doc(obj);

    qDebug() << ("Registering With Bitlive Tracker\n");

    socket = new QUdpSocket();

    socket->bind(QHostAddress("0.0.0.0"), 0, QAbstractSocket::ReuseAddressHint);

    connect(socket, &QUdpSocket::readyRead, this, &GuiInterface::RegistrationFinished);
    socket->writeDatagram(doc.toJson(QJsonDocument::Compact), QHostAddress(serverIp), 4855);
}


void GuiInterface::RegistrationFinished() {

    while ( socket->hasPendingDatagrams()) {
        int size = socket->pendingDatagramSize();
        char *data = new char[ size];
        socket->readDatagram( data, size);

        QString datastr( data);
        datastr.truncate( datastr.lastIndexOf( '}') + 1);

        auto json = ( QJsonDocument::fromJson( datastr.toUtf8() ) ).object();

        if( json["type"].toString() == "PeerRegistered") {

            myId = QString::number( json["peerId"].toInt());
            emit MyIdChanged();

            password = (QString::number(QRandomGenerator::global()->generate(), 16));
            emit PasswordChanged();

            qDebug() << ("Successfully Registered With Bitlive Tracker\n");
            qDebug() << ("Your Id is " + myId + " and Password is " + password + "\n");

            createNewWorker();

            socket->close();
//            socket->deleteLater();
        }
    }
}


void GuiInterface::createNewWorker() {

    FramedSocketWorker *fsw = new FramedSocketWorker(serverIp, myId, password);

    connect(myFeed, &CustomVideoOutput::getJsonValue, fsw, &FramedSocketWorker::sendJsonedFrame);
    connect(this, &GuiInterface::sendTextToWorker, fsw, &FramedSocketWorker::sendText);
//    connect(fsw, &FramedSocketWorker::connectionFinished, this, &GuiInterface::createNewWorker);

    peerFeedList.append(fsw);
    emit PeerFeedListChanged();
}


void GuiInterface::initiateConnection(QString peerId, QString password) {

    auto last_fsw = qobject_cast<FramedSocketWorker*>(peerFeedList.last());
    last_fsw->initiateConnection(peerId, password);
}


void GuiInterface::sendText(QString text) {

    if(!text.isEmpty() && !text.isNull())
        emit sendTextToWorker(text);
}
