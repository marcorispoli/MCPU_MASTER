/********************************************************************************
** Form generated from reading UI file 'startup.ui'
**
** Created by: Qt User Interface Compiler version 6.2.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STARTUP_H
#define UI_STARTUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_startupWindow
{
public:
    QFrame *mainFrame;
    QLabel *window_title_label;
    QFrame *process_startup_frame;
    QFrame *power_service_frame;
    QLabel *label_3;
    QLabel *power_sevice_revision;
    QFrame *compressor_frame;
    QLabel *label_4;
    QLabel *compressor_revision;
    QFrame *collimator_frame;
    QLabel *label_5;
    QLabel *collimator_revision;
    QFrame *filter_frame;
    QLabel *label_6;
    QLabel *filter_revision;
    QFrame *potter_frame;
    QLabel *label_9;
    QLabel *potter_revision;
    QFrame *motors_frame;
    QLabel *motors_revision;
    QLabel *label_7;
    QFrame *generator_frame;
    QLabel *label_8;
    QLabel *generator_revision;
    QFrame *main_process_start_frame;
    QLabel *label_10;
    QFrame *can_process_frame;
    QLabel *label_11;
    QLabel *can_process_revision;
    QFrame *installed_package_frame;
    QLabel *package_revision;
    QLabel *package_label;
    QFrame *device_startup_frame;
    QFrame *powerservice_device_frame;
    QLabel *label_12;
    QLabel *powerservice_device_revision;
    QFrame *compressor_device_frame;
    QLabel *label_13;
    QLabel *compressor_device_revision;
    QFrame *collimator_device_frame;
    QLabel *label_14;
    QLabel *collimator_device_revision;
    QFrame *filter_device_frame;
    QLabel *label_15;
    QLabel *filter_device_revision;
    QFrame *potter_device_frame;
    QLabel *label_16;
    QLabel *potter_device_revision;
    QFrame *generator_device_frame;
    QLabel *generator_device_revision;
    QLabel *label_17;
    QFrame *main_device_start_frame;
    QLabel *label_19;
    QFrame *can_device_frame;
    QLabel *label_20;
    QLabel *can_device_converter_revision;
    QFrame *motors_startup_frame;
    QFrame *slide_motor_frame;
    QLabel *label_18;
    QLabel *slide_setup_label;
    QFrame *body_motor_frame;
    QLabel *label_21;
    QLabel *body_setup_label;
    QFrame *mvert_motor_frame;
    QLabel *label_22;
    QLabel *mvert_setup_label;
    QFrame *carm_motor_frame;
    QLabel *label_23;
    QLabel *carm_setup_label;
    QFrame *main_motorse_start_frame;
    QLabel *label_26;
    QFrame *trx_motor_frame;
    QLabel *label_27;
    QLabel *trx_setup_label;

    void setupUi(QWidget *startupWindow)
    {
        if (startupWindow->objectName().isEmpty())
            startupWindow->setObjectName(QString::fromUtf8("startupWindow"));
        startupWindow->resize(600, 1024);
        startupWindow->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(85, 170, 255);"));
        mainFrame = new QFrame(startupWindow);
        mainFrame->setObjectName(QString::fromUtf8("mainFrame"));
        mainFrame->setGeometry(QRect(0, 0, 600, 1024));
        QFont font;
        font.setPointSize(20);
        font.setBold(true);
        mainFrame->setFont(font);
        mainFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(34, 34, 34);\n"
"background-image: url(:/Windows/Windows/background.png);\n"
""));
        mainFrame->setFrameShape(QFrame::StyledPanel);
        mainFrame->setFrameShadow(QFrame::Raised);
        window_title_label = new QLabel(mainFrame);
        window_title_label->setObjectName(QString::fromUtf8("window_title_label"));
        window_title_label->setGeometry(QRect(40, 30, 521, 61));
        QFont font1;
        font1.setPointSize(30);
        font1.setBold(true);
        font1.setItalic(true);
        window_title_label->setFont(font1);
        window_title_label->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"background-color: rgba(0, 0, 0,0);\n"
"font: 1000 italic 30pt \"Arial\";\n"
""));
        process_startup_frame = new QFrame(mainFrame);
        process_startup_frame->setObjectName(QString::fromUtf8("process_startup_frame"));
        process_startup_frame->setGeometry(QRect(1000, 210, 521, 771));
        process_startup_frame->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"background-color: rgba(255, 255, 255, 0);\n"
"border-width: 1px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        process_startup_frame->setFrameShape(QFrame::StyledPanel);
        process_startup_frame->setFrameShadow(QFrame::Raised);
        power_service_frame = new QFrame(process_startup_frame);
        power_service_frame->setObjectName(QString::fromUtf8("power_service_frame"));
        power_service_frame->setGeometry(QRect(20, 200, 481, 71));
        power_service_frame->setFrameShape(QFrame::StyledPanel);
        power_service_frame->setFrameShadow(QFrame::Raised);
        label_3 = new QLabel(power_service_frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setEnabled(true);
        label_3->setGeometry(QRect(10, 20, 301, 41));
        QFont font2;
        font2.setPointSize(20);
        font2.setBold(true);
        font2.setItalic(false);
        label_3->setFont(font2);
        label_3->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_3->setText(QString::fromUtf8("MCPU POWER SERVICE"));
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        power_sevice_revision = new QLabel(power_service_frame);
        power_sevice_revision->setObjectName(QString::fromUtf8("power_sevice_revision"));
        power_sevice_revision->setGeometry(QRect(290, 20, 181, 31));
        power_sevice_revision->setFont(font2);
        power_sevice_revision->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        power_sevice_revision->setText(QString::fromUtf8("__.__  [__.__]"));
        power_sevice_revision->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        compressor_frame = new QFrame(process_startup_frame);
        compressor_frame->setObjectName(QString::fromUtf8("compressor_frame"));
        compressor_frame->setGeometry(QRect(20, 280, 481, 71));
        compressor_frame->setFrameShape(QFrame::StyledPanel);
        compressor_frame->setFrameShadow(QFrame::Raised);
        label_4 = new QLabel(compressor_frame);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 20, 281, 41));
        label_4->setFont(font2);
        label_4->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_4->setText(QString::fromUtf8("MCPU COMPRESSOR"));
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        compressor_revision = new QLabel(compressor_frame);
        compressor_revision->setObjectName(QString::fromUtf8("compressor_revision"));
        compressor_revision->setGeometry(QRect(290, 20, 181, 31));
        compressor_revision->setFont(font2);
        compressor_revision->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        compressor_revision->setText(QString::fromUtf8("__.__.__"));
        compressor_revision->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        collimator_frame = new QFrame(process_startup_frame);
        collimator_frame->setObjectName(QString::fromUtf8("collimator_frame"));
        collimator_frame->setGeometry(QRect(20, 360, 481, 71));
        collimator_frame->setFrameShape(QFrame::StyledPanel);
        collimator_frame->setFrameShadow(QFrame::Raised);
        label_5 = new QLabel(collimator_frame);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 20, 271, 41));
        label_5->setFont(font2);
        label_5->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_5->setText(QString::fromUtf8("MCPU COLLIMATOR"));
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        collimator_revision = new QLabel(collimator_frame);
        collimator_revision->setObjectName(QString::fromUtf8("collimator_revision"));
        collimator_revision->setGeometry(QRect(290, 20, 181, 31));
        collimator_revision->setFont(font2);
        collimator_revision->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        collimator_revision->setText(QString::fromUtf8("__.__.__"));
        collimator_revision->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        filter_frame = new QFrame(process_startup_frame);
        filter_frame->setObjectName(QString::fromUtf8("filter_frame"));
        filter_frame->setGeometry(QRect(20, 440, 481, 71));
        filter_frame->setFrameShape(QFrame::StyledPanel);
        filter_frame->setFrameShadow(QFrame::Raised);
        label_6 = new QLabel(filter_frame);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 20, 251, 41));
        label_6->setFont(font2);
        label_6->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_6->setText(QString::fromUtf8("MCPU FILTER"));
        label_6->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        filter_revision = new QLabel(filter_frame);
        filter_revision->setObjectName(QString::fromUtf8("filter_revision"));
        filter_revision->setGeometry(QRect(290, 20, 181, 31));
        filter_revision->setFont(font2);
        filter_revision->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        filter_revision->setText(QString::fromUtf8("__.__.__"));
        filter_revision->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        potter_frame = new QFrame(process_startup_frame);
        potter_frame->setObjectName(QString::fromUtf8("potter_frame"));
        potter_frame->setGeometry(QRect(20, 520, 481, 71));
        potter_frame->setFrameShape(QFrame::StyledPanel);
        potter_frame->setFrameShadow(QFrame::Raised);
        label_9 = new QLabel(potter_frame);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(10, 20, 251, 41));
        label_9->setFont(font2);
        label_9->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_9->setText(QString::fromUtf8("MCPU POTTER"));
        label_9->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        potter_revision = new QLabel(potter_frame);
        potter_revision->setObjectName(QString::fromUtf8("potter_revision"));
        potter_revision->setGeometry(QRect(290, 20, 181, 31));
        potter_revision->setFont(font2);
        potter_revision->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        potter_revision->setText(QString::fromUtf8("__.__.__"));
        potter_revision->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        motors_frame = new QFrame(process_startup_frame);
        motors_frame->setObjectName(QString::fromUtf8("motors_frame"));
        motors_frame->setGeometry(QRect(20, 600, 481, 71));
        motors_frame->setFrameShape(QFrame::StyledPanel);
        motors_frame->setFrameShadow(QFrame::Raised);
        motors_revision = new QLabel(motors_frame);
        motors_revision->setObjectName(QString::fromUtf8("motors_revision"));
        motors_revision->setGeometry(QRect(290, 20, 181, 31));
        motors_revision->setFont(font2);
        motors_revision->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        motors_revision->setText(QString::fromUtf8("__.__.__"));
        motors_revision->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_7 = new QLabel(motors_frame);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 10, 251, 61));
        label_7->setFont(font2);
        label_7->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_7->setText(QString::fromUtf8("MCPU MOTORS"));
        label_7->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        generator_frame = new QFrame(process_startup_frame);
        generator_frame->setObjectName(QString::fromUtf8("generator_frame"));
        generator_frame->setGeometry(QRect(20, 680, 481, 71));
        generator_frame->setFrameShape(QFrame::StyledPanel);
        generator_frame->setFrameShadow(QFrame::Raised);
        label_8 = new QLabel(generator_frame);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 20, 251, 41));
        label_8->setFont(font2);
        label_8->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_8->setText(QString::fromUtf8("MCPU GENERATOR"));
        label_8->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        generator_revision = new QLabel(generator_frame);
        generator_revision->setObjectName(QString::fromUtf8("generator_revision"));
        generator_revision->setGeometry(QRect(290, 20, 181, 31));
        generator_revision->setFont(font2);
        generator_revision->setStyleSheet(QString::fromUtf8("\n"
"border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        generator_revision->setText(QString::fromUtf8("__.__.__"));
        generator_revision->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        main_process_start_frame = new QFrame(process_startup_frame);
        main_process_start_frame->setObjectName(QString::fromUtf8("main_process_start_frame"));
        main_process_start_frame->setGeometry(QRect(20, 20, 481, 80));
        main_process_start_frame->setStyleSheet(QString::fromUtf8("font: 500 18pt \"Roboto\";"));
        main_process_start_frame->setFrameShape(QFrame::StyledPanel);
        main_process_start_frame->setFrameShadow(QFrame::Raised);
        label_10 = new QLabel(main_process_start_frame);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setEnabled(true);
        label_10->setGeometry(QRect(10, 20, 451, 41));
        QFont font3;
        font3.setPointSize(18);
        font3.setBold(false);
        font3.setItalic(false);
        label_10->setFont(font3);
        label_10->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_10->setText(QString::fromUtf8("MAIN CPU PROCESS ACTIVATION"));
        label_10->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        can_process_frame = new QFrame(process_startup_frame);
        can_process_frame->setObjectName(QString::fromUtf8("can_process_frame"));
        can_process_frame->setGeometry(QRect(20, 120, 481, 71));
        can_process_frame->setFrameShape(QFrame::StyledPanel);
        can_process_frame->setFrameShadow(QFrame::Raised);
        label_11 = new QLabel(can_process_frame);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setEnabled(true);
        label_11->setGeometry(QRect(10, 20, 251, 41));
        label_11->setFont(font2);
        label_11->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_11->setText(QString::fromUtf8("MCPU CAN DRIVER"));
        label_11->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        can_process_revision = new QLabel(can_process_frame);
        can_process_revision->setObjectName(QString::fromUtf8("can_process_revision"));
        can_process_revision->setGeometry(QRect(290, 20, 181, 31));
        can_process_revision->setFont(font2);
        can_process_revision->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        can_process_revision->setText(QString::fromUtf8("__.__  [__.__]"));
        can_process_revision->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        installed_package_frame = new QFrame(mainFrame);
        installed_package_frame->setObjectName(QString::fromUtf8("installed_package_frame"));
        installed_package_frame->setGeometry(QRect(40, 100, 521, 61));
        installed_package_frame->setStyleSheet(QString::fromUtf8("background-image: url(\"\");\n"
"border-image: url(\"\");\n"
"border-color:rgba(0,0,0,0);\n"
"background-color: rgba(0, 0, 0,0);"));
        installed_package_frame->setFrameShape(QFrame::StyledPanel);
        installed_package_frame->setFrameShadow(QFrame::Raised);
        package_revision = new QLabel(installed_package_frame);
        package_revision->setObjectName(QString::fromUtf8("package_revision"));
        package_revision->setGeometry(QRect(370, 10, 131, 41));
        QFont font4;
        font4.setPointSize(20);
        font4.setBold(true);
        font4.setItalic(true);
        package_revision->setFont(font4);
        package_revision->setStyleSheet(QString::fromUtf8(""));
        package_revision->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        package_label = new QLabel(installed_package_frame);
        package_label->setObjectName(QString::fromUtf8("package_label"));
        package_label->setGeometry(QRect(10, 10, 251, 41));
        package_label->setFont(font4);
        package_label->setStyleSheet(QString::fromUtf8(""));
        device_startup_frame = new QFrame(mainFrame);
        device_startup_frame->setObjectName(QString::fromUtf8("device_startup_frame"));
        device_startup_frame->setGeometry(QRect(1000, 210, 521, 771));
        device_startup_frame->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"background-color: rgba(255, 255, 255, 0);\n"
"border-width: 1px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        device_startup_frame->setFrameShape(QFrame::StyledPanel);
        device_startup_frame->setFrameShadow(QFrame::Raised);
        powerservice_device_frame = new QFrame(device_startup_frame);
        powerservice_device_frame->setObjectName(QString::fromUtf8("powerservice_device_frame"));
        powerservice_device_frame->setGeometry(QRect(20, 200, 481, 71));
        powerservice_device_frame->setFrameShape(QFrame::StyledPanel);
        powerservice_device_frame->setFrameShadow(QFrame::Raised);
        label_12 = new QLabel(powerservice_device_frame);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setEnabled(true);
        label_12->setGeometry(QRect(10, 20, 221, 41));
        label_12->setFont(font2);
        label_12->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_12->setText(QString::fromUtf8("SERVICE BOARD"));
        label_12->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        powerservice_device_revision = new QLabel(powerservice_device_frame);
        powerservice_device_revision->setObjectName(QString::fromUtf8("powerservice_device_revision"));
        powerservice_device_revision->setGeometry(QRect(230, 20, 241, 41));
        powerservice_device_revision->setFont(font2);
        powerservice_device_revision->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        powerservice_device_revision->setText(QString::fromUtf8("100.100 [100.100]"));
        powerservice_device_revision->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        compressor_device_frame = new QFrame(device_startup_frame);
        compressor_device_frame->setObjectName(QString::fromUtf8("compressor_device_frame"));
        compressor_device_frame->setGeometry(QRect(20, 280, 481, 71));
        compressor_device_frame->setFrameShape(QFrame::StyledPanel);
        compressor_device_frame->setFrameShadow(QFrame::Raised);
        label_13 = new QLabel(compressor_device_frame);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(10, 20, 281, 41));
        label_13->setFont(font2);
        label_13->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_13->setText(QString::fromUtf8("COMPRESSOR DEVICE"));
        label_13->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        compressor_device_revision = new QLabel(compressor_device_frame);
        compressor_device_revision->setObjectName(QString::fromUtf8("compressor_device_revision"));
        compressor_device_revision->setGeometry(QRect(290, 20, 181, 31));
        compressor_device_revision->setFont(font2);
        compressor_device_revision->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        compressor_device_revision->setText(QString::fromUtf8("__.__.__"));
        compressor_device_revision->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        collimator_device_frame = new QFrame(device_startup_frame);
        collimator_device_frame->setObjectName(QString::fromUtf8("collimator_device_frame"));
        collimator_device_frame->setGeometry(QRect(20, 360, 481, 71));
        collimator_device_frame->setFrameShape(QFrame::StyledPanel);
        collimator_device_frame->setFrameShadow(QFrame::Raised);
        label_14 = new QLabel(collimator_device_frame);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(10, 20, 271, 41));
        label_14->setFont(font2);
        label_14->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_14->setText(QString::fromUtf8("COLLIMATOR DEVICE"));
        label_14->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        collimator_device_revision = new QLabel(collimator_device_frame);
        collimator_device_revision->setObjectName(QString::fromUtf8("collimator_device_revision"));
        collimator_device_revision->setGeometry(QRect(290, 20, 181, 31));
        collimator_device_revision->setFont(font2);
        collimator_device_revision->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        collimator_device_revision->setText(QString::fromUtf8("__.__.__"));
        collimator_device_revision->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        filter_device_frame = new QFrame(device_startup_frame);
        filter_device_frame->setObjectName(QString::fromUtf8("filter_device_frame"));
        filter_device_frame->setGeometry(QRect(20, 440, 481, 71));
        filter_device_frame->setFrameShape(QFrame::StyledPanel);
        filter_device_frame->setFrameShadow(QFrame::Raised);
        label_15 = new QLabel(filter_device_frame);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(10, 20, 251, 41));
        label_15->setFont(font2);
        label_15->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_15->setText(QString::fromUtf8("FILTER DEVICE"));
        label_15->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        filter_device_revision = new QLabel(filter_device_frame);
        filter_device_revision->setObjectName(QString::fromUtf8("filter_device_revision"));
        filter_device_revision->setGeometry(QRect(290, 20, 181, 31));
        filter_device_revision->setFont(font2);
        filter_device_revision->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        filter_device_revision->setText(QString::fromUtf8("__.__.__"));
        filter_device_revision->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        potter_device_frame = new QFrame(device_startup_frame);
        potter_device_frame->setObjectName(QString::fromUtf8("potter_device_frame"));
        potter_device_frame->setGeometry(QRect(20, 520, 481, 71));
        potter_device_frame->setFrameShape(QFrame::StyledPanel);
        potter_device_frame->setFrameShadow(QFrame::Raised);
        label_16 = new QLabel(potter_device_frame);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(10, 20, 251, 41));
        label_16->setFont(font2);
        label_16->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_16->setText(QString::fromUtf8("POTTER DEVICE"));
        label_16->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        potter_device_revision = new QLabel(potter_device_frame);
        potter_device_revision->setObjectName(QString::fromUtf8("potter_device_revision"));
        potter_device_revision->setGeometry(QRect(290, 20, 181, 31));
        potter_device_revision->setFont(font2);
        potter_device_revision->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        potter_device_revision->setText(QString::fromUtf8("__.__.__"));
        potter_device_revision->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        generator_device_frame = new QFrame(device_startup_frame);
        generator_device_frame->setObjectName(QString::fromUtf8("generator_device_frame"));
        generator_device_frame->setGeometry(QRect(20, 600, 481, 71));
        generator_device_frame->setFrameShape(QFrame::StyledPanel);
        generator_device_frame->setFrameShadow(QFrame::Raised);
        generator_device_revision = new QLabel(generator_device_frame);
        generator_device_revision->setObjectName(QString::fromUtf8("generator_device_revision"));
        generator_device_revision->setGeometry(QRect(250, 20, 221, 31));
        generator_device_revision->setFont(font2);
        generator_device_revision->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        generator_device_revision->setText(QString::fromUtf8("__.__.__"));
        generator_device_revision->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_17 = new QLabel(generator_device_frame);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(10, 10, 251, 61));
        label_17->setFont(font2);
        label_17->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_17->setText(QString::fromUtf8("GENERATOR"));
        label_17->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        main_device_start_frame = new QFrame(device_startup_frame);
        main_device_start_frame->setObjectName(QString::fromUtf8("main_device_start_frame"));
        main_device_start_frame->setGeometry(QRect(20, 20, 481, 80));
        main_device_start_frame->setStyleSheet(QString::fromUtf8("font: 500 18pt \"Roboto\";"));
        main_device_start_frame->setFrameShape(QFrame::StyledPanel);
        main_device_start_frame->setFrameShadow(QFrame::Raised);
        label_19 = new QLabel(main_device_start_frame);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setEnabled(true);
        label_19->setGeometry(QRect(10, 20, 451, 41));
        label_19->setFont(font3);
        label_19->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_19->setText(QString::fromUtf8("DEVICE ACTIVATION"));
        label_19->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        can_device_frame = new QFrame(device_startup_frame);
        can_device_frame->setObjectName(QString::fromUtf8("can_device_frame"));
        can_device_frame->setGeometry(QRect(20, 120, 481, 71));
        can_device_frame->setFrameShape(QFrame::StyledPanel);
        can_device_frame->setFrameShadow(QFrame::Raised);
        label_20 = new QLabel(can_device_frame);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setEnabled(true);
        label_20->setGeometry(QRect(10, 20, 281, 41));
        label_20->setFont(font2);
        label_20->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_20->setText(QString::fromUtf8("CAN CONVERTER"));
        label_20->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        can_device_converter_revision = new QLabel(can_device_frame);
        can_device_converter_revision->setObjectName(QString::fromUtf8("can_device_converter_revision"));
        can_device_converter_revision->setGeometry(QRect(310, 20, 161, 41));
        can_device_converter_revision->setFont(font2);
        can_device_converter_revision->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        can_device_converter_revision->setText(QString::fromUtf8("API: 100.100.100"));
        can_device_converter_revision->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        motors_startup_frame = new QFrame(mainFrame);
        motors_startup_frame->setObjectName(QString::fromUtf8("motors_startup_frame"));
        motors_startup_frame->setGeometry(QRect(40, 210, 551, 771));
        motors_startup_frame->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"background-color: rgba(255, 255, 255, 0);\n"
"border-width: 1px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        motors_startup_frame->setFrameShape(QFrame::StyledPanel);
        motors_startup_frame->setFrameShadow(QFrame::Raised);
        slide_motor_frame = new QFrame(motors_startup_frame);
        slide_motor_frame->setObjectName(QString::fromUtf8("slide_motor_frame"));
        slide_motor_frame->setGeometry(QRect(20, 200, 481, 71));
        slide_motor_frame->setFrameShape(QFrame::StyledPanel);
        slide_motor_frame->setFrameShadow(QFrame::Raised);
        label_18 = new QLabel(slide_motor_frame);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setEnabled(true);
        label_18->setGeometry(QRect(10, 20, 221, 41));
        label_18->setFont(font2);
        label_18->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_18->setText(QString::fromUtf8("SLIDE Setup"));
        label_18->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        slide_setup_label = new QLabel(slide_motor_frame);
        slide_setup_label->setObjectName(QString::fromUtf8("slide_setup_label"));
        slide_setup_label->setGeometry(QRect(230, 20, 241, 41));
        slide_setup_label->setFont(font2);
        slide_setup_label->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        slide_setup_label->setText(QString::fromUtf8("-"));
        slide_setup_label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        body_motor_frame = new QFrame(motors_startup_frame);
        body_motor_frame->setObjectName(QString::fromUtf8("body_motor_frame"));
        body_motor_frame->setGeometry(QRect(20, 280, 481, 71));
        body_motor_frame->setFrameShape(QFrame::StyledPanel);
        body_motor_frame->setFrameShadow(QFrame::Raised);
        label_21 = new QLabel(body_motor_frame);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(10, 20, 281, 41));
        label_21->setFont(font2);
        label_21->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_21->setText(QString::fromUtf8("BODY Setup"));
        label_21->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        body_setup_label = new QLabel(body_motor_frame);
        body_setup_label->setObjectName(QString::fromUtf8("body_setup_label"));
        body_setup_label->setGeometry(QRect(290, 20, 181, 31));
        body_setup_label->setFont(font2);
        body_setup_label->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        body_setup_label->setText(QString::fromUtf8("-"));
        body_setup_label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        mvert_motor_frame = new QFrame(motors_startup_frame);
        mvert_motor_frame->setObjectName(QString::fromUtf8("mvert_motor_frame"));
        mvert_motor_frame->setGeometry(QRect(20, 360, 481, 71));
        mvert_motor_frame->setFrameShape(QFrame::StyledPanel);
        mvert_motor_frame->setFrameShadow(QFrame::Raised);
        label_22 = new QLabel(mvert_motor_frame);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(10, 20, 271, 41));
        label_22->setFont(font2);
        label_22->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_22->setText(QString::fromUtf8("VERTICAL Setup"));
        label_22->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        mvert_setup_label = new QLabel(mvert_motor_frame);
        mvert_setup_label->setObjectName(QString::fromUtf8("mvert_setup_label"));
        mvert_setup_label->setGeometry(QRect(290, 20, 181, 31));
        mvert_setup_label->setFont(font2);
        mvert_setup_label->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        mvert_setup_label->setText(QString::fromUtf8("-"));
        mvert_setup_label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        carm_motor_frame = new QFrame(motors_startup_frame);
        carm_motor_frame->setObjectName(QString::fromUtf8("carm_motor_frame"));
        carm_motor_frame->setGeometry(QRect(20, 440, 481, 71));
        carm_motor_frame->setFrameShape(QFrame::StyledPanel);
        carm_motor_frame->setFrameShadow(QFrame::Raised);
        label_23 = new QLabel(carm_motor_frame);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(10, 20, 251, 41));
        label_23->setFont(font2);
        label_23->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_23->setText(QString::fromUtf8("ARM Setup"));
        label_23->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        carm_setup_label = new QLabel(carm_motor_frame);
        carm_setup_label->setObjectName(QString::fromUtf8("carm_setup_label"));
        carm_setup_label->setGeometry(QRect(290, 20, 181, 31));
        carm_setup_label->setFont(font2);
        carm_setup_label->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        carm_setup_label->setText(QString::fromUtf8("-"));
        carm_setup_label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        main_motorse_start_frame = new QFrame(motors_startup_frame);
        main_motorse_start_frame->setObjectName(QString::fromUtf8("main_motorse_start_frame"));
        main_motorse_start_frame->setGeometry(QRect(20, 20, 481, 80));
        main_motorse_start_frame->setStyleSheet(QString::fromUtf8("font: 500 18pt \"Roboto\";"));
        main_motorse_start_frame->setFrameShape(QFrame::StyledPanel);
        main_motorse_start_frame->setFrameShadow(QFrame::Raised);
        label_26 = new QLabel(main_motorse_start_frame);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setEnabled(true);
        label_26->setGeometry(QRect(10, 20, 451, 41));
        label_26->setFont(font3);
        label_26->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_26->setText(QString::fromUtf8("MOTORS INITIALIZATION"));
        label_26->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        trx_motor_frame = new QFrame(motors_startup_frame);
        trx_motor_frame->setObjectName(QString::fromUtf8("trx_motor_frame"));
        trx_motor_frame->setGeometry(QRect(20, 120, 481, 71));
        trx_motor_frame->setFrameShape(QFrame::StyledPanel);
        trx_motor_frame->setFrameShadow(QFrame::Raised);
        label_27 = new QLabel(trx_motor_frame);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setEnabled(true);
        label_27->setGeometry(QRect(10, 20, 281, 41));
        label_27->setFont(font2);
        label_27->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_27->setText(QString::fromUtf8("TRX Setup"));
        label_27->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        trx_setup_label = new QLabel(trx_motor_frame);
        trx_setup_label->setObjectName(QString::fromUtf8("trx_setup_label"));
        trx_setup_label->setGeometry(QRect(310, 20, 161, 41));
        trx_setup_label->setFont(font2);
        trx_setup_label->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        trx_setup_label->setText(QString::fromUtf8("-"));
        trx_setup_label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        retranslateUi(startupWindow);

        QMetaObject::connectSlotsByName(startupWindow);
    } // setupUi

    void retranslateUi(QWidget *startupWindow)
    {
        startupWindow->setWindowTitle(QString());
        window_title_label->setText(QCoreApplication::translate("startupWindow", "System Startup", nullptr));
        package_revision->setText(QCoreApplication::translate("startupWindow", "ID01", nullptr));
        package_label->setText(QCoreApplication::translate("startupWindow", "Installed Package:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class startupWindow: public Ui_startupWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTUP_H
