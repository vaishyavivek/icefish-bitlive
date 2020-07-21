#ifndef CUSTOMVIDEOOUTPUT_H
#define CUSTOMVIDEOOUTPUT_H

#include <QObject>
#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include <QThreadPool>

#include "utilities/FrameToJsonRunnable.h"

class CustomVideoOutput: public QObject {
    Q_OBJECT

    Q_PROPERTY(QAbstractVideoSurface* videoSurface READ VideoSurface WRITE setVideoSurface)

    Q_PROPERTY(int CameraRotation READ CameraRotation WRITE setCameraRotation NOTIFY CameraRotationChanged)

    Q_PROPERTY(QString Username READ Username NOTIFY UsernameChanged)

public:
    CustomVideoOutput(QString Username, QObject *parent = nullptr)
        :QObject(parent), userName(Username) {}

    QAbstractVideoSurface* VideoSurface() { return mySurface;}

    void setVideoSurface(QAbstractVideoSurface *surface) {
        if (mySurface != surface && mySurface && mySurface->isActive()) {
            mySurface->stop();
        }

        mySurface = surface;

        if (mySurface && myFormat.isValid()) {
            myFormat = mySurface->nearestFormat(myFormat);
            mySurface->start(myFormat);
        }
    }

    void clearFrame() {
        mySurface->stop();

    }

    int CameraRotation() const { return cameraRotation;}

    void setCameraRotation(int CameraRotation) {
        if(cameraRotation != CameraRotation) {
            cameraRotation = CameraRotation;
            emit CameraRotationChanged();
        }
    }

    void setFormat(int width, int height, QVideoFrame::PixelFormat frameFormat) {
        QSize size(width, height);
        QVideoSurfaceFormat format(size, frameFormat);
        myFormat = format;
        if (mySurface) {
            if (mySurface->isActive())
                mySurface->stop();
            myFormat = mySurface->nearestFormat(myFormat);
            mySurface->start(myFormat);
        }
    }

    void setLocalFrame(const QVideoFrame &frame) {
        if (mySurface) {

            FrameToJsonRunnable *ftir = new FrameToJsonRunnable();
            ftir->setFrame(frame, cameraRotation);
            connect(ftir, &FrameToJsonRunnable::setJsonValue, this, &CustomVideoOutput::getJsonValue);
            QThreadPool::globalInstance()->start(ftir);

            setFormat(frame.width(), frame.height(), frame.pixelFormat());
            mySurface->present(frame);
        }
    }

    void setNetworkFrame(const QVideoFrame &frame) {
        if (mySurface) {
            setFormat(frame.width(), frame.height(), frame.pixelFormat());
            mySurface->present(frame);
        }
    }

    QString Username() const { return userName;}


signals:
    void UsernameChanged();
    void CameraRotationChanged();
    void getJsonValue(QJsonValue json);

private:
    QString userName;
    QAbstractVideoSurface *mySurface = NULL;
    int cameraRotation = 0;
    QVideoSurfaceFormat myFormat;
};


#endif // CUSTOMVIDEOOUTPUT_H
