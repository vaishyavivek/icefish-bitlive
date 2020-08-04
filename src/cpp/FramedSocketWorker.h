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

    Q_PROPERTY(bool IsActive READ IsActive WRITE setIsActive NOTIFY IsActiveChanged)

public:

    explicit FramedSocketWorker(QString ServerIp, QString PeerId, QString Password, QObject *parent = nullptr);

    QString TextReceived() const { return textReceived;}

    bool IsActive() const { return isActive;}
    void setIsActive(bool IsActive) {
        if(isActive != IsActive) {
            isActive = IsActive;
            emit IsActiveChanged();
        }
    }

public slots:

    bool isConnectedToPeer();

    void initiateConnection(QString seedId, QString password, QString peerName);


    void sendText(QString data);

    void sendJsonedFrame(QJsonValue frame);

    void sendJsonedAudio(QJsonValue buffer, int size);


private slots:
    void ReadMessage();


signals:
    void TextReceivedChanged();

    void IsActiveChanged();

    void connectionFinished();

    void startAudioOutput();

    void setJsonedAudio(QJsonValue, int);

    void setDebugMessages(const QString &message);


private: //members
    QString peerId;
    QString password;

    QUdpSocket *socket;
    QString serverIp;
    int tries;

    QJsonObject otherPeer;
    bool isConnected = false;
    bool isActive = false;

    QString textReceived;

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
