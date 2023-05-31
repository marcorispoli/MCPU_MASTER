
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
    $${SHARED}/APPLICATION_INTERFACE/masterInterface.cpp \
    $${SHARED}/APPLOG/applog.cpp \
    $${SHARED}/WINDOWS/windows.cpp \
    $${TARGET_SOURCE}/main.cpp \
    $${TARGET_SOURCE}/WORKFLOWS/STARTUP/startup.cpp \
    $${TARGET_SOURCE}/DRIVERS/power_service.cpp  \
    $${TARGET_SOURCE}/DRIVERS/can_driver.cpp  \
    $${TARGET_SOURCE}/DRIVERS/compressor.cpp  \
    $${TARGET_SOURCE}/DRIVERS/collimator.cpp  \
    $${TARGET_SOURCE}/DRIVERS/filter.cpp  \
    $${TARGET_SOURCE}/DRIVERS/potter.cpp  \
    $${TARGET_SOURCE}/DRIVERS/motors.cpp  \
    $${TARGET_SOURCE}/DRIVERS/generator.cpp  \

HEADERS += \
    $${TARGET_SOURCE}/application.h \
    $${SHARED}/CONFIGFILE/configfile.h \
    $${SHARED}/CONFIGFILE/sysconfig.h \
    $${SHARED}/APPLICATION_INTERFACE/masterInterface.h \
    $${SHARED}/APPLOG/applog.h \
    $${SHARED}/WINDOWS/windows.h \
    $${TARGET_SOURCE}/CONFIGURATION/config.h \
    $${TARGET_SOURCE}/WORKFLOWS/STARTUP/startup.h \
    $${TARGET_SOURCE}/DRIVERS/power_service.h  \
    $${TARGET_SOURCE}/DRIVERS/can_driver.h  \
    $${TARGET_SOURCE}/DRIVERS/compressor.h  \
    $${TARGET_SOURCE}/DRIVERS/collimator.h  \
    $${TARGET_SOURCE}/DRIVERS/filter.h  \
    $${TARGET_SOURCE}/DRIVERS/potter.h  \
    $${TARGET_SOURCE}/DRIVERS/motors.h  \
    $${TARGET_SOURCE}/DRIVERS/generator.h  \


# Aggiunge tutti i path di progetto
INCLUDEPATH += \
    $${SHARED}/APPLICATION_INTERFACE \
    $${SHARED}/CAN_CLIENT \
    $${SHARED}/APPLOG \
    $${SHARED}/CONFIGFILE \    
    $${SHARED}/WINDOWS \
    $${TARGET_SOURCE} \
    $${TARGET_SOURCE}/WORKFLOWS/STARTUP \
    $${TARGET_SOURCE}/CONFIGURATION  \
    $${TARGET_SOURCE}/DRIVERS  \


