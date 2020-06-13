#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "bitlivesocket.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/src/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    auto socket = new BitliveSocket();
    auto ctxt = engine.rootContext();
    ctxt->setContextProperty("socket", socket);

    socket->SendHello();

    engine.load(url);

    return app.exec();
}
