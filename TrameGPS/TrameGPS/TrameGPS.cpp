#include "TrameGPS.h"
#define __WIN__
#include <QSqlDatabase>
#include <QSqlQuery>

TrameGPS::TrameGPS(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    buffer.clear();

    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();

    for (int i = 0; i < availablePorts.size(); i++)
    {
        QSerialPortInfo info = availablePorts[i];
        ui.portChoiceComboBox->addItem(info.portName(), QVariant(info.portName()));
    }
}

TrameGPS::~TrameGPS()
{
}

void TrameGPS::connectToBdd()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("192.168.64.184");
    db.setDatabaseName("TP2-CIEL2");
    db.setUserName("tp2");
    db.setPassword("tp2");
    bool ok = db.open();
    if (!ok) QMessageBox::critical(this, "Erreur BDD", QString("Erreur avec la BDD: " + db.lastError().text()));
}

void TrameGPS::sendToBdd(QString lattitude, QString longitude)
{
    QSqlQuery query;
    query.prepare("INSERT INTO gps (latitude, longitude) " "VALUES (:latitude, :longitude)");
    query.bindValue(":latitude", lattitude.toDouble());
    query.bindValue(":longitude", longitude.toDouble());
    query.exec();
}

void TrameGPS::setupDatabase() {
    this->db = QSqlDatabase::addDatabase("QMYSQL");
    this->db.setHostName("192.168.64.184");
    this->db.setDatabaseName("TP2-CIEL2");
    this->db.setUserName("root");
    this->db.setPassword("root");
    if (!this->db.open()) {
        qDebug("Erreur avec la BDD");
    }
    else {
        qDebug("Connexion à la BDD réussie");
    }
}

void TrameGPS::sendToBdd(const QString& latitude, const QString& longitude) {
    QSqlQuery query;
    query.prepare("INSERT INTO gps (latitude, longitude) VALUES (:latitude, :longitude)");
    query.bindValue(":latitude", latitude.toDouble());
    query.bindValue(":longitude", longitude.toDouble());
    if (!query.exec()) {
        qDebug("Erreur lors de l'insertion dans la BDD");
        //qDebug() << query.lastError().text();
    }
    else {
        qDebug("Données insérées avec succès dans la BDD");
    }
}

void TrameGPS::onOpenPortButtonClicked()
{
    qDebug("ok");
    if (ui.portChoiceComboBox->currentIndex() >= 0)
    {
        port = new QSerialPort(ui.portChoiceComboBox->currentText());
        QObject::connect(port, SIGNAL(readyRead()), this, SLOT(onSerialPortReadyRead()));
        port->setBaudRate(9600);
        port->setDataBits(QSerialPort::DataBits::Data8);
        port->setParity(QSerialPort::Parity::NoParity);
        port->setStopBits(QSerialPort::StopBits::OneStop);
        if (port->open(QIODevice::OpenModeFlag::ExistingOnly | QIODevice::OpenModeFlag::ReadWrite));
        {
            ui.portStatusLabel->setText("Statut port: Ouvert");
        }
    }
}

void TrameGPS::onSerialPortReadyRead() {
    QByteArray data = port->readAll();
    buffer.append(data);
    qDebug(("Data received: " + QString(buffer)).toStdString().c_str());

    int startIndex = buffer.indexOf("$GPGGA");
    int endIndex = buffer.indexOf("*", startIndex + 1);

    while (startIndex != -1 && endIndex != -1 && startIndex < endIndex) {
        QString trame = buffer.mid(startIndex, endIndex - startIndex + 1);
        //qDebug(("Trame extracted: " + trame).toStdString().c_str());

        QStringList trameDecoupe = trame.split(",");
        if (trameDecoupe.size() >= 15) {
            QString latitude = trameDecoupe[2];
            QString longitude = trameDecoupe[4].mid(2); // Remove the leading "00"
            qDebug(("Latitude: " + latitude + " Longitude: " + longitude).toStdString().c_str());
            // this->sendToBdd(latitude, longitude);
        }
        else {
            qDebug("Invalid trame format");
        }

        // Remove the processed trame from the buffer
        buffer.remove(0, endIndex + 1);

        // Find the next trame
        startIndex = buffer.indexOf("$GPGGA");
        endIndex = buffer.indexOf("*", startIndex + 1);
    }
}
