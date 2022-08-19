#include "view/mainwindow.h"
#include "view/datigrafico.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    DatiGrafico* linea = new DatiGrafico(10,20,2,DatiGrafico::TipoGrafico::linea,"Grafico a linea");
    DatiGrafico* punti = new DatiGrafico(10,20,2,DatiGrafico::TipoGrafico::punti,"Grafico a punti");
    DatiGrafico* torta = new DatiGrafico(10,20,2,DatiGrafico::TipoGrafico::torta,"Grafico a torta");
    DatiGrafico* prova = new DatiGrafico();
    w.aggiungiGrafico(linea);
    w.aggiungiGrafico(punti);
    w.aggiungiGrafico(torta);
    w.aggiungiGrafico(prova);
    w.show();

    delete linea;
    delete punti;
    delete torta;
    delete prova;

    return a.exec();
}
