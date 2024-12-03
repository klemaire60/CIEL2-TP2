#include "QtLiaisonSerie.h"

QtLiaisonSerie::QtLiaisonSerie(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();

    for (int i = 0; i < availablePorts.size(); i++)
    {
        QSerialPortInfo info = availablePorts[i];
        ui.portChoiceComboBox->addItem(info.portName(), QVariant(info.portName()));
    }
}

void QtLiaisonSerie::onOpenPortButtonClicked()
{
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

void QtLiaisonSerie::onSerialPortReadyRead()
{
    QByteArray data = port->read(port->bytesAvailable());
    QString str(data);
    ui.receivedDataText->insert(str);
}

void QtLiaisonSerie::onSendButtonClicked()
{
    if (port->isOpen())
    {
        port->write(ui.lineEdit_2->text().toStdString().c_str());
    }
}

QtLiaisonSerie::~QtLiaisonSerie()
{}