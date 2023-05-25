#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "Gestore.h"
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_btn_aggiungiAutore_clicked();

    void on_btn_autore_conferma_clicked();

    void on_btn_autore_aggiungiAfferenza_clicked();

    void on_btn_visualizzaAutori_clicked();

    void on_btn_home_clicked();

    void on_stackedWidget_currentChanged(int arg1);

    void on_lista_showAutori_itemClicked(QListWidgetItem *item);

    void on_btn_conferenza_aggiungiOrganizatore_clicked();

    void on_btn_aggiungiConferenza_clicked();

    void on_btn_conferenza_conferma_clicked();

    void on_btn_visualizzaConferenze_clicked();

    void on_lista_showConferenze_itemClicked(QListWidgetItem *item);

    void on_btn_rivista_conferma_clicked();

    void on_btn_aggiungiRivista_clicked();

    void on_btn_visualizzaRiviste_clicked();

    void on_btn_articolo_aggiungiKeyword_clicked();

    void on_btn_aggiungiArticolo_clicked();

    void on_btn_articolo_avanti_goto2_clicked();

    void on_btn_articolo_indietro_goto1_clicked();

    void on_lista_articolo2_tuttiAutori_itemDoubleClicked(QListWidgetItem *item);

    void on_lista_articolo2_autoriScelti_itemDoubleClicked(QListWidgetItem *item);

    void on_lista_articolo2_tuttiArticoli_itemDoubleClicked(QListWidgetItem *item);

    void on_lista_articolo2_articoliScelti_itemDoubleClicked(QListWidgetItem *item);

    void on_btn_articolo_avanti_goto3_clicked();

    void on_btn_articolo3_indietro_goto2_clicked();

    void on_radioButton_articolo3_rivista_clicked();

    void on_radioButton_articolo3_conferenza_clicked();

    void on_btn_articolo_conferma_clicked();

    void on_btn_visualizzaArticoli_clicked();

    void on_lista_showArticoli_itemClicked(QListWidgetItem *item);

    void on_comboBox_showArticoli_visualizza_currentTextChanged(const QString &arg1);

    void on_btn_filtraArticoli_back_clicked();

    void on_btn_showArticoli_filtraArticoli_clicked();

    void on_comboBox_filtraArticoli_mostra_currentTextChanged(const QString &arg1);

    void on_lista_filtraArticoli_listaMostrata_itemClicked(QListWidgetItem *item);

    void on_checkBox_filtraArticoli_ordinaPerPrezzo_stateChanged(int arg1);

    void on_btn_filtraArticoli_prezzoPiuBasso_clicked();

    void on_btn_filtraArticoli_prezzoPiuAlto_clicked();

    void on_lista_showRiviste_tuttiAutori_itemClicked(QListWidgetItem *item);

    void on_btn_showRiviste_tutteRiviste_clicked();

    void on_btn_filtraArticoli_ordina_clicked();

    void on_btn_showConferenze_mostraSimili_clicked();

    void on_btn_apriFile_clicked();

    void on_btn_showRiviste_rivisteSpecialistiche_clicked();

    void on_btn_keywords_back_clicked();

    void on_btn_showArticoli_keywordsGuadagnoAlto_clicked();

    void on_btn_help_clicked();

    void on_btn_help_tuttiMetodi_clicked();

    void on_btn_showArticoli_articoliInfluenzati_clicked();

    void on_btn_articoli_Influenzati_back_clicked();

private:
    Ui::MainWindow *ui;
    Gestore* g = new Gestore();
    Conferenza* leggiConferenza(QStringList colonne);
    Rivista* leggiRivista(QStringList colonne);
    void erroreImportazione(int row);
    int precedente=0;

};
#endif // MAINWINDOW_H
