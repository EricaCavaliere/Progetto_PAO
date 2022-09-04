#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "model/campione.h"
#include "model/elemento.h"
#include "model/composto.h"
#include "model/miscela.h"
#include "view/mainwindow.h"

#include <QString>

typedef QPair<Campione*,QString> Path;

class Controller: public QWidget
{
    Q_OBJECT
public:
    explicit Controller(QWidget* = 0);
    ~Controller();
private:
    MainWindow* view;
    QVector<Path> model;

    bool isFileOpenRead(QFile&);
    bool isFileOpenWrite(QFile&);

    void insertInModel(Campione*, QString);
    void viewTable(QAbstractItemModel*);

    Campione::Stato fromStringToStato(QString);
    QString fromStatoToString(Campione::Stato);
    DatiGrafico::TipoGrafico fromStringToGrafico(QString);
    QString fromGraficoToString(DatiGrafico::TipoGrafico);

    Elemento crea_elemento();
    Composto crea_composto();
    Miscela crea_miscela();

    Composto lettura_composto(QJsonDocument&);
    Miscela lettura_miscela(QJsonDocument&);
    DatiGrafico lettura_grafico(QJsonDocument&);

    void salva_composto_json(QJsonObject&,Composto*,const DatiGrafico&);
    void salva_miscela_json(QJsonObject&,Miscela*,const DatiGrafico&);
    void imposta_tabella_json(QJsonObject&,const DatiGrafico &);

public slots:
    void nuovo_file();
    void apri_file();
    void salva_file(const QVector<DatiGrafico>&);

};

#endif // CONTROLLER_H
