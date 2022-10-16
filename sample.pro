QT -= gui
QT += network
CONFIG += c++11 console
CONFIG -= app_bundle
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        net2.cpp
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


HEADERS += \
    net2.h
INCLUDEPATH += D:/OpenSSL-Win64/OpenSSL-Win64/OpenSSL-Win64
DISTFILES += \
    ../build-sample-Desktop_Qt_6_1_2_MSVC2019_64bit-Debug/debug/main.obj
