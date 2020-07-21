#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include <QObject>
#include <QUdpSocket>
#include <QCamera>
//#include <QAudioInput>
#include <QThread>
#include "CustomVideoOutput.h"


class GuiInterface : public QObject
{
    Q_OBJECT

    Q_PROPERTY(CustomVideoOutput* MyFeed READ MyFeed NOTIFY MyFeedChanged)

    Q_PROPERTY(QList<QObject*> PeerFeedList READ PeerFeedList NOTIFY PeerFeedListChanged)

    Q_PROPERTY(QString MyId READ MyId NOTIFY MyIdChanged)

    Q_PROPERTY(QString Password READ Password NOTIFY PasswordChanged)

public:
    explicit GuiInterface(QString ServerIp, QObject *parent = nullptr);

    CustomVideoOutput *MyFeed() const { return myFeed; }

    QList<QObject*> PeerFeedList() { return peerFeedList; }

    QString MyId() const { return myId;}

    QString Password() const { return password;}


public slots:

    void changeFeedSettings();

    void changeVocalSettings();

    void initiateConnection(QString peerId, QString password);

    void finishConnection();

    void sendText(QString text);

//    void readJsonAudio(QJsonValue value, int size);

signals:

    void MyFeedChanged();
    void PeerFeedListChanged();
    void switchToMainRoomView();

    void sendTextToWorker(QString text);

    void startVoiceRecorder();
    void pauseVoiceRecorder();
    void sendJsonedAudioToWorker(QJsonValue value, size_t size);

    void MyIdChanged();
    void PasswordChanged();


private slots:

    void RegistrationFinished();

    void createNewWorker();


private:
    void PrepareMyFeed();

    void PrepareMyVoice();

    void SendHello();

    QString myId;
    QString password;
    QString username;

    QUdpSocket *socket;

    QCamera *camera;
//    QIODevice *audioBuffer;
//    QAudioInput *audioIn;
    QThread voiceGenerator;

    QString serverIp;
    CustomVideoOutput *myFeed;
    QList<QObject*> peerFeedList;
};

#endif // GUIINTERFACE_H
