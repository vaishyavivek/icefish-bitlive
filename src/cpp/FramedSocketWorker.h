#ifndef FRAMEDSOCKETWORKER_H
#define FRAMEDSOCKETWORKER_H

#include <QObject>
#include <QVideoFrame>
#include <QUdpSocket>
#include <QJsonObject>
#include <QThread>

#include "CustomVideoOutput.h"

class FramedSocketWorker : public CustomVideoOutput
{
    Q_OBJECT

    Q_PROPERTY(QString TextReceived READ TextReceived NOTIFY TextReceivedChanged)

public:

    explicit FramedSocketWorker(QString ServerIp, QString PeerId, QString Password, QObject *parent = nullptr);

    QString TextReceived() const { return textReceived;}


public slots:

    bool isConnectedToPeer();

    void initiateConnection(QString destinationPeerId, QString password);

    void sendText(QString data);

    void sendJsonedFrame(QJsonValue frame);

    void sendJsonedAudio(QJsonValue buffer, int size);


private slots:
    void ReadMessage();


signals:
    void TextReceivedChanged();

    void connectionFinished();

    void startAudioOutput();

    void setJsonedAudio(QJsonValue, int);

private: //members
    QString peerId;
    QString password;

    QUdpSocket *socket;
    QString serverIp;
    int tries;

    QJsonObject otherPeer;
    bool isConnected = false;

    QString textReceived;

//    QAudioOutput *audioOutput;
//    QIODevice *audioDevice;
    QThread voiceReceiver;

    //methods

    void SendReHello();

    void SendPeerConnectRequest();

    void SendPeerConnectAccept(QJsonObject peerData);

    void SendPeerConnectDummy();

    void SendPeerConnectAckowledge();

    void SendPeerConnectFinish();
};

#endif // FRAMEDSOCKETWORKER_H
