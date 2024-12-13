#include "TrameGPS.h"
#include <QtWidgets/QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    TrameGPS w;

    // Affichage de l'interface avant la connexion � la base de donn�es
    w.show();

    // Connexion � la base de donn�es apr�s que l'interface soit affich�e
    w.connectToBdd("192.168.64.184", 3306, "TP2-CIEL2", "site", "site");

    return a.exec();
}
