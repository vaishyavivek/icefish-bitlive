#include "GuiInterface.h"

#include <QDebug>
#include <QCamera>
#include <QCameraInfo>
#include <QVideoProbe>
#include <QJsonDocument>
#include <QUdpSocket>
#include <QThreadPool>
#include <QRandomGenerator>

#include "FramedSocketWorker.h"
#include "utilities/AudioInputGenerator.h"

GuiInterface::GuiInterface(QString ServerIp, QObject *parent)
    : QObject(parent), serverIp(ServerIp) {

    PrepareMyFeed();

    PrepareMyVoice();

    SendHello();
}


void GuiInterface::PrepareMyFeed() {

    myFeed = new CustomVideoOutput("You");
    emit MyFeedChanged();

    auto camera = new QCamera();
    const QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    for (const QCameraInfo &cameraInfo : cameras) {
        if (cameraInfo.position() == QCamera::FrontFace) {
            camera = new QCamera(cameraInfo);
            cameraRotation = cameraInfo.orientation();
            emit CameraRotationChanged();
        }
    }


    camera->setCaptureMode(QCamera::CaptureVideo);

    auto probe = new QVideoProbe();
    if(probe->setSource(camera)) {
        connect(probe, &QVideoProbe::videoFrameProbed, myFeed, &CustomVideoOutput::setLocalFrame);
    }

    camera->start();
}

void GuiInterface::PrepareMyVoice() {


    AudioInputGenerator *aig = new AudioInputGenerator();
    aig->moveToThread(&voiceGenerator);
    connect(&voiceGenerator, &QThread::finished, aig, &QObject::deleteLater);
    connect(aig, &AudioInputGenerator::setAudioBuffer, this, &GuiInterface::sendJsonedAudioToWorker, Qt::QueuedConnection);
    connect(this, &GuiInterface::startVoiceRecorder, aig, &AudioInputGenerator::start);

    voiceGenerator.start();
    emit startVoiceRecorder();

}


void GuiInterface::SendHello() {

    QJsonObject obj;
    obj["type"] = "Hello";
    QJsonDocument doc(obj);

    qDebug() << ("Registering With Bitlive Tracker\n");

    socket = new QUdpSocket();

    socket->bind(QHostAddress("0.0.0.0"), 0, QAbstractSocket::ReuseAddressHint);

    connect(socket, &QUdpSocket::readyRead, this, &GuiInterface::RegistrationFinished, Qt::QueuedConnection);
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
    connect(this, &GuiInterface::sendJsonedAudioToWorker, fsw, &FramedSocketWorker::sendJsonedAudio);
    connect(this, &GuiInterface::sendTextToWorker, fsw, &FramedSocketWorker::sendText);
    connect(fsw, &FramedSocketWorker::connectionFinished, this, &GuiInterface::finishConnection);

    peerFeedList.append(fsw);
    emit PeerFeedListChanged();
}


void GuiInterface::initiateConnection(QString peerId, QString password) {

    if (peerFeedList.length() > 0) {
        auto last_fsw = qobject_cast<FramedSocketWorker*>(peerFeedList.last());
        if (!last_fsw->isConnectedToPeer())
            last_fsw->initiateConnection(peerId, password);
        else {
            qDebug() << "No connection available";
        }
    }

}


void GuiInterface::finishConnection() {

    if (peerFeedList.length() == 1) {
        emit switchToMainRoomView();
    }
    createNewWorker();
}


void GuiInterface::sendText(QString text) {

    if(!text.isEmpty() && !text.isNull())
        emit sendTextToWorker(text);
}
