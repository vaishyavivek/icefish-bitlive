QT += quick multimedia

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        src/cpp/FramedSocketWorker.cpp \
        src/cpp/GuiInterface.cpp \
        src/cpp/main.cpp \
        src/cpp/utilities/AudioInputGenerator.cpp \
        src/cpp/utilities/AudioOutputGenerator.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#LIBS += -lopus
#INCLUDEPATH += src/opus/include \
#        src/opus/src
#LIBS += -llibopus

HEADERS += \
    src/cpp/CustomVideoOutput.h \
    src/cpp/utilities/AudioInputGenerator.h \
    src/cpp/utilities/AudioOutputGenerator.h \
    src/cpp/utilities/FrameToJsonRunnable.h \
    src/cpp/utilities/JsonToFrameRunnable.h \
    src/cpp/FramedSocketWorker.h \
    src/cpp/GuiInterface.h


unix:!android {
    LIBS += -L$$PWD/src/opus-dev-lib/lib/unix/x86_64/ -lopus
}
android {
    LIBS += -L$$PWD/src/opus-dev-lib/lib/armeabi-v7a/ -lopus
}

INCLUDEPATH += $$PWD/src/opus-dev-lib/include
DEPENDPATH += $$PWD/src/opus-dev-lib/include

unix:!android {
    PRE_TARGETDEPS += $$PWD/src/opus-dev-lib/lib/unix/x86_64/libopus.a
}
android {
    PRE_TARGETDEPS += $$PWD/src/opus-dev-lib/lib/armeabi-v7a/libopus.a
}
