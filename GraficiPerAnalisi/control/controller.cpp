#include "controller.h"

#include <QInputDialog>


Controller::Controller():MainWindow(){show();}

void Controller::menu_file_nuovo(){
    QStringList items;
    items << tr("Elemento") << tr("Composto") << tr("Sostanza") << tr("Miscela");
    QString i = QInputDialog::getItem(this,tr("Nuovo grafico"),tr("Che cosa vuole creare?"),items);

    if(i==tr("Elemento")){
        /*
         * si apre un file json dove ci saranno salvati gli elementi della tavola periodica
         * si mostra a vide un QDialog con elencato gli elementi gia' salvati
         * se l'elemento interessato non e' presente, si permettera' di aggiungere a mano un nuovo elemento
         * per aggiungere un nuovo elemento si aprira' un nuovo qdialog che permette di inserire i dati dell'elemento
         * il nuovo elemento sara' inserito nel file json e salvato.
         */
    }else if(i==tr("Composto")){
        /*
         * Il composto e' una sostanza con un insieme di elementi.
         * si apre un QDialog dove si andra' ad indicare di quali elementi e' composto e altri dati che possono formare un composto
         * - se viene cliccato OK allora sara reso visibile il grafico a torta del composto nel MainWindow e verra' segnato come grarico da slavare successivamente
         * - se viene clicato ANNULLA o X, allora tutte le indicazioni fatte nel QDialog andranno perdute e non verra' creato alcun grafico.
         */
    }else if(i==tr("Sostanza")){
        /*
         * La sosatanza sara' simile al campione con i dati inseriti (HA SENSO INSERIRLA?).
         * Sara' aperto un QDialog che permettera' di inserire i dati.
         * Per quanto riguarda il grafico, la visualizzazione sara' uguale al Composto
         */
    }else if(i==tr("Miscela")){
        /*
         * Una miscela e' un insieme di composti.
         * Verra' aperto un QDialog dove sara' richiesto di indicare i composti di cui e' formato la miscela.
         * Ad ogni composto o viene creato a mano, o viene data la possibilita' di aprire un QFileDialog dove sara' possibile trovare il file json del composto interessato
         * Dopo aver inserito i Composti e i dati relativi alla Miscela, si dara' la possibilita' di vedere il grafico a video tramite il pulsante OK o annullare tutte le impostazioni tramite il pulsante ANNULLA o X.
         */
    }
}

void Controller::menu_file_apri(){
    /*
     * si aprira' un semplice QFileDialog dove sara' possibile indicare il file json con i dati del grafico interessato.
     */
}

void Controller::menu_file_salva(){
    /*
     * saranno controllate tutti i
     */
}

void Controller::menu_file_salvaTutto(){

}
