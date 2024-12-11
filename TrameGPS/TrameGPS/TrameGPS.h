#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TrameGPS.h"
#include <qserialport.h> 
#include <QSerialPortInfo>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>

class TrameGPS : public QMainWindow
{
    Q_OBJECT

public:
    TrameGPS(QWidget *parent = nullptr);
    ~TrameGPS();

    void connectToBdd();

private:
    Ui::TrameGPSClass ui;
    QSerialPort* port;
    void sendToBdd(QString lattitude, QString longitude);
    QByteArray buffer; // Tampon pour accumuler les données reçues
    QSqlDatabase db;

    void setupDatabase();
    void sendToBdd(const QString& latitude, const QString& longitude);

public slots:
    void onOpenPortButtonClicked();
    void onSerialPortReadyRead();
};
