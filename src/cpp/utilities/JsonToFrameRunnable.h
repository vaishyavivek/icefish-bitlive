#ifndef JSONTOFRAMERUNNABLE_H
#define JSONTOFRAMERUNNABLE_H

#include <QObject>
#include <QRunnable>
#include <QVideoFrame>
#include <QJsonValue>

class JsonToFrameRunnable: public QObject, public QRunnable {
    Q_OBJECT

    void run() override {

        auto const encoded = json.toString().toLatin1();
        QImage img;
        img.loadFromData(QByteArray::fromBase64(encoded), "PNG");

        QVideoFrame frame(img);
        emit setFrame(frame);
    }

signals:
    void setFrame(const QVideoFrame &frame);

private:
    QJsonValue json;

public:
    void setJsonValue(QJsonValue jsonBuffer) {
        json = jsonBuffer;
    }
};


#endif // JSONTOFRAMERUNNABLE_H
