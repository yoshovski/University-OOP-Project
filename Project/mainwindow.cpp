#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QStatusBar>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <string>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    ui->btn_home->setDisabled(true);
    ui->stackedWidget->setCurrentWidget(ui->page_Home);

}

Conferenza* MainWindow::leggiConferenza(QStringList colonne){
    QString nome = colonne.at(0);
    QString acronimo = colonne.at(1);
    QString luogo = colonne.at(2);
    QString data_string = colonne.at(3);
    QDate data = QDate::fromString(data_string.right(10), "dd/MM/yyyy");
    QStringList organizzatori = colonne.at(4).split(",");
    int numPartecipanti = colonne.at(5).toInt();

    Conferenza* c = new Conferenza(nome,acronimo,luogo,data,organizzatori,numPartecipanti);
    return c;
}

Rivista* MainWindow::leggiRivista(QStringList colonne){
    QString nome = colonne.at(0);
    QString acronimo = colonne.at(1);
    QString editore = colonne.at(2);
    QString data_string = colonne.at(3);
    QDate data = QDate::fromString(data_string.right(10), "dd/MM/yyyy");
    int volume = colonne.at(4).toInt();

    Rivista* r = new Rivista(nome,acronimo,editore,data,volume);
    return r;
}

void MainWindow::erroreImportazione(int row){
    QString testo = "Il file di testo contiene errore in riga " + QString::number(row);
    QMessageBox alert(QMessageBox::Critical, "Attenzione", testo , QMessageBox::Ok, this);
    alert.exec();
    return;
}

MainWindow::~MainWindow()
{
    delete g;
    delete ui;
}

void MainWindow::on_btn_aggiungiAutore_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_aggiungiAutore);
}

void MainWindow::on_btn_autore_conferma_clicked()
{
    //Prendi i valori dai campi di testo
    QString id = ui->lineEdit_autore_id->text();
    QString nome = ui->lineEdit_autore_nome->text();
    QString cognome = ui->lineEdit_autore_cognome->text();
    int numeroAfferenze = ui->lista_autore_afferenze->count();

    //Se almeno UN (1) campo è vuoto
    if(id.isEmpty() || nome.isEmpty() || cognome.isEmpty() || numeroAfferenze<=0){
        QMessageBox alert(QMessageBox::Critical, "Attenzione", "I campi non possono essere vuoti!", QMessageBox::Ok, this);
        alert.exec();
        return;
    }
    else{
        QStringList afferenze;

        //prendo i valori dalla lista Afferenze
        for(int i = 0; i < numeroAfferenze; i++)
             afferenze << ui->lista_autore_afferenze->item(i)->text();

        Autore* a = new Autore(id, nome, cognome, afferenze);

        //se restituisce FALSE - autore già presente
        if(!(g->aggiungiAutore(a))){
            QMessageBox alert2(QMessageBox::Critical, "Attenzione", "Autore gia' presente in lista", QMessageBox::Ok, this);
            alert2.exec();
            return;
        }
    }

    //ripulisco i campi per un nuovo inserimento
    ui->lineEdit_autore_afferneze->clear();
    ui->lineEdit_autore_cognome->clear();
    ui->lineEdit_autore_id->clear();
    ui->lineEdit_autore_nome->clear();
    ui->lista_autore_afferenze->clear();

    ui->statusbar->showMessage("Autore aggiunto", 3000);
}

void MainWindow::on_btn_autore_aggiungiAfferenza_clicked()
{
    QString afferenza = ui->lineEdit_autore_afferneze->text();

    //il campo Afferenze non può essere vuoto
    if(afferenza.isEmpty()){
        QMessageBox alert(QMessageBox::Critical, "Attenzione", "Il campo non puo' essere vuoto!", QMessageBox::Ok, this);
        alert.exec();
        return;
    }

    //Controllo se l'afferenza è già presente nella lista che è in corso di creazione
    bool notFound = ui->lista_autore_afferenze->findItems(afferenza,Qt::MatchExactly).isEmpty();
    if(notFound)
        ui->lista_autore_afferenze->addItem(afferenza);
    else{
        QMessageBox alert2(QMessageBox::Critical, "Attenzione", "Afferenza gia' inserita!", QMessageBox::Ok, this);
        alert2.exec();
        ui->lineEdit_autore_afferneze->clear();
        return;
    }

    //Ripulisco campo
    ui->lineEdit_autore_afferneze->clear();
}

void MainWindow::on_btn_visualizzaAutori_clicked()
{
    //non prosegue se non c'è nessun autore
    if(g->getAutori().isEmpty()){
        if(ui->lista_showConferenze->selectedItems().size()==0){
                QMessageBox alert(QMessageBox::Critical, "Attenzione", "Non hai aggiunto nessun autore!", QMessageBox::Ok, this);
                alert.exec();
                return;
            }
    }
    ui->lista_showAutori->clear();

    //Non mostra afferenze se non e' selezionato nessun autore
    if(ui->lista_showAutori->selectedItems().size()==0){
        ui->lista_showAutori_afferenze->clear();
    }

    //mostra tutti gli autori
    QStringList autori = g->listaAutori();
    ui->lista_showAutori->addItems(autori);
    ui->stackedWidget->setCurrentWidget(ui->page_visualizzaAutori);
}

void MainWindow::on_btn_home_clicked()
{
   //#1 PULIZIONE tutti i campi delle pagine "aggiungi"
    {
    //pulisci "aggiungi Autore"
    {
    ui->lineEdit_autore_id->clear();
    ui->lineEdit_autore_nome->clear();
    ui->lineEdit_autore_cognome->clear();
    ui->lineEdit_autore_afferneze->clear();
    ui->lista_autore_afferenze->clear();}

    //pulisci "aggiungi Conferenza"
    {
      ui->lineEdit_conferenza_nome->clear();
      ui->lineEdit_conferenza_acronimo->clear();
      ui->lineEdit_conferenza_organizzatore->clear();
      ui->lineEdit_conferenza_luogo->clear();
      ui->spinBox_conferenza_numPartecipanti->setValue(1); //il minimo valore di partecipanti dev'essere 1
      ui->lista_conferenza_organizzatori->clear();
    }

    //pulisci "aggiungi Rivista"
    {
      ui->lineEdit_rivista_nome->clear();
      ui->lineEdit_rivista_acronimo->clear();
      ui->lineEdit_rivista_editore->clear();
      ui->spinBox_rivista_volume->setValue(1); //il minimo valore di volume dev'essere 1
    }

    //pulisci "aggiungi Articolo"
    {
       ui->lineEdit_articolo_id->clear();
       ui->lineEdit_articolo_titolo->clear();
       ui->lineEdit_articolo_keyword->clear();
       ui->lista_articolo_keywords->clear();
       ui->spinBox_articolo_numeroPagine->setValue(1); //numero minimo di pagine = 1
       ui->doubleSpinBox_articolo_prezzo->setValue(0.00);
       ui->radioButton_articolo3_rivista->setChecked(true); //metto il radioButton rivista di default
    }
    }

    ui->stackedWidget->setCurrentWidget(ui->page_Home);
    ui->btn_home->setDisabled(true);
}

