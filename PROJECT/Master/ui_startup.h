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
    QLabel *label;

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
""));
        mainFrame->setFrameShape(QFrame::StyledPanel);
        mainFrame->setFrameShadow(QFrame::Raised);
        label = new QLabel(mainFrame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(140, 30, 341, 61));
        QFont font1;
        font1.setPointSize(30);
        font1.setBold(true);
        font1.setItalic(true);
        label->setFont(font1);
        label->setStyleSheet(QString::fromUtf8("color: rgb(246, 201, 255);\n"
"font: 700 italic 30pt \"Arial\";"));

        retranslateUi(startupWindow);

        QMetaObject::connectSlotsByName(startupWindow);
    } // setupUi

    void retranslateUi(QWidget *startupWindow)
    {
        startupWindow->setWindowTitle(QCoreApplication::translate("startupWindow", "GENERATOR DEBUG WINDOW", nullptr));
        label->setText(QCoreApplication::translate("startupWindow", "System Startup", nullptr));
    } // retranslateUi

};

namespace Ui {
    class startupWindow: public Ui_startupWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTUP_H
