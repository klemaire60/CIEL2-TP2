#pragma once

#include <QtWidgets/QMainWindow>
#include <qserialport.h> 
#include <QSerialPortInfo>
#include "ui_QtLiaisonSerie.h"

class QtLiaisonSerie : public QMainWindow
{
    Q_OBJECT

public:
    QtLiaisonSerie(QWidget *parent = nullptr);
    ~QtLiaisonSerie();

private:
    Ui::QtLiaisonSerieClass ui;
    QSerialPort* port;

public slots:
    void onOpenPortButtonClicked();
    void onSerialPortReadyRead();
    void onSendButtonClicked();
};
