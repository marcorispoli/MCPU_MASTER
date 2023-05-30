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
    QLabel *startup_label;
    QFrame *process_start;
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
    QLabel *package_label;
    QLabel *package_revision;

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
        startup_label = new QLabel(mainFrame);
        startup_label->setObjectName(QString::fromUtf8("startup_label"));
        startup_label->setGeometry(QRect(40, 30, 511, 61));
        QFont font1;
        font1.setPointSize(30);
        font1.setBold(true);
        font1.setItalic(true);
        startup_label->setFont(font1);
        startup_label->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"background-color: rgba(255, 255, 255, 0);\n"
"color: rgb(239, 190, 189);\n"
"font: 700 italic 30pt \"Arial\";\n"
"border-bottom-width: 1px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
""));
        process_start = new QFrame(mainFrame);
        process_start->setObjectName(QString::fromUtf8("process_start"));
        process_start->setGeometry(QRect(40, 210, 521, 771));
        process_start->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"background-color: rgba(255, 255, 255, 0);\n"
"border-width: 1px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        process_start->setFrameShape(QFrame::StyledPanel);
        process_start->setFrameShadow(QFrame::Raised);
        power_service_frame = new QFrame(process_start);
        power_service_frame->setObjectName(QString::fromUtf8("power_service_frame"));
        power_service_frame->setGeometry(QRect(20, 200, 481, 71));
        power_service_frame->setFrameShape(QFrame::StyledPanel);
        power_service_frame->setFrameShadow(QFrame::Raised);
        label_3 = new QLabel(power_service_frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setEnabled(true);
        label_3->setGeometry(QRect(10, 20, 251, 41));
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
        label_3->setText(QString::fromUtf8("Power Service:"));
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
        compressor_frame = new QFrame(process_start);
        compressor_frame->setObjectName(QString::fromUtf8("compressor_frame"));
        compressor_frame->setGeometry(QRect(20, 280, 481, 71));
        compressor_frame->setFrameShape(QFrame::StyledPanel);
        compressor_frame->setFrameShadow(QFrame::Raised);
        label_4 = new QLabel(compressor_frame);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 20, 251, 41));
        label_4->setFont(font2);
        label_4->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_4->setText(QString::fromUtf8("Compressor:"));
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
        collimator_frame = new QFrame(process_start);
        collimator_frame->setObjectName(QString::fromUtf8("collimator_frame"));
        collimator_frame->setGeometry(QRect(20, 360, 481, 71));
        collimator_frame->setFrameShape(QFrame::StyledPanel);
        collimator_frame->setFrameShadow(QFrame::Raised);
        label_5 = new QLabel(collimator_frame);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 20, 251, 41));
        label_5->setFont(font2);
        label_5->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_5->setText(QString::fromUtf8("Collimator:"));
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
        filter_frame = new QFrame(process_start);
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
        label_6->setText(QString::fromUtf8("Filter:"));
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
        potter_frame = new QFrame(process_start);
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
        label_9->setText(QString::fromUtf8("Potter:"));
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
        motors_frame = new QFrame(process_start);
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
        label_7->setText(QString::fromUtf8("Motors:"));
        label_7->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        generator_frame = new QFrame(process_start);
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
        label_8->setText(QString::fromUtf8("Generator:"));
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
        main_process_start_frame = new QFrame(process_start);
        main_process_start_frame->setObjectName(QString::fromUtf8("main_process_start_frame"));
        main_process_start_frame->setGeometry(QRect(20, 20, 481, 80));
        main_process_start_frame->setFrameShape(QFrame::StyledPanel);
        main_process_start_frame->setFrameShadow(QFrame::Raised);
        label_10 = new QLabel(main_process_start_frame);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setEnabled(true);
        label_10->setGeometry(QRect(10, 20, 451, 41));
        label_10->setFont(font2);
        label_10->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_10->setText(QString::fromUtf8("MAIN CPU PROCESS ACTIVATION"));
        label_10->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        can_process_frame = new QFrame(process_start);
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
        label_11->setText(QString::fromUtf8("Can Process:"));
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
        package_label = new QLabel(mainFrame);
        package_label->setObjectName(QString::fromUtf8("package_label"));
        package_label->setGeometry(QRect(60, 100, 251, 41));
        QFont font3;
        font3.setPointSize(20);
        font3.setBold(true);
        font3.setItalic(true);
        package_label->setFont(font3);
        package_label->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"background-color: rgba(255, 255, 255, 0);\n"
"color: white;\n"
"font: 700 italic 20pt \"Arial\";\n"
"border-bottom-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
""));
        package_revision = new QLabel(mainFrame);
        package_revision->setObjectName(QString::fromUtf8("package_revision"));
        package_revision->setGeometry(QRect(420, 100, 91, 41));
        package_revision->setFont(font3);
        package_revision->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"background-color: rgba(255, 255, 255, 0);\n"
"color: white;\n"
"font: 700 italic 20pt \"Arial\";\n"
"border-bottom-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
""));

        retranslateUi(startupWindow);

        QMetaObject::connectSlotsByName(startupWindow);
    } // setupUi

    void retranslateUi(QWidget *startupWindow)
    {
        startupWindow->setWindowTitle(QString());
        startup_label->setText(QCoreApplication::translate("startupWindow", "System Startup", nullptr));
        package_label->setText(QCoreApplication::translate("startupWindow", "Installed Package:", nullptr));
        package_revision->setText(QCoreApplication::translate("startupWindow", "ID01", nullptr));
    } // retranslateUi

};

namespace Ui {
    class startupWindow: public Ui_startupWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTUP_H
