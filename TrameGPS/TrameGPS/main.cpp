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
    w.connectToBdd("localhost", 3000, "TP2-CIEL2", "root", "root");

    return a.exec();
}
