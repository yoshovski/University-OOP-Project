#ifndef CONFERENZA_H
#define CONFERENZA_H

#include <QDate>
#include <QString>
#include "Articolo.h"
#include "Elemento.h"
using namespace std;

class Conferenza{
    public:
        Conferenza();
        Conferenza(QString _nome, QString _acronimo, QString _luogo, QDate _data, QStringList _organizzatori, int _numeroPartecipanti);
        ~Conferenza();

        void setNome(QString n);
        QString getNome() const;

        void setAcronimo(QString a);
        QString getAcronimo() const;

        void setLuogo(QString l);
        QString getLuogo() const;

        void setData(QDate d);
        QDate getData() const;

        void aggiungiOrganizzatore(QString o); //aggiunge singolo organizzatore
        void setOrganizzatori(QStringList o); //aggiunge più organizzatori da una lista
        QStringList getOrganizzatori() const;

        void setNumPartecipanti(int n);
        int getNumPartecipanti() const;

        void aggiungiArticolo(Articolo* a);
        QList<Articolo*> getArticoli() const;
        QStringList getListaArticoli(); //restituisce gli articoli della conferenza come una StringList

        bool articoloPresente(QString id); //verifica se un articolo è presente in conferenza a partire dall'id di un articolo

        QString toString(); //restituisce la conferenza sottoforma di stringa

        QString operator[](const int& i) const; //restituisce organizzatore in posizione i
        bool operator==(const Conferenza& c) const;

    private:
        QString nome;
        QString acronimo;
        QString luogo;
        QDate data;
        QStringList organizzatori;
        int numeroPartecipanti;
        QList <Articolo*> articoli; //articoli pubblicati per la conferenza
};

#endif // CONFERENZA_H
