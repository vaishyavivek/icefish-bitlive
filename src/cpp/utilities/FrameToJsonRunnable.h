#ifndef FRAMETOIMAGERUNNABLE_H
#define FRAMETOIMAGERUNNABLE_H

#include <QRunnable>
#include <QJsonValue>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QBuffer>
#include <QImageWriter>
#include "private/qvideoframe_p.h"


class FrameToJsonRunnable: public QObject, public QRunnable {
    Q_OBJECT

    void run() override {

        QByteArray barray;
        QBuffer qbuffer(&barray);
        qbuffer.open(QIODevice::WriteOnly);

        QImageWriter writer(&qbuffer, "PNG");
//        writer.setQuality(10);

        if ( frame.handleType() == QAbstractVideoBuffer::NoHandle) {
            QImage image = qt_imageFromVideoFrame( frame );
            if ( image.isNull() )
                return;

            if ( image.format() != QImage::Format_ARGB32 )
                image = image.convertToFormat( QImage::Format_ARGB32 );

            writer.write(image);
        }

        if ( frame.handleType() == QAbstractVideoBuffer::GLTextureHandle ) {
            QImage image( frame.width(), frame.height(), QImage::Format_ARGB32 );
            GLuint textureId = static_cast<GLuint>( frame.handle().toInt() );
            QOpenGLContext* ctx = QOpenGLContext::currentContext();
            QOpenGLFunctions* f = ctx->functions();
            GLuint fbo;
            f->glGenFramebuffers( 1, &fbo );
            GLint prevFbo;
            f->glGetIntegerv( GL_FRAMEBUFFER_BINDING, &prevFbo );
            f->glBindFramebuffer( GL_FRAMEBUFFER, fbo );
            f->glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0 );
            f->glReadPixels( 0, 0,  frame.width(),  frame.height(), GL_RGBA, GL_UNSIGNED_BYTE, image.bits() );
            f->glBindFramebuffer( GL_FRAMEBUFFER, static_cast<GLuint>( prevFbo ) );

            writer.write(image.rgbSwapped());
        }

        emit setJsonValue(QLatin1String(barray.toBase64()));
        return;
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