void MainWindow::on_stackedWidget_currentChanged(int arg1)
{
    if(ui->stackedWidget->currentWidget()==ui->page_help){
        ui->btn_help->setText("Indietro");
    }
    else{
        precedente=arg1;
        ui->btn_help->setText("Help");
    }

    ui->btn_home->setDisabled(false);
}

void MainWindow::on_lista_showAutori_itemClicked(QListWidgetItem *item)
{
    ui->lista_showAutori_afferenze->clear();
    int row = ui->lista_showAutori->row(item); //prendo la posizione dell'autore scelto
    QStringList afferenze = g->listaAfferenze(row);
    ui->lista_showAutori_afferenze->addItems(afferenze); //mostro la lista delle sue afferenze
}

void MainWindow::on_btn_conferenza_aggiungiOrganizatore_clicked()
{
    QString organizzatore = ui->lineEdit_conferenza_organizzatore->text();

    //il campo Organizzatore non può essere vuoto
    if(organizzatore.isEmpty()){
        QMessageBox alert(QMessageBox::Critical, "Attenzione", "Il campo non puo' essere vuoto!", QMessageBox::Ok, this);
        alert.exec();
        return;
    }

    //Controllo se l'organizzazione è già presente nella lista che è in corso di creazione
    bool notFound = ui->lista_conferenza_organizzatori->findItems(organizzatore,Qt::MatchExactly).isEmpty();
    if(notFound)
        ui->lista_conferenza_organizzatori->addItem(organizzatore);
    else{
        QMessageBox alert2(QMessageBox::Critical, "Attenzione", "Organizzatore gia' inserito!", QMessageBox::Ok, this);
        alert2.exec();
        ui->lineEdit_autore_afferneze->clear();
        return;
    }

    //Ripulisco campo
    ui->lineEdit_conferenza_organizzatore->clear();
}

void MainWindow::on_btn_aggiungiConferenza_clicked()
{
    ui->calendarWidget_conferenza->setSelectedDate(QDate::currentDate());
    ui->stackedWidget->setCurrentWidget(ui->page_aggiungiConferenza);
}

void MainWindow::on_btn_conferenza_conferma_clicked()
{
    //Prendi i valori dai campi di testo
    QString nome = ui->lineEdit_conferenza_nome->text();
    QString acronimo = ui->lineEdit_conferenza_acronimo->text();
    QString luogo = ui->lineEdit_conferenza_luogo->text();
    QDate data = ui->calendarWidget_conferenza->selectedDate();
    int numOrganizzatori = ui->lista_conferenza_organizzatori->count();
    int numPartecipanti = ui->spinBox_conferenza_numPartecipanti->value();

    //Se almeno UN (1) campo è vuoto
    if(nome.isEmpty() || acronimo.isEmpty() || luogo.isEmpty() || numOrganizzatori<=0 || numPartecipanti==0){
        QMessageBox alert(QMessageBox::Critical, "Attenzione", "I campi non possono essere vuoti!", QMessageBox::Ok, this);
        alert.exec();
        return;
    }
    else{
        QStringList organizzatori;

        //prendo i valori dalla lista Organizzatori
        for(int i = 0; i < numOrganizzatori; i++)
             organizzatori << ui->lista_conferenza_organizzatori->item(i)->text();

        Conferenza* c = new Conferenza(nome,acronimo,luogo,data,organizzatori,numPartecipanti);

        //se restituisce FALSE - conferenza già presente
        if(!(g->aggiungiConferenza(c))){
            QMessageBox alert2(QMessageBox::Critical, "Attenzione", "Conferenza gia' presente in lista", QMessageBox::Ok, this);
            alert2.exec();
            return;
        }
    }

    //ripulisco i campi per un nuovo inserimento
    ui->lineEdit_conferenza_acronimo->clear();
    ui->lineEdit_conferenza_luogo->clear();
    ui->lineEdit_conferenza_nome->clear();
    ui->lineEdit_conferenza_organizzatore->clear();
    ui->lista_conferenza_organizzatori->clear();
    ui->spinBox_conferenza_numPartecipanti->setValue(1);
    ui->calendarWidget_conferenza->setSelectedDate(QDate::currentDate()); //reimposto la data selezionata

    ui->statusbar->showMessage("Conferenza aggiunta", 3000);
}

void MainWindow::on_btn_visualizzaConferenze_clicked()
{
    //non prosegue se non c'è nessuna conferenza
    if(g->getConferenze().isEmpty()){
            QMessageBox alert(QMessageBox::Critical, "Attenzione", "Non hai ancora aggiunto nessuna Conferenza!", QMessageBox::Ok, this);
            alert.exec();
            return;
        }

    ui->lista_showConferenze->clear();
    ui->lista_showConferenze_simili->clear();
    ui->lista_showConferenze_simili->hide();

    //Non mostra informazioni aggiuntive se non e' selezionato nessun articolo
    if(ui->lista_showConferenze->selectedItems().size()==0){
        ui->lista_showConferenze_organizzatori->clear();}

    //mostra tutte le conferenze
    QStringList conferenze = g->listaConferenze();
    ui->lista_showConferenze->addItems(conferenze);
    ui->stackedWidget->setCurrentWidget(ui->page_visualizzaConferenze);
}

void MainWindow::on_lista_showConferenze_itemClicked(QListWidgetItem *item)
{
    ui->lista_showConferenze_organizzatori->clear();
    int row = ui->lista_showConferenze->row(item); //prendo la posizione della conferenza scelta
    QStringList organizzatori = g->listaOrganizzatori(row);
    ui->lista_showConferenze_organizzatori->addItems(organizzatori); //mostro gli organizzatori della conferenza

    //mostra le conferenze simili
    QStringList conferenzeSimili = g->listaConferenzeSimili(row);
    ui->lista_showConferenze_simili->clear();
    ui->lista_showConferenze_simili->addItems(conferenzeSimili);
}

