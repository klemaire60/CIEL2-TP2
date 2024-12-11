#include "TrameGPS.h"
#include <QtWidgets/QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TrameGPS w;
    w.show();

    w.connectToBdd();

    return a.exec();
}
