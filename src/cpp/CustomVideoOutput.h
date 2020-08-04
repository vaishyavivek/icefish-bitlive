#ifndef CUSTOMVIDEOOUTPUT_H
#define CUSTOMVIDEOOUTPUT_H

#include <QObject>
#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include <QThreadPool>
#include <QDebug>

#include "utilities/FrameToJsonRunnable.h"

class CustomVideoOutput: public QObject {
    Q_OBJECT

    Q_PROPERTY(QAbstractVideoSurface* videoSurface READ VideoSurface WRITE setVideoSurface)

    Q_PROPERTY(QString Username READ Username WRITE setUserName NOTIFY UsernameChanged)

    Q_PROPERTY(int CameraRotation READ CameraRotation WRITE setCameraRotation NOTIFY CameraRotationChanged)

    Q_PROPERTY(int QualityBar READ QualityBar WRITE setQualityBar NOTIFY QualityBarChanged)

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
            ftir->setFrame(frame, cameraRotation, qualityBar + 1);
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

    void setUserName(const QString &UserName) {
        if (userName != UserName) {
            userName = UserName;
            emit UsernameChanged();
        }
    }


    int CameraRotation() const { return cameraRotation;}

    void setCameraRotation(int CameraRotation) {
        if(cameraRotation != CameraRotation) {
            cameraRotation = CameraRotation;
            emit CameraRotationChanged();
        }
    }


    int QualityBar() const { return qualityBar;}

    void setQualityBar(int QualityBar) {
        if(qualityBar != QualityBar) {
            qualityBar = QualityBar;
            emit QualityBarChanged();
            qDebug() << qualityBar;
        }
    }


signals:

    void UsernameChanged();

    void CameraRotationChanged();

    void QualityBarChanged();


    void getJsonValue(QJsonValue json);

private:
    QString userName;
    int cameraRotation = 0;
    int qualityBar = 0;
    QAbstractVideoSurface *mySurface = NULL;
    QVideoSurfaceFormat myFormat;
};


#endif // CUSTOMVIDEOOUTPUT_H