void MainWindow::on_btn_rivista_conferma_clicked()
{
    //Prende i valori dai campi di testo
    QString nome = ui->lineEdit_rivista_nome->text();
    QString acronimo = ui->lineEdit_rivista_acronimo->text();
    QString editore = ui->lineEdit_rivista_editore->text();
    QDate data = ui->calendarWidget_rivista->selectedDate();
    int volume = ui->spinBox_rivista_volume->value();

    //Se almeno UN (1) campo è vuoto
    if(nome.isEmpty() || acronimo.isEmpty() || editore.isEmpty()){
        QMessageBox alert(QMessageBox::Critical, "Attenzione", "I campi non possono essere vuoti!", QMessageBox::Ok, this);
        alert.exec();
        return;
    }
    else{
        Rivista* r = new Rivista(nome,acronimo,editore,data,volume);

        //se restituisce FALSE - rivista già presente
        if(!(g->aggiungiRivista(r))){
            QMessageBox alert2(QMessageBox::Critical, "Attenzione", "Rivista gia' presente in lista", QMessageBox::Ok, this);
            alert2.exec();
            return;
        }
    }

    //ripulisco i campi per un nuovo inserimento
    ui->lineEdit_rivista_nome->clear();
    ui->lineEdit_rivista_acronimo->clear();
    ui->lineEdit_rivista_editore->clear();
    ui->spinBox_rivista_volume->setValue(1);
    ui->calendarWidget_rivista->setSelectedDate(QDate::currentDate()); //reimposto la data selezionata

    ui->statusbar->showMessage("Rivista aggiunta", 3000);
}

void MainWindow::on_btn_aggiungiRivista_clicked()
{
    ui->calendarWidget_rivista->setSelectedDate(QDate::currentDate());
    ui->stackedWidget->setCurrentWidget(ui->page_aggiungiRivista);
}

void MainWindow::on_btn_visualizzaRiviste_clicked()
{
    //non prosegue se non c'è nessuna rivista
    if(g->getRiviste().isEmpty()){
            QMessageBox alert(QMessageBox::Critical, "Attenzione", "Non hai ancora aggiunto nessuna Rivista!", QMessageBox::Ok, this);
            alert.exec();
            return;
        }

    //mostra la lista di autori solo se ce ne sono
    if(g->getAutori().isEmpty()){
        ui->lista_showRiviste_tuttiAutori->hide();
        ui->label_showRiviste_tuttiAutori->hide();
    }
    else if(ui->lista_showRiviste_tuttiAutori->isHidden()){
        ui->lista_showRiviste_tuttiAutori->show();
        ui->label_showRiviste_tuttiAutori->show();
    }

    //ripulise i campi
    ui->lista_showRiviste->clear();
    ui->lista_showRiviste_tuttiAutori->clear();

    //mostra tutte le riviste
    QStringList riviste = g->listaRiviste();
    ui->lista_showRiviste->addItems(riviste);

    //mostra tutti gli autori
    QStringList autori = g->listaAutori();
    ui->lista_showRiviste_tuttiAutori->addItems(autori);
    ui->btn_showRiviste_tutteRiviste->hide();

    ui->stackedWidget->setCurrentWidget(ui->page_visualizzaRiviste);
}

void MainWindow::on_btn_articolo_aggiungiKeyword_clicked()
{
    QString keyword = ui->lineEdit_articolo_keyword->text();

    //il campo Keywords non può essere vuoto
    if(keyword.isEmpty()){
        QMessageBox alert(QMessageBox::Critical, "Attenzione", "Il campo non puo' essere vuoto!", QMessageBox::Ok, this);
        alert.exec();
        return;
    }

    //Controllo se la keyword è già presente nella lista che è in corso di creazione
    bool notFound = ui->lista_articolo_keywords->findItems(keyword,Qt::MatchExactly).isEmpty();
    if(notFound)
        ui->lista_articolo_keywords->addItem(keyword);
    else{
        QMessageBox alert2(QMessageBox::Critical, "Attenzione", "Keyword gia' inserita!", QMessageBox::Ok, this);
        alert2.exec();
        ui->lineEdit_articolo_keyword->clear();
        return;
    }

    ui->lineEdit_articolo_keyword->clear();
}

void MainWindow::on_btn_aggiungiArticolo_clicked()
{
    //non prosegue se non c'è nessun autore
    if(g->getAutori().isEmpty()){
            QMessageBox alert(QMessageBox::Critical, "Attenzione", "Devi prima aggiungere almeno un autore!", QMessageBox::Ok, this);
            alert.exec();
            return;
            }

    //ripulisco i campi
    ui->lista_articolo2_articoliScelti->clear();
    ui->lista_articolo2_autoriScelti->clear();
    ui->stackedWidget->setCurrentWidget(ui->page_aggiungiArticolo1);

    //Tutti gli Autori
    ui->lista_articolo2_tuttiAutori->clear();
    QStringList autori = g->listaAutori();
    ui->lista_articolo2_tuttiAutori->addItems(autori);

    //Tutti gli Articoli
    ui->lista_articolo2_tuttiArticoli->clear();
    QStringList articoli = g->listaArticoli();
    ui->lista_articolo2_tuttiArticoli->addItems(articoli);
    }

void MainWindow::on_btn_articolo_avanti_goto2_clicked()
{
    //Prendo i valori dai campi
    QString id = ui->lineEdit_articolo_id->text();
    QString titolo = ui->lineEdit_articolo_titolo->text();
    int numeroKeywords = ui->lista_articolo_keywords->count();

    //Se almeno UN (1) campo è vuoto
    if(id.isEmpty() || titolo.isEmpty() || numeroKeywords<=0){
        QMessageBox alert(QMessageBox::Critical, "Attenzione", "I campi non possono essere vuoti!", QMessageBox::Ok, this);
        alert.exec();
        return;
    }

    //NON prosegue se esiste già un articolo con lo stesso id
    if(g->idEsistente(id)){
        QMessageBox alert(QMessageBox::Critical, "Attenzione", "Id gia' esistente!", QMessageBox::Ok, this);
        alert.exec();
        return;
    }

    ui->stackedWidget->setCurrentWidget(ui->page_aggiungiArticolo2);
}

void MainWindow::on_btn_articolo_indietro_goto1_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_aggiungiArticolo1);
}

void MainWindow::on_lista_articolo2_tuttiAutori_itemDoubleClicked(QListWidgetItem *item)
{
   //Trasferisce un autore dall'elenco "Tutti Autori" in "Autori Scelti"
   ui->lista_articolo2_autoriScelti->addItem(item->clone());
   delete item;
}

void MainWindow::on_lista_articolo2_autoriScelti_itemDoubleClicked(QListWidgetItem *item)
{
    //Trasferisce un autore dall'elenco "Autori Scelti" in "Tutti Autori"
    ui->lista_articolo2_tuttiAutori->addItem(item->clone());
    delete item;
}

void MainWindow::on_lista_articolo2_tuttiArticoli_itemDoubleClicked(QListWidgetItem *item)
{
    //Trasferisce un articolo dall'elenco "Tutti Articoli" in "Articoli Correlati Scelti"
    ui->lista_articolo2_articoliScelti->addItem(item->clone());
    delete item;
}

