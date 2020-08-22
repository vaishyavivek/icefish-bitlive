/* $Id: GuiInterface.cpp
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

#include "GuiInterface.h"

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

    debugMessage = "";

    debugFile.setFileName("icefish-bitlive.log");
    if (!debugFile.open(QIODevice::WriteOnly | QIODevice::Append))
        qDebug() << "Could not open log file";


    PrepareMyFeed();

    PrepareMyVoice();

    SendHello();
}


void GuiInterface::PrepareMyFeed() {

    myFeed = new CustomVideoOutput("You");
    emit MyFeedChanged();

    camera = new QCamera();
    const QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    for (const QCameraInfo &cameraInfo : cameras) {
        if (cameraInfo.position() == QCamera::FrontFace) {
            camera = new QCamera(cameraInfo);
            myFeed->setCameraRotation(cameraInfo.orientation());
        }
    }

    QCameraViewfinderSettings viewfinderSettings;
    viewfinderSettings.setResolution(640, 480);
    viewfinderSettings.setMinimumFrameRate(5);
    viewfinderSettings.setMaximumFrameRate(15);

    camera->setViewfinderSettings(viewfinderSettings);

    camera->setCaptureMode(QCamera::CaptureVideo);

    auto probe = new QVideoProbe();
    if(probe->setSource(camera)) {
        connect(probe, &QVideoProbe::videoFrameProbed, myFeed, &CustomVideoOutput::setLocalFrame);
    }

    camera->start();
    emit IsVideoRunningChanged();
}


void GuiInterface::setIsVideoRunning(bool IsVideoRunning) {

    Q_UNUSED(IsVideoRunning);

    if (camera->status() == QCamera::ActiveStatus) {
        camera->stop();
        myFeed->clearFrame();
    }
    else {
        camera->start();
    }
    emit IsVideoRunningChanged();
}



void GuiInterface::PrepareMyVoice() {


    AudioInputGenerator *aig = new AudioInputGenerator();
    aig->moveToThread(&voiceGenerator);
    connect(&voiceGenerator, &QThread::finished, aig, &QObject::deleteLater);
    connect(aig, &AudioInputGenerator::setAudioBuffer, this, &GuiInterface::sendJsonedAudioToWorker);
    connect(this, &GuiInterface::startVoiceRecorder, aig, &AudioInputGenerator::start);
    connect(this, &GuiInterface::pauseVoiceRecorder, aig, &AudioInputGenerator::pause);

    voiceGenerator.start();
    emit startVoiceRecorder();

    isAudioRunning = true;

}


void GuiInterface::setIsAudioRunning(bool IsAudioRunning) {

    Q_UNUSED(IsAudioRunning);

    emit pauseVoiceRecorder();
    isAudioRunning = !isAudioRunning;
    emit IsAudioRunningChanged();
}




void GuiInterface::SendHello() {

    QJsonObject obj;
    obj["type"] = "Hello";
    QJsonDocument doc(obj);

//    qDebug() << ("Registering With Bitlive Tracker\n");
    setDebugMessage("Registering With Bitlive Tracker");

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

            iconId = QRandomGenerator::global()->generate() % 6;
            password = (QString::number(QRandomGenerator::global()->generate(), 16));
            password = password.mid(0, 4);

            roomId = myId;
            roomId.insert(3, "-");
            roomId += "-" + password;
            emit RoomIdChanged();

            setDebugMessage("Successfully Registered With Bitlive Tracker");
            setDebugMessage("Your Id is " + myId + " and Password is " + password);


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
    connect(fsw, &FramedSocketWorker::setTextReceived, this, &GuiInterface::receiveText);
    connect(myFeed, &CustomVideoOutput::SeednameChanged, fsw, &FramedSocketWorker::getSeedName);
    connect(fsw, &FramedSocketWorker::setDebugMessages, this, &GuiInterface::setDebugMessage);
    connect(fsw, &FramedSocketWorker::connectionFinished, this, &GuiInterface::finishConnection);

    peerFeedList.append(fsw);
    emit PeerFeedListChanged();
}




void GuiInterface::initiateConnection(QString seedRoomId) {

    //input seedRoomId is in the form of 123-456-abcd
    QString seedId = seedRoomId.mid(0, 3) + seedRoomId.mid(4, 3);
    QString password = seedRoomId.mid(8, 4);

    if (peerFeedList.length() > 0) {
        auto last_fsw = qobject_cast<FramedSocketWorker*>(peerFeedList.last());
        if (!last_fsw->isConnectedToPeer())
            last_fsw->initiateConnection(seedId, password);
        else {
            qDebug() << "No connection available";
            setDebugMessage("No connection available");
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

    if(!text.isEmpty() && !text.isNull()) {

        CustomChatOutput *cco = new CustomChatOutput(text, "You", "", iconId, true);
        chatHistory.append(cco);
        emit ChatHistoryChanged();

        emit sendTextToWorker(text);
    }
}


void GuiInterface::receiveText(CustomChatOutput *cco) {

    chatHistory.append(cco);
    emit ChatHistoryChanged();
}
