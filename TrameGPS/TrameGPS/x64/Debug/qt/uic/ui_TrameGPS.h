/********************************************************************************
** Form generated from reading UI file 'TrameGPS.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRAMEGPS_H
#define UI_TRAMEGPS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TrameGPSClass
{
public:
    QWidget *centralWidget;
    QComboBox *portChoiceComboBox;
    QPushButton *openPortButton;
    QLabel *portStatusLabel;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TrameGPSClass)
    {
        if (TrameGPSClass->objectName().isEmpty())
            TrameGPSClass->setObjectName(QString::fromUtf8("TrameGPSClass"));
        TrameGPSClass->resize(600, 400);
        centralWidget = new QWidget(TrameGPSClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        portChoiceComboBox = new QComboBox(centralWidget);
        portChoiceComboBox->setObjectName(QString::fromUtf8("portChoiceComboBox"));
        portChoiceComboBox->setGeometry(QRect(50, 20, 101, 22));
        openPortButton = new QPushButton(centralWidget);
        openPortButton->setObjectName(QString::fromUtf8("openPortButton"));
        openPortButton->setGeometry(QRect(50, 50, 111, 21));
        portStatusLabel = new QLabel(centralWidget);
        portStatusLabel->setObjectName(QString::fromUtf8("portStatusLabel"));
        portStatusLabel->setGeometry(QRect(180, 20, 121, 31));
        TrameGPSClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(TrameGPSClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 20));
        TrameGPSClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TrameGPSClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        TrameGPSClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(TrameGPSClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        TrameGPSClass->setStatusBar(statusBar);

        retranslateUi(TrameGPSClass);
        QObject::connect(openPortButton, SIGNAL(clicked()), TrameGPSClass, SLOT(onOpenPortButtonClicked()));

        QMetaObject::connectSlotsByName(TrameGPSClass);
    } // setupUi

    void retranslateUi(QMainWindow *TrameGPSClass)
    {
        TrameGPSClass->setWindowTitle(QCoreApplication::translate("TrameGPSClass", "TrameGPS", nullptr));
        openPortButton->setText(QCoreApplication::translate("TrameGPSClass", "Ouvrir le port", nullptr));
        portStatusLabel->setText(QCoreApplication::translate("TrameGPSClass", "Statut port : ferme", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TrameGPSClass: public Ui_TrameGPSClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAMEGPS_H