void MainWindow::on_lista_articolo2_articoliScelti_itemDoubleClicked(QListWidgetItem *item)
{
    //Trasferisce un articolo dall'elenco "Articoli Correlati Scelti" in "Tutti Articoli"
    ui->lista_articolo2_tuttiArticoli->addItem(item->clone());
    delete item;
}

void MainWindow::on_btn_articolo_avanti_goto3_clicked()
{
    //per proseguire bisogna aver scelto almeno un (1) autore
    int numAutoriScelti = ui->lista_articolo2_autoriScelti->count();
    if(numAutoriScelti==0){
         QMessageBox alert(QMessageBox::Critical, "Attenzione", "Non hai scelto nessun autore!", QMessageBox::Ok, this);
         alert.exec();
         return;
       }

    //ripulisce i campi e mostra tutte le riviste
    ui->lista_articolo3->clear();
    QStringList riviste = g->listaRiviste();
    ui->lista_articolo3->addItems(riviste);

    ui->stackedWidget->setCurrentWidget(ui->page_aggiungiArticolo3);
}

void MainWindow::on_btn_articolo3_indietro_goto2_clicked()
{
    ui->radioButton_articolo3_rivista->setChecked(true); //metto di default il radioButton rivista
    ui->stackedWidget->setCurrentWidget(ui->page_aggiungiArticolo2);
}

void MainWindow::on_radioButton_articolo3_rivista_clicked()
{
    //mostro tutte le riviste
    ui->lista_articolo3->clear();
    QStringList riviste = g->listaRiviste();
    ui->lista_articolo3->addItems(riviste);
    ui->label_articolo3_lista->setText("Lista Riviste");
}

void MainWindow::on_radioButton_articolo3_conferenza_clicked()
{
    //mostro tutte le conferenze
    ui->lista_articolo3->clear();
    QStringList conferenze = g->listaConferenze();
    ui->lista_articolo3->addItems(conferenze);
    ui->label_articolo3_lista->setText("Lista Conferenze");
}

void MainWindow::on_btn_articolo_conferma_clicked()
{
    //controllo che la lista abbia elementi
    int numItems = ui->lista_articolo3->count();
    if(numItems==0){
        QMessageBox alert(QMessageBox::Critical, "Attenzione", "Non ci sono riviste o conferenze!", QMessageBox::Ok, this);
        alert.exec();
        return;
    }

    //Se non ci sono elementi selezionati -> Errore
    if(ui->lista_articolo3->selectedItems().size()==0){
        QMessageBox alert(QMessageBox::Critical, "Attenzione", "Devi selezionare dove pubblicare l'articolo!", QMessageBox::Ok, this);
        alert.exec();
        return;
    }

    //Prendo i valori dai campi
    QString id = ui->lineEdit_articolo_id->text();
    QString titolo = ui->lineEdit_articolo_titolo->text();
    int numeroPagine = ui->spinBox_articolo_numeroPagine->value();
    float prezzo = ui->doubleSpinBox_articolo_prezzo->value();
    int numeroKeywords = ui->lista_articolo_keywords->count();

    QStringList keywords;

    //prendo i valori dalla lista keywords
    for(int i = 0; i < numeroKeywords; i++)
         keywords << ui->lista_articolo_keywords->item(i)->text();

    int numAutoriScelti = ui->lista_articolo2_autoriScelti->count();
    QList<Autore*> tmp_listaAutori;

    //prendo i valori dalla lista autoriScelti
    for(int row = 0; row < numAutoriScelti; row++){
         QString autore = ui->lista_articolo2_autoriScelti->item(row)->text();
             for(auto it = g->getAutori().begin(); it != g->getAutori().end(); ++it){
                  int riga = autore.indexOf(" ;",0); //identifico dove sta il primo ;
                  autore = autore.left(riga); //prendo l'ID
                      if((*it)->getId()==autore){ //quando l'ID viene trovato, aggiungo l'autore alla lista temporanea
                          tmp_listaAutori.push_back(*it);
                          break;
                      }
                  }
    }

     Articolo* a = new Articolo(id,titolo,numeroPagine,tmp_listaAutori,keywords,prezzo);

    int numArticoliScelti = ui->lista_articolo2_articoliScelti->count();
    if(numArticoliScelti>0){
         QList<Articolo*> tmp_articoliScelti;

         //prendo i valori dalla lista articoliScelti
         for(int row = 0; row < numArticoliScelti; row++){
                 QString articolo = ui->lista_articolo2_articoliScelti->item(row)->text();
                     for(auto it = g->getArticoli().begin(); it != g->getArticoli().end(); ++it){
                          int riga = articolo.indexOf(" ;",0); //identifico dove sta il primo ;
                          articolo = articolo.left(riga); //prendo l'ID
                              if((*it)->getId()==articolo){ //quando l'ID viene trovato, aggiungo l'articolo alla lista temporanea
                                  tmp_articoliScelti.push_back(*it);
                                  break;
                              }
                          }
            }
               a->aggiungiArticoliCorrelati(tmp_articoliScelti);
    }

    //Pubblica Articolo in Rivista o Conferenza
    if(ui->radioButton_articolo3_rivista->isChecked()){
        int row = ui->lista_articolo3->currentRow();
        QList<Rivista*> riviste = g->getRiviste();
        riviste[row]->aggiungiArticolo(a);
    }
    else if(ui->radioButton_articolo3_conferenza->isChecked()){
        int row = ui->lista_articolo3->currentRow();
        QList<Conferenza*> conferenze = g->getConferenze();
        conferenze[row]->aggiungiArticolo(a);
    }

    g->aggiungiArticolo(a);

    //ripulisco i campi
    ui->lineEdit_articolo_id->clear();
    ui->lineEdit_articolo_titolo->clear();
    ui->lineEdit_articolo_keyword->clear();
    ui->spinBox_articolo_numeroPagine->setValue(1);
    ui->doubleSpinBox_articolo_prezzo->setValue(0.0);
    ui->lista_articolo_keywords->clear();
    ui->lista_articolo2_autoriScelti->clear();
    ui->lista_articolo2_articoliScelti->clear();
    ui->radioButton_articolo3_rivista->setChecked(true); //metto il radio Button rivista di default

    //Tutti gli Autori
    ui->lista_articolo2_tuttiAutori->clear();
    QStringList autori = g->listaAutori();
    ui->lista_articolo2_tuttiAutori->addItems(autori);

    //Tutti gli Articoli
    ui->lista_articolo2_tuttiArticoli->clear();
    QStringList articoli = g->listaArticoli();
    ui->lista_articolo2_tuttiArticoli->addItems(articoli);

    ui->stackedWidget->setCurrentWidget(ui->page_aggiungiArticolo1);

    ui->statusbar->showMessage("Articolo pubblicato con successo",3000);
}

