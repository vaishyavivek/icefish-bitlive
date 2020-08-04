#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>

#include "GuiInterface.h"
#include "utilities/ShareUtils.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/main/android/icon.svg"));

    QCoreApplication::setApplicationName("IceFish Bitlive");
    QCoreApplication::setOrganizationDomain("icefish.host");
    QCoreApplication::setOrganizationName("IceFish Developers");

    QString mainQmlFile;


//#ifdef Q_OS_ANDROID
#define MAINFILE "qrc:/src/qml/mobile/main.qml"
//#else
//#define MAINFILE "qrc:/src/qml/desktop/main.qml"
//#endif

    qmlRegisterType<ShareUtils> ("com.lasconic", 1, 0, "ShareUtils");


    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral(MAINFILE));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    auto gui = new GuiInterface("3.6.254.253");

    auto ctxt = engine.rootContext();
    ctxt->setContextProperty("backend", gui);

    engine.load(url);

    return app.exec();
}
