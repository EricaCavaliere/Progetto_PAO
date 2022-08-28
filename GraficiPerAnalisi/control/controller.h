#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "view/mainwindow.h"
#include "model/campione.h"

#include <vector>

class Controller:public MainWindow
{
public:
    Controller();
private:
    std::vector<Campione> c;
private slots:
    void menu_file_nuovo() override;
    void menu_file_apri() override;
    void menu_file_salva() override;
    void menu_file_salvaTutto() override;
};

#endif // CONTROLLER_H