void MainWindow::on_btn_visualizzaArticoli_clicked()
{
    //non prosegue se non ci sono articoli
    if(g->getArticoli().isEmpty()){
            QMessageBox alert(QMessageBox::Critical, "Attenzione", "Non hai ancora aggiunto nessun articolo!", QMessageBox::Ok, this);
            alert.exec();
            return;
            }

    ui->lista_showArticoli->clear();
    ui->stackedWidget->setCurrentWidget(ui->page_visualizzaArticoli);

    //Non mostra informazioni aggiuntive se non e' selezionato nessun articolo
    if(ui->lista_showArticoli->selectedItems().size()==0){
        ui->lista_showArticoli_secondaria->clear();
    }

    QStringList articoli = g->listaArticoli();
    ui->lista_showArticoli->addItems(articoli);
}

void MainWindow::on_lista_showArticoli_itemClicked(QListWidgetItem *item)
{
   QString vista = ui->comboBox_showArticoli_visualizza->currentText(); //prende il tipo di vista impostato
   int row = ui->lista_showArticoli->row(item); //prende la posizione dell'articolo scelto
    ui->lista_showArticoli_secondaria->clear();

    if(vista=="Keywords"){ //mostra le keywords dell'articolo
        ui->label_showArticoli_listaSecondaria->setText("Lista Keywords");
        QStringList keywords = g->listaKeywords(row);
        ui->lista_showArticoli_secondaria->addItems(keywords);
        }
    else if(vista=="Autori"){ //mostra gli autori dell'articolo
        ui->label_showArticoli_listaSecondaria->setText("Lista Autori dell'articolo");
        QStringList autori = g->listaAutoriInArticoli(row);
        ui->lista_showArticoli_secondaria->addItems(autori);
    }
    else if(vista=="Articoli Correlati"){ //mostra gli articoli correlati dell'articolo
        ui->label_showArticoli_listaSecondaria->setText("Lista Articoli Correlati");
        QStringList articoli = g->listaArticoliCorrelati(row);
        ui->lista_showArticoli_secondaria->addItems(articoli);
    }
}

void MainWindow::on_comboBox_showArticoli_visualizza_currentTextChanged(const QString &arg1)
{
    //se  c'è un elemento selezionato, mostra le informazione aggiuntive relative a "arg1"
    if(ui->lista_showArticoli->selectedItems().size()==1){
    int row = ui->lista_showArticoli->currentRow(); //prende la posizione dell'articolo
     ui->lista_showArticoli_secondaria->clear();

     if(arg1=="Keywords"){
         ui->label_showArticoli_listaSecondaria->setText("Lista Keywords");
         QStringList keywords = g->listaKeywords(row);
         ui->lista_showArticoli_secondaria->addItems(keywords);
         }
     else if(arg1=="Autori"){
         ui->label_showArticoli_listaSecondaria->setText("Lista Autori dell'articolo");
         QStringList autori = g->listaAutoriInArticoli(row);
         ui->lista_showArticoli_secondaria->addItems(autori);
     }
     else if(arg1=="Articoli Correlati"){
         ui->label_showArticoli_listaSecondaria->setText("Lista Articoli Correlati");
         QStringList articoli = g->listaArticoliCorrelati(row);
         ui->lista_showArticoli_secondaria->addItems(articoli);
     }
    }
}

void MainWindow::on_btn_filtraArticoli_back_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_visualizzaArticoli);
}

void MainWindow::on_btn_showArticoli_filtraArticoli_clicked()
{
    //ripulisco i campi
    ui->comboBox_filtraArticoli_mostra->setCurrentIndex(0);
    ui->lista_filtraArticoli_articoli->clear();
    ui->lista_filtraArticoli_listaMostrata->clear();
    ui->checkBox_filtraArticoli_ordinaPerPrezzo->setChecked(false);
    ui->label_filtraArticoli_MediaPrezzi->setText("");

    //mostro tutti gli autori
    QStringList autori = g->listaAutori();
    ui->lista_filtraArticoli_listaMostrata->addItems(autori);
    ui->label_filtraArticoli_titleLista_left->setText("Lista Autori");
    ui->label_filtraArticoli_header->setText("Premi su un autore per visualizzare tutti i suoi articoli.");

    ui->stackedWidget->setCurrentWidget(ui->page_filtraArticoli);
}

void MainWindow::on_comboBox_filtraArticoli_mostra_currentTextChanged(const QString &arg1)
{
    //ripulisco i campi
    ui->lista_filtraArticoli_articoli->clear();
    ui->lista_filtraArticoli_listaMostrata->clear();

    //nascondo le funzionalità che non sono presenti per tutti i tipi di elementi che poi verranno mostrate nel momento opportuno
    ui->btn_filtraArticoli_prezzoPiuBasso->hide();
    ui->btn_filtraArticoli_prezzoPiuAlto->hide();
    ui->btn_filtraArticoli_ordina->hide();

    if(arg1=="Autori"){
        ui->label_filtraArticoli_titleLista_left->setText("Lista Autori");
        ui->label_filtraArticoli_header->setText("Premi su un autore per visualizzare tutti i suoi articoli.");

        //mostra tutti gli autori
        QStringList autori = g->listaAutori();
        ui->lista_filtraArticoli_listaMostrata->addItems(autori);

        //mostra le funzionalità aggiuntive disponibili per "Autore"
        ui->label_filtraArticoli_MediaPrezzi->show();
        ui->btn_filtraArticoli_prezzoPiuBasso->show();
        ui->btn_filtraArticoli_prezzoPiuAlto->show();
        ui->btn_filtraArticoli_ordina->show();

        }
    else if(arg1=="Conferenze"){
        ui->label_filtraArticoli_titleLista_left->setText("Lista Conferenze");
        ui->label_filtraArticoli_header->setText("Premi su una conferenza per visualizzare tutti i suoi articoli.");

        //mostra tutte le conferenze
        QStringList conferenze = g->listaConferenze();
        ui->lista_filtraArticoli_listaMostrata->addItems(conferenze);
        ui->label_filtraArticoli_MediaPrezzi->hide();
    }
    else if(arg1=="Riviste"){
        ui->label_filtraArticoli_titleLista_left->setText("Lista Riviste");
        ui->label_filtraArticoli_header->setText("Premi su una rivista per visualizzare tutti i suoi articoli.");

        //mostra tutte le riviste
        QStringList riviste = g->listaRiviste();
        ui->lista_filtraArticoli_listaMostrata->addItems(riviste);
        ui->label_filtraArticoli_MediaPrezzi->hide();
    }
}

