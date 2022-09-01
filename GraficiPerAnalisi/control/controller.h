#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "model/campione.h"
#include "model/elemento.h"
#include "model/composto.h"
#include "model/miscela.h"
#include "view/mainwindow.h"

#include <QString>

typedef QPair<Campione,QString> Path;

class Controller
{
    Q_OBJECT
public:
    explicit Controller(MainWindow*);
    ~Controller();
private:
    MainWindow* view;
    QVector<Path> model;//dati e percorso cartella
    Elemento inserire_elemento();
    Campione::Stato lettura_stato(QString);
    Composto lettura_composto(QFile*);
    Miscela lettura_miscela(QFile*);
private slots:
    void nuovo_file();
    void apri_file();
    void salva_file(QVector<DatiGrafico*>);
    //void menu_file_nuovo();
    //void menu_file_apri();
    //void menu_file_salva();
    //void nuovo_elemento_Aggiorna();
    //void nuovo_elemento_Annulla();
};

#endif // CONTROLLER_H
