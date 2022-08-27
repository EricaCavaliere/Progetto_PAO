#include "modificatabella.h"

#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QPushButton>

ModificaTabella::ModificaTabella(int rows, int columns, DatiGrafico::TipoGrafico t, QWidget* parent):QWidget(parent),tipo(t){
    if(columns%2!=0) columns++;

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *informazione = new QLabel(tr("Inserire nella tabella i dati:"),this);
    layout->addWidget(informazione);

    model = new QStandardItemModel(rows,columns,this);
    if(tipo == DatiGrafico::TipoGrafico::torta){
        QString fette = tr("Nomi fette");
        QString dati = tr("Dati fette");
        for(int i=0;i<columns/2;++i){
            model->setHeaderData(i*2,Qt::Horizontal,fette);
            model->setHeaderData(i*2+1,Qt::Horizontal,dati);
        }
        QModelIndex index;
        for(int i=0;i<rows;++i){
            for(int j=0;j<columns;++j){
                //nome
                index = model->index(i,j*2);
                model->setData(index,"Slide");
                //dato (y)
                index = model->index(i,j*2+1);
                model->setData(index,0.0);
            }
        }
    }else{
        QString x = "x";
        QString y = "y";
        for(int i=0;i<columns/2;++i){
            model->setHeaderData(i*2,Qt::Horizontal,x);
            model->setHeaderData(i*2+1,Qt::Horizontal,y);
        }
        QModelIndex index;
        for(int i=0;i<rows;++i){
            for(int j=0;j<columns;++j){
                //x
                index = model->index(i,j*2);
                model->setData(index,0.0);
                //y
                index = model->index(i,j*2+1);
                model->setData(index,0.0);
            }
        }
    }
    view = new QTableView;
    view->setModel(model);
    QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
    view->setSelectionModel(selectionModel);
    //QHeaderView *headerView = view->horizontalHeader();
    //headerView->setStretchLastSection(true);
    layout->addWidget(view);

    QVBoxLayout *pulsanti = new QVBoxLayout(this);
    QPushButton *ok = new QPushButton(tr("OK"),this);
    QPushButton *annulla = new QPushButton(tr("ANNULLA"),this);
    connect(ok,&QPushButton::clicked,this,&ModificaTabella::ok_clicked);
    //connect(annulla,&QPushButton::clicked,this,&ModificaTabella::annulla_clicked);
    pulsanti->addWidget(ok);
    pulsanti->addWidget(annulla);
    layout->addLayout(pulsanti);

    setLayout(layout);
    setVisible(true);
}

DatiGrafico* ModificaTabella::ok_clicked(){
    DatiGrafico* d = new DatiGrafico();
    int rows = view->model()->rowCount();

    QModelIndex indexX,indexY;
    if(tipo==DatiGrafico::TipoGrafico::torta){
        for(int i=0;i<rows;++i){
            indexX = view->model()->index(rows,0);
            indexY = view->model()->index(rows,1);
            QString x = view->model()->data(indexX).toString();
            int y = view->model()->data(indexY).toInt();
            d->setPunto(0.0,y,0,i);
            d->setPunto(x,0,i);
        }
    }else{
        for(int i=0;i<rows;++i){
            indexX = view->model()->index(rows,0);
            indexY = view->model()->index(rows,1);
            int x = view->model()->data(indexX).toInt();
            int y = view->model()->data(indexY).toInt();
            d->setPunto(x,y,0,i);
        }
    }

    close();
    return d;
}
void ModificaTabella::annulla_clicked(){}
