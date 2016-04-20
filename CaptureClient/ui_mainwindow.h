/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *uVBitrate;
    QLineEdit *uABitrate;
    QLabel *label_3;
    QLineEdit *uFramerate;
    QPushButton *stateButton;
    QLabel *label_2;
    QPushButton *Open;
    QPushButton *liveState;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_2;
    QLabel *label_4;
    QComboBox *NICinfo;
    QLabel *label_5;
    QLineEdit *serverAddr;
    QPushButton *uploadButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(649, 241);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 40, 271, 111));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 1, 1, 1);

        uVBitrate = new QLineEdit(layoutWidget);
        uVBitrate->setObjectName(QStringLiteral("uVBitrate"));

        gridLayout->addWidget(uVBitrate, 0, 2, 1, 1);

        uABitrate = new QLineEdit(layoutWidget);
        uABitrate->setObjectName(QStringLiteral("uABitrate"));

        gridLayout->addWidget(uABitrate, 1, 2, 1, 1);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 2, 1, 1, 1);

        uFramerate = new QLineEdit(layoutWidget);
        uFramerate->setObjectName(QStringLiteral("uFramerate"));

        gridLayout->addWidget(uFramerate, 2, 2, 1, 1);

        stateButton = new QPushButton(layoutWidget);
        stateButton->setObjectName(QStringLiteral("stateButton"));

        gridLayout->addWidget(stateButton, 0, 0, 1, 1);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 1, 1, 1);

        Open = new QPushButton(layoutWidget);
        Open->setObjectName(QStringLiteral("Open"));

        gridLayout->addWidget(Open, 2, 0, 1, 1);

        liveState = new QPushButton(layoutWidget);
        liveState->setObjectName(QStringLiteral("liveState"));

        gridLayout->addWidget(liveState, 1, 0, 1, 1);

        layoutWidget1 = new QWidget(centralWidget);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(288, 40, 291, 111));
        gridLayout_2 = new QGridLayout(layoutWidget1);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(layoutWidget1);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_2->addWidget(label_4, 0, 0, 1, 1);

        NICinfo = new QComboBox(layoutWidget1);
        NICinfo->setObjectName(QStringLiteral("NICinfo"));

        gridLayout_2->addWidget(NICinfo, 0, 1, 1, 1);

        label_5 = new QLabel(layoutWidget1);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_2->addWidget(label_5, 1, 0, 1, 1);

        serverAddr = new QLineEdit(layoutWidget1);
        serverAddr->setObjectName(QStringLiteral("serverAddr"));

        gridLayout_2->addWidget(serverAddr, 1, 1, 1, 1);

        uploadButton = new QPushButton(layoutWidget1);
        uploadButton->setObjectName(QStringLiteral("uploadButton"));

        gridLayout_2->addWidget(uploadButton, 2, 1, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 649, 23));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "CaptureClient", 0));
        label->setText(QApplication::translate("MainWindow", "Video Bitrate", 0));
        label_3->setText(QApplication::translate("MainWindow", "Video Framerate", 0));
        stateButton->setText(QApplication::translate("MainWindow", "begin", 0));
        label_2->setText(QApplication::translate("MainWindow", "Audio Bitrate", 0));
        Open->setText(QApplication::translate("MainWindow", "Open", 0));
        liveState->setText(QApplication::translate("MainWindow", "Start live", 0));
        label_4->setText(QApplication::translate("MainWindow", "NIC", 0));
        label_5->setText(QApplication::translate("MainWindow", "ServerAddress", 0));
        serverAddr->setInputMask(QString());
        serverAddr->setText(QString());
        uploadButton->setText(QApplication::translate("MainWindow", "upload", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
