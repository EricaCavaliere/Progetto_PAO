#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "view/mainwindow.h"
#include "model/elemento.h"
#include "model/composto.h"

#include <vector>

class Controller:public MainWindow
{
public:
    Controller();
private:
    Elemento inserire_elemento();
    Composto lettura_composto(QFile*);
private slots:
    void menu_file_nuovo() override;
    void menu_file_apri() override;
    void menu_file_salva() override;

    void nuovo_elemento_Aggiorna();
    void nuovo_elemento_Annulla();
};

#endif // CONTROLLER_H
