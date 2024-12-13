#include "TrameGPS.h"
#include <QtWidgets/QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    TrameGPS w;

    // Affichage de l'interface avant la connexion à la base de données
    w.show();

    // Connexion à la base de données après que l'interface soit affichée
    w.connectToBdd("192.168.64.184", 3306, "TP2-CIEL2", "site", "site");

    return a.exec();
}
