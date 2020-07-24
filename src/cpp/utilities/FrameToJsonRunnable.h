#ifndef FRAMETOIMAGERUNNABLE_H
#define FRAMETOIMAGERUNNABLE_H

#include <QRunnable>
#include <QJsonValue>
#include <QVideoFrame>
#include <QBuffer>
#include <QImageWriter>
//#include <QTransform>
//#include <QDebug>


class FrameToJsonRunnable: public QObject, public QRunnable {
    Q_OBJECT

    void run() override {

        if (frame.isValid()) {

            QByteArray barray;
            QBuffer qbuffer(&barray);
            qbuffer.open(QIODevice::WriteOnly);

            QImageWriter writer(&qbuffer, "JPG");
            writer.setQuality(qualityBar*10);

            QImage img = frame.image();
            if (!img.isNull()) {

                QTransform rot;
                rot.rotate(-rotation);
                img = img.transformed(rot);

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
    int rotation;
    int qualityBar;

public:
    void setFrame(const QVideoFrame &buffer, int Rotation, int QualityBar) {
        frame = buffer;
        rotation = Rotation;
        qualityBar = QualityBar;
    }

};


#endif // FRAMETOIMAGERUNNABLE_H
