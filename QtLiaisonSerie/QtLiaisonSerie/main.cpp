#include "QtLiaisonSerie.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtLiaisonSerie w;
    w.show();
    return a.exec();
}
