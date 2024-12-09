#include "TrameGPS.h"
#include <QtWidgets/QApplication>
#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TrameGPS w;
    w.show();

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("192.168.64.184");
    db.setDatabaseName("TP2-CIEL2");
    db.setUserName("root");
    db.setPassword("root");
    bool ok = db.open();
    if (!ok) qDebug("Erreur avec la BDD");

    return a.exec();
}
