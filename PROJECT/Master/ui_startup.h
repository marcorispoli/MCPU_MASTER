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
    QLabel *main_process_start_frame;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *power_sevice_revision;
    QLabel *compressor_revision;
    QLabel *collimator_revision;
    QLabel *filter_revision;
    QLabel *motors_revision;
    QLabel *generator_revision;
    QLabel *potter_revision;
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
        main_process_start_frame = new QLabel(process_start);
        main_process_start_frame->setObjectName(QString::fromUtf8("main_process_start_frame"));
        main_process_start_frame->setGeometry(QRect(20, 20, 481, 61));
        QFont font2;
        font2.setPointSize(20);
        font2.setBold(true);
        font2.setItalic(false);
        main_process_start_frame->setFont(font2);
        main_process_start_frame->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"background-color: rgb(239, 190, 189);\n"
"\n"
"color: black;\n"
"font: 700 normal 20pt \"Arial\";\n"
"border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        main_process_start_frame->setAlignment(Qt::AlignCenter);
        label_3 = new QLabel(process_start);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 110, 251, 61));
        label_3->setFont(font2);
        label_3->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"background-color: rgba(239, 190, 189,0);\n"
"\n"
"color: white;\n"
"font: 700 normal 20pt \"Arial\";\n"
"border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_3->setText(QString::fromUtf8("Power Service:"));
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_4 = new QLabel(process_start);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 180, 251, 61));
        label_4->setFont(font2);
        label_4->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"background-color: rgba(239, 190, 189,0);\n"
"\n"
"color: white;\n"
"font: 700 normal 20pt \"Arial\";\n"
"border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_4->setText(QString::fromUtf8("Compressor:"));
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_5 = new QLabel(process_start);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 250, 251, 61));
        label_5->setFont(font2);
        label_5->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"background-color: rgba(239, 190, 189,0);\n"
"\n"
"color: white;\n"
"font: 700 normal 20pt \"Arial\";\n"
"border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_5->setText(QString::fromUtf8("Collimator:"));
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_6 = new QLabel(process_start);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(20, 320, 251, 61));
        label_6->setFont(font2);
        label_6->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"background-color: rgba(239, 190, 189,0);\n"
"\n"
"color: white;\n"
"font: 700 normal 20pt \"Arial\";\n"
"border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_6->setText(QString::fromUtf8("Filter:"));
        label_6->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_7 = new QLabel(process_start);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(20, 390, 251, 61));
        label_7->setFont(font2);
        label_7->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"background-color: rgba(239, 190, 189,0);\n"
"\n"
"color: white;\n"
"font: 700 normal 20pt \"Arial\";\n"
"border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_7->setText(QString::fromUtf8("Motors:"));
        label_7->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_8 = new QLabel(process_start);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(20, 460, 251, 61));
        label_8->setFont(font2);
        label_8->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"background-color: rgba(239, 190, 189,0);\n"
"\n"
"color: white;\n"
"font: 700 normal 20pt \"Arial\";\n"
"border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_8->setText(QString::fromUtf8("Generator:"));
        label_8->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_9 = new QLabel(process_start);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(20, 530, 251, 61));
        label_9->setFont(font2);
        label_9->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"background-color: rgba(239, 190, 189,0);\n"
"\n"
"color: white;\n"
"font: 700 normal 20pt \"Arial\";\n"
"border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        label_9->setText(QString::fromUtf8("Potter:"));
        label_9->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        power_sevice_revision = new QLabel(process_start);
        power_sevice_revision->setObjectName(QString::fromUtf8("power_sevice_revision"));
        power_sevice_revision->setGeometry(QRect(280, 120, 121, 41));
        power_sevice_revision->setFont(font2);
        power_sevice_revision->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"background-color: rgba(239, 190, 189,0);\n"
