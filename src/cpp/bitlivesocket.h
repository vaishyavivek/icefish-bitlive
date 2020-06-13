#ifndef BITLIVESOCKET_H
#define BITLIVESOCKET_H

#include <QObject>

#include <QUdpSocket>
#include <QJsonObject>

class BitliveSocket : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString DebugMessage READ DebugMessage NOTIFY DebugMessageChanged)

public:
    explicit BitliveSocket(QObject *parent = nullptr);

    QString DebugMessage() const{ return message;}

    void SendHello();

    void SendReHello(QString peerId);


signals:

    void DebugMessageChanged();


public slots:

    void initiateConnection(QString destinationPeerId, QString password);

    void sendData(QByteArray data);

    void ReadMessage();


private:
    void RegistrationFinished(QString peerId);

    void SendPeerConnectRequest();
    void SendPeerConnectAccept(QJsonObject peerData);
    void SendPeerConnectDummy();
    void SendPeerConnectAckowledge();
    void SendPeerConnectFinish();

    QUdpSocket *socket;
    int tries;
    QString message;
    QString myId;
    QString password;
    QJsonObject otherPeer;
};

#endif // BITLIVESOCKET_H