void MainWindow::on_lista_filtraArticoli_listaMostrata_itemClicked(QListWidgetItem *item)
{
    int row = ui->lista_filtraArticoli_listaMostrata->row(item); //prendo la posizione dell'articolo scelto
    int index = ui->comboBox_filtraArticoli_mostra->currentIndex(); //0 (Autori), 1 (Conferenze), 2(Riviste)
    ui->lista_filtraArticoli_articoli->clear();

    QStringList articoli;

    if(ui->checkBox_filtraArticoli_ordinaPerPrezzo->isChecked())
        articoli = g->articoliOrdinatiPerPrezzo(row,index);
    else{
    switch(index){
        case 0: articoli = g->listaArticoliDiAutore(row); break;
        case 1: articoli = g->listaArticoliDiConferenza(row); break;
        case 2: articoli = g->listaArticoliDiRivista(row); break;
        default: break;
        }
    }

    //mostra il prezzo medio di tutti gli articoli di un autore (solo se in vista "autori")
    if(index==0){
        QString mediaPrezzo = "Prezzo Medio: " + QString::number(g->mediaPrezziArticoliAutore(row));
        ui->label_filtraArticoli_MediaPrezzi->setText(mediaPrezzo);
    }
    else{
        ui->label_filtraArticoli_MediaPrezzi->clear();
    }

    ui->lista_filtraArticoli_articoli->addItems(articoli);
}

void MainWindow::on_checkBox_filtraArticoli_ordinaPerPrezzo_stateChanged(int arg1)
{
    //se non è stato selezionato nessun articolo
    if(ui->lista_filtraArticoli_listaMostrata->selectedItems().size()==0)
        return;

    ui->lista_filtraArticoli_articoli->clear();
    int row = ui->lista_filtraArticoli_listaMostrata->currentRow(); //prende la posizione dell'elemento scelto

    QStringList articoli;
    int index = ui->comboBox_filtraArticoli_mostra->currentIndex();

    if(arg1==2) //arg1 -> 0 (Unchecked), 2(Checked)
        articoli = g->articoliOrdinatiPerPrezzo(row,index); //mostra la lista con gli articoli ordinati per prezzo
    else{
        switch(index){ //mostra la lista degli elementi, nell'ordine in cui sono stati inseriti
            case 0: articoli = g->listaArticoliDiAutore(row);break;
            case 1: articoli = g->listaArticoliDiConferenza(row); break;
            case 2: articoli = g->listaArticoliDiRivista(row); break;
            default: break;
            }
    }

    ui->lista_filtraArticoli_articoli->addItems(articoli);
}


void MainWindow::on_btn_filtraArticoli_prezzoPiuBasso_clicked()
{
    if(ui->lista_filtraArticoli_listaMostrata->selectedItems().size()==0){
        QMessageBox alert(QMessageBox::Critical, "Attenzione", "Devi prima selezionare un autore!", QMessageBox::Ok, this);
        alert.exec();
        return;
    }

    int row = ui->lista_filtraArticoli_listaMostrata->currentRow();

    ui->checkBox_filtraArticoli_ordinaPerPrezzo->setChecked(false);
    QStringList articoli = g->prezzoPiuBassoArticoliAutore(row);

    ui->lista_filtraArticoli_articoli->clear();
    ui->label_filtraArticoli_MediaPrezzi->setText("");
    ui->lista_filtraArticoli_articoli->addItems(articoli);
    ui->statusbar->showMessage("Successo! Se scegli un altro autore, devi premere di nuovo il pulsante.",3000);
}

void MainWindow::on_btn_filtraArticoli_prezzoPiuAlto_clicked()
{
    //dev'essere selezionato un autore
    if(ui->lista_filtraArticoli_listaMostrata->selectedItems().size()==0){
        QMessageBox alert(QMessageBox::Critical, "Attenzione", "Devi prima selezionare un autore!", QMessageBox::Ok, this);
        alert.exec();
        return;
    }

    int row = ui->lista_filtraArticoli_listaMostrata->currentRow();

    ui->checkBox_filtraArticoli_ordinaPerPrezzo->setChecked(false);
    QStringList articoli = g->prezzoPiuAltoArticoliAutore(row);

    ui->lista_filtraArticoli_articoli->clear();
    ui->label_filtraArticoli_MediaPrezzi->setText("");
    ui->lista_filtraArticoli_articoli->addItems(articoli);
    ui->statusbar->showMessage("Successo! Se scegli un altro autore, devi premere di nuovo il pulsante.",3000);
}

void MainWindow::on_lista_showRiviste_tuttiAutori_itemClicked(QListWidgetItem *item)
{
    ui->lista_showRiviste->clear();
    ui->btn_showRiviste_tutteRiviste->show();

    int row = ui->lista_showRiviste_tuttiAutori->row(item); //prendo la posizione dell'autore scelto
    QStringList riviste = g->listaRivisteDiAutoreNoArticolo(row);
    ui->lista_showRiviste->addItems(riviste);
    ui->label_showRiviste_listaRiviste->setText("Lista Riviste");
    ui->statusbar->showMessage("Successo!",1000);

}

void MainWindow::on_btn_showRiviste_tutteRiviste_clicked()
{
    ui->lista_showRiviste->clear();
    QStringList riviste = g->listaRiviste();
    ui->lista_showRiviste->addItems(riviste);
    ui->lista_showRiviste_tuttiAutori->clearSelection();
    ui->btn_showRiviste_tutteRiviste->hide();
    ui->label_showRiviste_listaRiviste->setText("Lista Riviste");
    ui->statusbar->showMessage("Stai visualizzando tutte le riviste di nuovo!",3000);
}

void MainWindow::on_btn_filtraArticoli_ordina_clicked()
{
    if(ui->lista_filtraArticoli_listaMostrata->selectedItems().size()==0){
        QMessageBox alert(QMessageBox::Critical, "Attenzione", "Devi prima selezionare un autore!", QMessageBox::Ok, this);
        alert.exec();
        return;
    }

    ui->lista_filtraArticoli_articoli->clear();
    int row = ui->lista_filtraArticoli_listaMostrata->currentRow();
    QStringList articoli = g->ordinaArticoliDiAutore(row);
    ui->lista_filtraArticoli_articoli->addItems(articoli);
    ui->checkBox_filtraArticoli_ordinaPerPrezzo->setChecked(false);

    ui->statusbar->showMessage("Successo! Se scegli un altro autore, devi premere di nuovo il pulsante.",3000);
}

void MainWindow::on_btn_showConferenze_mostraSimili_clicked()
{
    if(ui->lista_showConferenze->selectedItems().size()==0){
            QMessageBox alert(QMessageBox::Critical, "Attenzione", "Devi prima selezionare una conferenza!", QMessageBox::Ok, this);
            alert.exec();
            return;
        }

    if(ui->lista_showConferenze_simili->isHidden())
        ui->lista_showConferenze_simili->show();

    ui->statusbar->showMessage("Successo!",3000);

}


