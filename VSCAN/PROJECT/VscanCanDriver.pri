
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#Include supporto per Qt Network (moduli TCP/IP)
QT += network

FORMS += \
    $${TARGET_SOURCE}/WINDOW/window.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


LIBS += -L$${TARGET_SOURCE}/DLL/ -lvs_can_api

DISTFILES +=

SOURCES += \
    $${TARGET_SOURCE}/main.cpp \
    $${PROJLIB}/APPLOG/applog.cpp \
    $${TARGET_SOURCE}/SERVER/server.cpp \
    $${TARGET_SOURCE}/CAN/can_driver.cpp \
    $${TARGET_SOURCE}/WINDOW/window.cpp \



HEADERS += \
    $${TARGET_SOURCE}/application.h \
    $${PROJLIB}/APPLOG/applog.h \
    $${TARGET_SOURCE}/SERVER/server.h \
    $${TARGET_SOURCE}/CAN/can_driver.h \
    $${TARGET_SOURCE}/WINDOW/window.h \
    $${TARGET_SOURCE}/DLL/vs_can_api.h \


# Aggiunge tutti i path di progetto
INCLUDEPATH += \    
    $${TARGET_SOURCE}/DLL \
    $${TARGET_SOURCE}/SERVER \
    $${TARGET_SOURCE}/CAN \
    $${TARGET_SOURCE}/WINDOW \
    $${PROJLIB}/APPLOG \
    $${TARGET_SOURCE} \


DEPENDPATH += \
    $${TARGET_SOURCE}/DLL \



