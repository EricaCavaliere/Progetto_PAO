#include "view/mainwindow.h"
#include "control/controller.h"
#include <QApplication>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    DatiGrafico linea(QString("titolo"),QString("intestazione"),DatiGrafico::linea);
    DatiGrafico punti(QString("titolo"),QString("intestazione"),DatiGrafico::punti);
    DatiGrafico torta(QString("titolo"),QString("intestazione"),DatiGrafico::torta);
    w.aggiungiGrafico(&linea);
    w.aggiungiGrafico(&punti);
    w.aggiungiGrafico(&torta);
    w.show();
    return a.exec();
}