void MainWindow::on_btn_apriFile_clicked()
{
    /* ORGANIZZAZIONE DEL FILE
    * I dati sono organizzati su un file .txt che contine tutti i tipi di elementi.
    * Ogni riga rappresenta un elemento diverso. Il tipo di elemento è specificato
    * all'inizio della riga. Ogni attributo è diviso da un separatore.
    *
    * TIPI: 1=Autore, 2=Conferenza, 3=Rivista, 4=Articolo
    * SEPARATORI USATI: ";" "," "|" (punto e virgola, virgola, barra)
    *
    * L'ordine degli attributi verrà spigato di seguito.
    * N.B. Lo spazio fra i separatori NON esiste nel file .txt
    * Nel commento corrente c'è SOLO per facilitare la comprensione.
    *
    * =======================================================================
    * AUTORE
    * 1 ; ID ; Nome ; Cognome ; Afferenza_1 , Affeerenza_2 , Afferenza_n
    * -----------------------------------------------------------------------
    * CONFERENZA
    * 2 ; Nome ; Acronimo ; Luogo ; GIORNO/MESE/ANNO ; Organizzatore_1 , Organizzatore_2 , Organizzatore_N ; Numero_Partecipanti
    * -----------------------------------------------------------------------
    * RIVISTA
    * 3 ; Nome ; Acronimo ; Edicola ; GIORNO/MESE/ANNO ; Volume
    * -----------------------------------------------------------------------
    * ARTICOLO (pubblicato in conferenza)
    * 4 ; ID ; Titolo ; Prezzo ; ID_Autore_1 , ID_Autore_2 , ID_Autore_n ; keyword_1 , keyword_2 , keyword_n ; Prezzo ; ArticoloCorrelato_1 , ArticoloCorrelato_2 , Articolo_Correlato_b ; 2 ; Nome | Acronimo | Luogo | GIORNO/MESE/ANNO | Organizzatore_1 , Organizzatore_2 , Organizzatore_N | Numero_Partecipanti
    * -----------------------------------------------------------------------
    * ARTICOLO (pubblicato in rivista)
    * 4 ; ID ; Titolo ; Prezzo ; ID_Autore_1 , ID_Autore_2 , ID_Autore_n ; keyword_1 , keyword_2 , keyword_n ; Prezzo ; ArticoloCorrelato_1 , ArticoloCorrelato_2 , Articolo_Correlato_b ; 3 ; Nome | Acronimo | Edicola | GIORNO/MESE/ANNO | Volume
    * =======================================================================
    *
    * Per specificare la Rivista o la Conferenza alla quale appartiene l'articolo, bisogna
    * includere tutti i campi di Rivista/Conferenza soddivisi da "|" e non più da ";"
    * Attributi multipli come Afferenze, Organizzatori, Keyword, ID_Autori, ID_ArticoliCorrelati
    * sono suddivisi da ";" (virgola).
    *
    * GLi articoli correlati non sono obbligatori. Se si vogliono omettere sarà necessario
    * lasciare il campo vuoto. Ciò comporterebbe l'esistenza di due separatori ";" uno accanto all'altro -> ";;"
    * Un comportamento normale.
    *
    * Per specificare la rivista/conferenza nella quale è pubblicato l'articolo, non basta solo il nome.
    * Per il semplice motivo che possono esistere più riviste/conferenze con lo stesso nome.
    */

    //Si apre la finestra del File Explorer/Finder preimpostato alla ricerca di elementi.txt
    QString path = QFileDialog::getOpenFileName(this,tr("Open File"),"elementi.txt");
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
        return;
    }

    QMessageBox::information(this,tr("File Name"),path);

    int elementi_totali_prima = g->getArticoli().size() + g->getAutori().size() + g->getRiviste().size() + g->getConferenze().size();

    QTextStream in(&file);
    QStringList autori;
    int row=0; //count righe
    while(!in.atEnd()) {
        row++;
        QString line = in.readLine();
        QStringList righe = line.split("\n");
        autori+=righe;

        QStringList colonne;

        for(int i=0; i<righe.size(); i++){
            QString colonna = righe.at(i);
            colonne+= colonna.split(";");
        }

        QString tipo = colonne.at(0); //la tipologia di elemento (1=autore, 2=conferenza, 3=rivista, 4=articolo)
        if(tipo=="1"){ //autore
            QString id = colonne.at(1);
            QString nome = colonne.at(2);
            QString cognome = colonne.at(3);
            QStringList afferenze = colonne.at(4).split(",");

            if(colonne.size()!=5){
                erroreImportazione(row);
                return;
            }

            Autore* a = new Autore(id,nome,cognome,afferenze);
            g->aggiungiAutore(a);
        }
        else if(tipo=="2"){ //conferenza
            QString nome = colonne.at(1);
            QString acronimo = colonne.at(2);
            QString luogo = colonne.at(3);
            QString data_string = colonne.at(4);
            QDate data = QDate::fromString(data_string.right(10), "dd/MM/yyyy");
            QStringList organizzatori = colonne.at(5).split(",");
            int numPartecipanti = colonne.at(6).toInt();

            if(colonne.size()!=7){
                erroreImportazione(row);
                return;
            }

            Conferenza* c = new Conferenza(nome,acronimo,luogo,data,organizzatori,numPartecipanti);
            g->aggiungiConferenza(c);
        }
        else if(tipo=="3"){ //rivista
            QString nome = colonne.at(1);
            QString acronimo = colonne.at(2);
            QString editore = colonne.at(3);
            QString data_string = colonne.at(4);
            QDate data = QDate::fromString(data_string.right(10), "dd/MM/yyyy");
            int volume = colonne.at(5).toInt();

            if(colonne.size()!=6){
                erroreImportazione(row);
                return;
            }

            Rivista* r = new Rivista(nome,acronimo,editore,data,volume);
            g->aggiungiRivista(r);
        }
        else if(tipo=="4"){ //articolo
            QString id_articolo = colonne.at(1);
            QString titolo = colonne.at(2);
            int numPagine = colonne.at(3).toInt();
            QStringList autori = colonne.at(4).split(",");
            QStringList keywords = colonne.at(5).split(",");
            float prezzo = colonne.at(6).toFloat();
            QStringList articoli_correlati = colonne.at(7).split(",");

            if(colonne.size()!=10){
                erroreImportazione(row);
                return;
            }

            Articolo* a = new Articolo(id_articolo,titolo,numPagine,keywords,prezzo);
            g->aggiungiArticolo(a);
            g->aggiungiAutoriInArticolo(a,autori);
            if(articoli_correlati.size()>0){
                g->aggiungiArticoliCorrelati(a,articoli_correlati);
            }

            if(colonne.at(8)=="2"){
                QStringList conferenza = colonne.at(9).split("|");

                if(conferenza.size()!=6){
                    erroreImportazione(row);
                    return;
                }

                g->pubblicaInConferenza(a,leggiConferenza(conferenza));
            }
            else if(colonne.at(8)=="3"){
                QStringList rivista = colonne.at(9).split("|");

                if(rivista.size()!=5){
                    erroreImportazione(row);
                    return;
                }

                g->pubblicaInRivista(a,leggiRivista(rivista));
            }

        }
        else{
                erroreImportazione(row);
                return;
        }

      }

    int elementi_totali_dopo = g->getArticoli().size() + g->getAutori().size() + g->getRiviste().size() + g->getConferenze().size();

    file.close();

    if((elementi_totali_dopo-elementi_totali_prima)!=row){
        QMessageBox alert(QMessageBox::Information, "Attenzione", "Il file ha degli elementi che sono già presenti. Tranquillo, non sono stati inseriti i duplicati!", QMessageBox::Ok, this);
        alert.exec();
        return;
    }

    ui->statusbar->showMessage("Importazione avvenuta con successo",3000);
}

