#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include <QObject>
#include <QUdpSocket>
#include <QCamera>
//#include <QAudioInput>
#include <QThread>
#include <QFile>
#include "CustomVideoOutput.h"


class GuiInterface : public QObject
{
    Q_OBJECT

    Q_PROPERTY(CustomVideoOutput* MyFeed READ MyFeed NOTIFY MyFeedChanged)

    Q_PROPERTY(QList<QObject*> PeerFeedList READ PeerFeedList NOTIFY PeerFeedListChanged)

    Q_PROPERTY(QString RoomId READ RoomId NOTIFY RoomIdChanged)

    Q_PROPERTY(bool IsVideoRunning READ IsVideoRunning WRITE setIsVideoRunning NOTIFY IsVideoRunningChanged)

    Q_PROPERTY(bool IsAudioRunning READ IsAudioRunning WRITE setIsAudioRunning NOTIFY IsAudioRunningChanged)


public:
    explicit GuiInterface(QString ServerIp, QObject *parent = nullptr);


    //returns my video stream from camera
    CustomVideoOutput *MyFeed() const { return myFeed; }


    //returns framedSocketWorker list of all the peers
    QList<QObject*> PeerFeedList() { return peerFeedList; }


    //returns seedid + password
    QString RoomId() const { return roomId;}


    //controls whether the camera feed is running
    bool IsVideoRunning() { return camera->state() == QCamera::ActiveState; }
    void setIsVideoRunning(bool IsVideoRunning);


    //instruct the videogenerator thread to stop or continue processing
    bool IsAudioRunning() const{ return isAudioRunning;}
    void setIsAudioRunning(bool IsAudioRunning);


    //write the debug message to 'icefish-bitlive.log' file
    void setDebugMessage(const QString &DebugMessage) {

        QDateTime local(QDateTime::currentDateTime());

        QTextStream stream(&debugFile);
        stream << local.toString() << " >> " << DebugMessage << "\n";
    }

    ~GuiInterface() {
        debugFile.close();
    }


public slots:

    //use android intent to share the room id
    void sharePeerId();

    void initiateConnection(QString seedRoomid, QString peerName);

    void finishConnection();

    void sendText(QString text);

//    void readJsonAudio(QJsonValue value, int size);

signals:

    void RoomIdChanged();

    void IsVideoRunningChanged();

    void IsAudioRunningChanged();

    void MyFeedChanged();

    void PeerFeedListChanged();

    void switchToMainRoomView();

    void sendTextToWorker(QString text);

    void startVoiceRecorder();
    void pauseVoiceRecorder();

    void sendJsonedAudioToWorker(QJsonValue value, size_t size);


private slots:

    void RegistrationFinished();

    void createNewWorker();


private:
    void PrepareMyFeed();

    void PrepareMyVoice();

    void SendHello();


    QString serverIp;


    QString roomId;
    QString myId;
    QString password;


    QUdpSocket *socket;

    QCamera *camera;

    QThread voiceGenerator;
    bool isAudioRunning;


    CustomVideoOutput *myFeed;

    QList<QObject*> peerFeedList;

    QString debugMessage;
    QFile debugFile;
};

#endif // GUIINTERFACE_H
