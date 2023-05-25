#ifndef ARTICOLO_H
#define ARTICOLO_H

#include <QString>
#include "Autore.h"
#include <QList>
#include <vector>

class Articolo{
    public:
        Articolo();
        //sono presenti più costruttori per facilitare la creazione dell'oggetto senza la necessità di usare i metodi "set"
        Articolo(QString _id, QString _titolo, int _numeroPagine, QStringList _keywords, float _prezzo);
        Articolo(QString _id, QString _titolo, int _numeroPagine, QList<Autore*> _autori, QStringList _keywords, float _prezzo);
        ~Articolo();

        void setId(QString _id);
        QString getId() const;

        void setTitolo(QString _titolo);
        QString getTitolo() const;

        void setNumeroPagine(int _numero);
        int getNumeroPagine() const;

        bool aggiungiAutore(Autore* a); //True = aggiunta con successo, False = Autore già presente
        QList<Autore*> getAutori() const;

        void aggiungiKeyword(QString _keyword); //aggiunge singola keyword
        void setKeywords(QStringList _keywords); //aggiunge più keywrods da una lista
        QStringList getKeywords() const;

        void aggiungiArticoloCorrelato(Articolo* a);
        void aggiungiArticoliCorrelati(QList<Articolo*> a);
        QList<Articolo*> getArticoliCorrelati() const;

        void setPrezzo(float _prezzo);
        float getPrezzo() const;

        QString toString() const; //restituisce l'articolo sottoforma di stringa

        bool operator==(const Articolo& a) const;
        Articolo& operator=(const Articolo&);
        QString operator[](const int& i) const; //restituisce la keyword in posizione i

    private:
        QString id;
        QString titolo;
        int numeroPagine;
        QList<Autore*> autori;
        QStringList keywords;
        float prezzo;
        QList<Articolo*> articoliCorrelati;
};

#endif // ARTICOLO_H