void MainWindow::on_btn_showRiviste_rivisteSpecialistiche_clicked()
{
    ui->lista_showRiviste->clear();
    ui->lista_showRiviste->addItems(g->listaRivisteSpecialistiche());
    ui->label_showRiviste_listaRiviste->setText("Lista Riviste Specialistiche");
    ui->btn_showRiviste_tutteRiviste->show();
}

void MainWindow::on_btn_keywords_back_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_visualizzaArticoli);
}

void MainWindow::on_btn_showArticoli_keywordsGuadagnoAlto_clicked()
{
    ui->lista_keywords_guadagnoAlto->clear();
    ui->lista_keywords_guadagnoAlto->addItems(g->listaKeywordsGuadagno());
    ui->stackedWidget->setCurrentWidget(ui->page_keywords_guadagnoAlto);
}

void MainWindow::on_btn_help_clicked()
{
    //se si trova già sulla pagina "help", il pulsante rimanderà alla pagina precedente
    if(ui->stackedWidget->currentWidget()==ui->page_help){
        ui->stackedWidget->setCurrentIndex(precedente);
        if(precedente==0){ //se si trova sulla home, il pulsante home viene disabilitato
            ui->btn_home->setDisabled(true);
        }
    }
    else{
        //vado sulla pagina "help"
        ui->stackedWidget->setCurrentWidget(ui->page_help);

         QString testo;

         //da informazioni aggiuntive in base alla pagina precedente
         switch(precedente){
            case 0:{testo = "E' consigliabile importare il file di testo chiamato \"elementi.txt\"\n"
                            "tramite il tasto \"Importa\". Se si tenta di importare un file che non segue\n"
                            "le regole di organizzazione descritte nel apposito metodo, l'operazione sarà impedita.\n\n"
                            "Le operazioni delle sezioni B, C, D, E, F si trovano nelle pagine:\n"
                            "\"Visualizza Conferenze\" - \"Visualizza Riviste\" e \"Visualizza Articoli\""; break;}
            case 1: testo = "Sezione A -> Operazione 1"; break;
            case 2: testo = "Sezione A -> Operazione 5"; break;
            case 3: testo = "Sezione A -> Operazione 2"; break;
            case 4: testo = "Sezione A -> Operazione 6\nSezione F -> Operazione 5"; break;
            case 5: testo = "Sezione A -> Operazione 3"; break;
            case 6: testo = "Sezione A -> Operazioni 5, 7\nSezione E -> Operazioni 2, 6"; break;
            case 7:{testo = "Sezione A -> Operazione 4\n\nAssicurati di riempire tutti i campi\n"
                            "Inoltre puoi inserire la stessa keyword solo una volta."; break;}
            case 8:{testo = "Sezione A -> Operazioni 4, 5, 8\n\nDevi premere due volte sull'autore/articolo per inserirlo.\n"
                            "Se hai sbagliato, puoi eliminarlo dalla lista dei SCELTI\n"
                            "facendo doppio click.\nInoltre non è obbligatorio scegliere articoli correlati."; break;}
            case 9:{testo = "Sezione A -> Operazioni 4, 6, 7\n\nStai visualizzando tutte le riviste e conferenze.\n"
                            "Semplicemente premi su un elemento e prosegui.\n"
                            "Ricorda che puoi tornare indietro se desideri apportare\n"
                            "modifiche prima di confermare."; break;}
            case 10:{testo ="Sezione A -> Operazione 8\nSezione C -> Operazione 6\n"
                            "Sezione F -> Operazione 1\n\n"
                            "Per vedere altre operazioni premi su \"filtra articoli\" nella pagina precedente"; break;}
            case 11:{testo ="La pagina FILTRA ARTICOLI contiene le seguenti operazioni:\n"
                            "Sezione A -> Operazione 5\nSezione B -> Operazioni 1, 4, 5\n"
                            "Sezione C -> Operazioni 1, 2, 3\nSezione D -> Operazioni 2, 3, 4, 6\n\n"; break;}
            case 12: testo ="Sezione C -> Operazione 6"; break;
            case 13:{testo ="Sezione F -> Operazione 1\n\n"
                            "Stai visualizzando tutti gli articoli influenzati dall'articolo\n"
                            "che hai scelto nella pagine precedente.\n"; break;}
            default: break;
         }
        ui->plainTextEdit_help->setPlainText(testo);
    }
}

void MainWindow::on_btn_help_tuttiMetodi_clicked()
{
    ui->plainTextEdit_help->clear();
    QString testo =
    "SEZIONE A: 1 - 2 - 3 - 4 - 5 - 6 - 7 - 8\n\n"
    "SEZIONE B: 1 - 4 - 5\n\n"
    "SEZIONE C: 1 - 2 - 3 - 6\n\n"
    "SEZIONE D: 2 - 3 - 4 - 6\n\n"
    "SEZIONE E: 2 -  6\n\n"
    "SEZIONE F: 1 - 4 - 5";

    ui->plainTextEdit_help->setPlainText(testo);
}

void MainWindow::on_btn_showArticoli_articoliInfluenzati_clicked()
{
    //dev'esserci un articolo selezionato
    if(ui->lista_showArticoli->selectedItems().size()==0){
        QMessageBox alert(QMessageBox::Information, "Attenzione", "Non hai selezionato nessun articolo.", QMessageBox::Ok, this);
        alert.exec();
        return;
    }

    int row = ui->lista_showArticoli->currentRow();
    ui->lista_articoli_Influenzati->clear();
    QStringList articoli_influenzati = g->listaArticoliInfluenzati(row);
    ui->lista_articoli_Influenzati->addItems(articoli_influenzati);

    ui->stackedWidget->setCurrentWidget(ui->page_articoli_Influenzati);
}

void MainWindow::on_btn_articoli_Influenzati_back_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_visualizzaArticoli);
}
