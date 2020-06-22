#ifndef FRAMEDSOCKETWORKER_H
#define FRAMEDSOCKETWORKER_H

#include <QObject>
#include <QVideoFrame>
#include <QUdpSocket>
#include <QJsonObject>

#include "CustomVideoOutput.h"

class FramedSocketWorker : public CustomVideoOutput
{
    Q_OBJECT

    Q_PROPERTY(QString TextReceived READ TextReceived NOTIFY TextReceivedChanged)

public:

    explicit FramedSocketWorker(QString ServerIp, QString PeerId, QString Password, QObject *parent = nullptr);

    QString TextReceived() const { return textReceived;}


public slots:

    void initiateConnection(QString destinationPeerId, QString password);

    void sendText(QString data);

    void sendJsonedFrame(QJsonValue frame);


private slots:
    void ReadMessage();


signals:
    void TextReceivedChanged();

    void connectionFinished();


private: //members
    QString peerId;
    QString password;

    QUdpSocket *socket;
    QString serverIp;
    int tries;

    QJsonObject otherPeer;

    QString textReceived;

    //methods

    void SendReHello();

    void SendPeerConnectRequest();

    void SendPeerConnectAccept(QJsonObject peerData);

    void SendPeerConnectDummy();

    void SendPeerConnectAckowledge();

    void SendPeerConnectFinish();
};

#endif // FRAMEDSOCKETWORKER_H
