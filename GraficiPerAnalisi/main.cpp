#include "view/mainwindow.h"
#include "view/datigrafico.h"
#include <QApplication>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    DatiGrafico linea(10,20,2,DatiGrafico::TipoGrafico::linea,"Grafico a linea");
    DatiGrafico punti(10,20,2,DatiGrafico::TipoGrafico::punti,"Grafico a punti");
    DatiGrafico torta(10,20,2,DatiGrafico::TipoGrafico::torta,"Grafico a torta");
    DatiGrafico prova;
    w.aggiungiGrafico(&linea);
    w.aggiungiGrafico(&punti);
    w.aggiungiGrafico(&torta);
    w.aggiungiGrafico(&prova);
    w.show();

    return a.exec();
}
