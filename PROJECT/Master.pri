
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#Include supporto per Qt Network (moduli TCP/IP)
QT += network

FORMS += \
    $${TARGET_SOURCE}/WORKFLOWS/STARTUP/startup.ui \

RESOURCES += \
    $${TARGET_RESOURCE}/assets.qrc


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

SOURCES += \
    $${SHARED}/CONFIGFILE/configfile.cpp \
    $${SHARED}/APPLOG/applog.cpp \
    $${TARGET_SOURCE}/main.cpp \
    $${TARGET_SOURCE}/WORKFLOWS/STARTUP/startup.cpp \


HEADERS += \
    $${TARGET_SOURCE}/application.h \
    $${SHARED}/CONFIGFILE/configfile.h \
    $${SHARED}/APPLOG/applog.h \
    $${TARGET_SOURCE}/CONFIGURATION/config.h \
    $${TARGET_SOURCE}/WORKFLOWS/STARTUP/startup.h \



# Aggiunge tutti i path di progetto
INCLUDEPATH += \
    $${SHARED}/APPLICATION_INTERFACE \
    $${SHARED}/CAN_CLIENT \
    $${SHARED}/APPLOG \
    $${SHARED}/CONFIGFILE \
    $${SHARED}/CAN_DEVICE_PROTOCOL \
    $${TARGET_SOURCE} \
    $${TARGET_SOURCE}/WORKFLOWS/STARTUP \
    $${TARGET_SOURCE}/CONFIGURATION  \


