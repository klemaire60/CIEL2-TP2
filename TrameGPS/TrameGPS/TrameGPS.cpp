#include "TrameGPS.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSqlQuery>
#include <QDebug>

TrameGPS::TrameGPS(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    buffer.clear();

    // Remplir la liste des ports disponibles
    updatePortList();
}

TrameGPS::~TrameGPS()
{
    if (port && port->isOpen()) {
        port->close();
    }
}

void TrameGPS::updatePortList()
{
    // Effacer la liste existante
    ui.portChoiceComboBox->clear();

    // R�cup�rer les informations sur les ports s�rie disponibles
    QList<QSerialPortInfo> serialPortInfos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo& info : serialPortInfos) {
        // Ajouter chaque port s�rie � la liste d�roulante
        ui.portChoiceComboBox->addItem(info.portName());
    }

    // Si aucun port s�rie n'est disponible, afficher un message
    if (ui.portChoiceComboBox->count() == 0) {
        ui.portChoiceComboBox->addItem("Aucun port serie disponible");
    }
}

void TrameGPS::onOpenPortButtonClicked()
{
    qDebug("ok");

    // V�rifier si un port est s�lectionn� et s'il est valide
    QString selectedPort = ui.portChoiceComboBox->currentText();

    if (selectedPort.isEmpty() || selectedPort == "Aucun port serie disponible") {
        ui.portStatusLabel->setText("Veuillez selectionner un port");
        return;
    }

    // Cr�er un objet QSerialPort avec le port s�lectionn�
    port = new QSerialPort(selectedPort, this);
    QObject::connect(port, &QSerialPort::readyRead, this, &TrameGPS::onSerialPortReadyRead);

    // Configurer le port s�rie
    port->setBaudRate(QSerialPort::Baud9600);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);

    // Ouvrir le port s�rie en lecture et �criture
    if (port->open(QIODevice::ReadWrite)) {
        ui.portStatusLabel->setText("Port ouvert avec succes");
    }
    else {
        ui.portStatusLabel->setText("Erreur d'ouverture du port");
        qDebug() << "Erreur d'ouverture du port : " << port->errorString();
    }
}


void TrameGPS::onSerialPortReadyRead()
{
    QByteArray data = port->readAll();  // Utilisez le bon objet 'port' ici
    QString trame = QString::fromUtf8(data);

    // Debugging: Log trame brute pour v�rifier
    qDebug() << "Trame brute : " << trame;

    // V�rifier si la trame contient "$GPGGA", qui est la trame GPS de position
    if (trame.contains("$GPGGA")) {
        QStringList trameDecoupe = trame.split(',');

        // V�rification si la trame contient assez de donn�es
        if (trameDecoupe.size() > 5) {
            // Extraction de la latitude (au format NMEA, par exemple : 4855.1234)
            QString latitudeBrute = trameDecoupe[2];
            // Extraction de la longitude (au format NMEA, par exemple : 02326.5678)
            QString longitudeBrute = trameDecoupe[4];

            // Debugging: Log des donn�es extraites
            qDebug() << "Latitude brute : " << latitudeBrute;
            qDebug() << "Longitude brute : " << longitudeBrute;

            // Conversion des valeurs de latitude et longitude en format d�cimal
            double latitude = convertNMEA(latitudeBrute);
            double longitude = convertNMEA(longitudeBrute);

            // Debugging: Log des donn�es converties
            qDebug() << "Latitude convertie : " << latitude;
            qDebug() << "Longitude convertie : " << longitude;

            // Envoi des donn�es converties dans la base de donn�es
            sendToBdd(QString::number(latitude), QString::number(longitude));
        }
        else {
            qDebug() << "Trame invalide ou incompl�te.";
        }
    }
}

double TrameGPS::convertNMEA(const QString& nmea)
{
    if (nmea.isEmpty()) return 0.0;

    // Conversion des coordonn�es NMEA (latitude/longitude)
    double degrees = nmea.left(2).toDouble();
    double minutes = nmea.mid(2).toDouble();
    return degrees + (minutes / 60.0);
}

void TrameGPS::sendToBdd(const QString& latitude, const QString& longitude)
{
    if (db.isOpen()) {
        QSqlQuery query;
        query.prepare("INSERT INTO gps_data (latitude, longitude) VALUES (?, ?)");
        query.addBindValue(latitude);
        query.addBindValue(longitude);

        if (!query.exec()) {
            qDebug() << "Erreur d'insertion dans la base de donnees: " << query.lastError();
        }
    }
}

void TrameGPS::connectToBdd(const QString& host, int port, const QString& dbName, const QString& user, const QString& password)
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(host);
    db.setPort(port);
    db.setDatabaseName(dbName);
    db.setUserName(user);
    db.setPassword(password);

    if (!db.open()) {
        QMessageBox::critical(this, "Erreur de connexion", "Impossible de se connecter a la base de donnees.");
    }
    else {
        qDebug() << "Connexion a la base de donnees reussie!";
    }
}