"\n"
"color: white;\n"
"font: 700 normal 20pt \"Arial\";\n"
"border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        power_sevice_revision->setText(QString::fromUtf8("__.__.__"));
        power_sevice_revision->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        compressor_revision = new QLabel(process_start);
        compressor_revision->setObjectName(QString::fromUtf8("compressor_revision"));
        compressor_revision->setGeometry(QRect(280, 190, 121, 41));
        compressor_revision->setFont(font2);
        compressor_revision->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"background-color: rgba(239, 190, 189,0);\n"
"\n"
"color: white;\n"
"font: 700 normal 20pt \"Arial\";\n"
"border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        compressor_revision->setText(QString::fromUtf8("__.__.__"));
        compressor_revision->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        collimator_revision = new QLabel(process_start);
        collimator_revision->setObjectName(QString::fromUtf8("collimator_revision"));
        collimator_revision->setGeometry(QRect(280, 260, 121, 41));
        collimator_revision->setFont(font2);
        collimator_revision->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"background-color: rgba(239, 190, 189,0);\n"
"\n"
"color: white;\n"
"font: 700 normal 20pt \"Arial\";\n"
"border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        collimator_revision->setText(QString::fromUtf8("__.__.__"));
        collimator_revision->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        filter_revision = new QLabel(process_start);
        filter_revision->setObjectName(QString::fromUtf8("filter_revision"));
        filter_revision->setGeometry(QRect(280, 330, 121, 41));
        filter_revision->setFont(font2);
        filter_revision->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"background-color: rgba(239, 190, 189,0);\n"
"\n"
"color: white;\n"
"font: 700 normal 20pt \"Arial\";\n"
"border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        filter_revision->setText(QString::fromUtf8("__.__.__"));
        filter_revision->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        motors_revision = new QLabel(process_start);
        motors_revision->setObjectName(QString::fromUtf8("motors_revision"));
        motors_revision->setGeometry(QRect(280, 400, 121, 41));
        motors_revision->setFont(font2);
        motors_revision->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"background-color: rgba(239, 190, 189,0);\n"
"\n"
"color: white;\n"
"font: 700 normal 20pt \"Arial\";\n"
"border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        motors_revision->setText(QString::fromUtf8("__.__.__"));
        motors_revision->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        generator_revision = new QLabel(process_start);
        generator_revision->setObjectName(QString::fromUtf8("generator_revision"));
        generator_revision->setGeometry(QRect(280, 470, 121, 41));
        generator_revision->setFont(font2);
        generator_revision->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"background-color: rgba(239, 190, 189,0);\n"
"\n"
"color: white;\n"
"font: 700 normal 20pt \"Arial\";\n"
"border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        generator_revision->setText(QString::fromUtf8("__.__.__"));
        generator_revision->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        potter_revision = new QLabel(process_start);
        potter_revision->setObjectName(QString::fromUtf8("potter_revision"));
        potter_revision->setGeometry(QRect(280, 540, 121, 41));
        potter_revision->setFont(font2);
        potter_revision->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"background-color: rgba(239, 190, 189,0);\n"
"\n"
"color: white;\n"
"font: 700 normal 20pt \"Arial\";\n"
"border-width: 0px;\n"
"border-style: solid;\n"
"border-color: rgb(239, 190, 189);\n"
"\n"
""));
        potter_revision->setText(QString::fromUtf8("__.__.__"));
        potter_revision->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
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
        startupWindow->setWindowTitle(QCoreApplication::translate("startupWindow", "GENERATOR DEBUG WINDOW", nullptr));
        startup_label->setText(QCoreApplication::translate("startupWindow", "System Startup", nullptr));
        main_process_start_frame->setText(QCoreApplication::translate("startupWindow", "MAINCPU PROCESS ACTIVATION", nullptr));
        package_label->setText(QCoreApplication::translate("startupWindow", "Installed Package:", nullptr));
        package_revision->setText(QCoreApplication::translate("startupWindow", "ID01", nullptr));
    } // retranslateUi

};

namespace Ui {
    class startupWindow: public Ui_startupWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTUP_H
