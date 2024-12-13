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
    w.connectToBdd("localhost", 3000, "TP2-CIEL2", "root", "root");

    return a.exec();
}
