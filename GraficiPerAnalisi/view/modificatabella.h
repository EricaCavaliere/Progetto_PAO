#ifndef MODIFICATABELLA_H
#define MODIFICATABELLA_H

#include "datigrafico.h"

#include <QWidget>
#include <QTableView>

class ModificaTabella:public QWidget
{
    Q_OBJECT
public:
    ModificaTabella(int rows, int columns, DatiGrafico::TipoGrafico d, QWidget* parent=nullptr);
private:
    DatiGrafico::TipoGrafico tipo;
    QTableView* view;
    QAbstractItemModel *model;
private slots:
    DatiGrafico* ok_clicked();
    void annulla_clicked();
};

#endif // MODIFICATABELLA_H
