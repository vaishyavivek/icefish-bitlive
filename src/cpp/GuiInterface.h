#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include <QObject>
#include <QUdpSocket>
#include "CustomVideoOutput.h"


class GuiInterface : public QObject
{
    Q_OBJECT

    Q_PROPERTY(CustomVideoOutput* MyFeed READ MyFeed NOTIFY MyFeedChanged)

    Q_PROPERTY(QList<QObject*> PeerFeedList READ PeerFeedList NOTIFY PeerFeedListChanged)

public:
    explicit GuiInterface(QString ServerIp, QObject *parent = nullptr);

    CustomVideoOutput *MyFeed() const { return myFeed; }

    QList<QObject*> PeerFeedList() { return peerFeedList; }


public slots:

    void initiateConnection(QString peerId, QString password);

    void sendText(QString text);

signals:
    void MyFeedChanged();
    void PeerFeedListChanged();

    void sendTextToWorker(QString text);

private slots:

    void RegistrationFinished();

    void createNewWorker();

private:
    void PrepareMyFeed();

    void SendHello();

    QString myId;
    QString password;
    QString username;
    QUdpSocket *socket;

    QString serverIp;
    CustomVideoOutput *myFeed;
    QList<QObject*> peerFeedList;
};

#endif // GUIINTERFACE_H
