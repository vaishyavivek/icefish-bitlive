/* $Id: main.cpp
 * IceFish bitLive aims to solve end to end connectivity issue
 * without any dependency on a centralized server
 * by using a complete P2P driven approach.
 * © IceFish Community, 2020
 *
 * This file is part of IceFish bitLive.
 *
 * IceFish bitLive is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * IceFish bitLive is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with IceFish bitLive.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This project is hosted here,
 * https://github.com/vaishyavivek/icefish-bitlive
*/

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
