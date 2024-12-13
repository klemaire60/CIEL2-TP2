#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TrameGPS.h"
#include <QSerialPort.h> 
#include <QSerialPortInfo>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>

class TrameGPS : public QMainWindow
{
    Q_OBJECT

public:
    TrameGPS(QWidget* parent = nullptr);
    ~TrameGPS();

    void connectToBdd(const QString& host, int port, const QString& dbName, const QString& user, const QString& password);
    void updatePortList(); // Ajouter cette méthode pour mettre à jour la liste des ports série

private:
    Ui::TrameGPSClass ui;
    QSerialPort* port;
    void sendToBdd(const QString& latitude, const QString& longitude);
    QByteArray buffer; // Tampon pour accumuler les données reçues
    QSqlDatabase db;

    void setupDatabase();
    double convertNMEA(const QString& nmea);

public slots:
    void onOpenPortButtonClicked();
    void onSerialPortReadyRead();
};
