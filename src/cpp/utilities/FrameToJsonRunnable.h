#ifndef FRAMETOIMAGERUNNABLE_H
#define FRAMETOIMAGERUNNABLE_H

#include <QRunnable>
#include <QJsonValue>
#include <QVideoFrame>
//#include <QOpenGLContext>
//#include <QOpenGLFunctions>
#include <QBuffer>
#include <QImageWriter>
#include <QDebug>


class FrameToJsonRunnable: public QObject, public QRunnable {
    Q_OBJECT

    void run() override {

        if (frame.isValid()) {

            QByteArray barray;
            QBuffer qbuffer(&barray);
            qbuffer.open(QIODevice::WriteOnly);

            QImageWriter writer(&qbuffer, "JPG");
            writer.setQuality(10);

            QImage img = frame.image();
            if (!img.isNull()) {

                img.scaled(240, 360);

                writer.write(img);

                QString str = QString::fromLatin1(barray.toBase64().data());
                emit setJsonValue(str);

            }

        }

    }


signals:
    void setJsonValue(QJsonValue value);

private:
    QVideoFrame frame;


public:
    void setFrame(const QVideoFrame &buffer) {
        frame = buffer;
    }

};


#endif // FRAMETOIMAGERUNNABLE_H
