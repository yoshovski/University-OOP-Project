#ifndef GESTORE_H
#define GESTORE_H

#include "Articolo.h"
#include "Autore.h"
#include "Conferenza.h"
#include "Rivista.h"
#include <QList>
#include <algorithm>
#include "Elemento.h"

class Gestore{
    public:
       Gestore();
       ~Gestore();
       //AUTORE
       bool aggiungiAutore(Autore* a); //SEZIONE A(1) True = aggiunta con successo, False = Autore già presente
       const QList<Autore*>& getAutori() const;
       const Autore* getAutore(int i) const;
       QStringList listaAfferenze(int row) const;
       QStringList listaAutori(); //SEZIONE A(5)
       QStringList listaAutoriInArticoli(int row) const;

       //CONFERENZA
       bool aggiungiConferenza(Conferenza* c); //SEZIONE A(2) True = aggiunta con successo, False = Conferenza già presente
       const QList<Conferenza*>& getConferenze() const;
       QStringList listaOrganizzatori(int row) const;
       QStringList listaConferenze(); //SEZIONE A(6)
       QStringList listaConferenze(QList<Conferenza*> c) const;
       QStringList listaConferenzeSimili(int row) const; //SEZIONE F(5) restituisce la lista delle conferenze simili della conferenza in posizione row.

       //RIVISTA
       bool aggiungiRivista(Rivista* r); //SEZIONE A(3) True = aggiunta con successo, False = Rivista già presente
       const QList<Rivista*>& getRiviste() const;
       QStringList listaRiviste(); //SEZIONE A(7)
       QStringList listaRiviste(QList<Rivista*> riv) const;
       QStringList listaRivisteDiAutoreNoArticolo(int row) const; //SEZIONE E(2) Visualizza le riviste in cui un autore non ha mai pubblicato un articolo
       QStringList listaRivisteSpecialistiche() const; //SEZIONE E(6)

       //ARTICOLO
       bool aggiungiArticolo(Articolo* a); //SEZIONE A(4) True = aggiunta con successo, False = Rivista già presente
       const QList<Articolo*>& getArticoli() const;
       void aggiungiAutoriInArticolo(Articolo* a, QStringList id_autori); //aggiunge gli autori con gli id della lista nell'articolo specificato
       void aggiungiArticoliCorrelati(Articolo* a, QStringList id_articoli); //aggiungi articoli correlati dell'articolo "a" avendo solo gli id.
       bool idEsistente(QString _id); //True = id presente, False = id NON presente

       QStringList listaArticoli(); //SEZIONE A(8) restituisce la lista degli articoli come QStringList
       QStringList listaArticoli(QList<Articolo*> a); //converte la lista d'articoli messa come parametro, in Lista QStringList
       QStringList listaArticoliCorrelati(int row) const;
       QStringList listaArticoliDiRivista(int row); //SEZIONE B(5)
       QStringList listaArticoliDiConferenza(int row); //SEZIONE B(4)
       QStringList listaArticoliDiAutore(int row); //SEZIONE B(1)
       QList<Articolo*> getArticoliDiAutore(int row);


       void pubblicaInConferenza(Articolo* a,Conferenza* c);
       void pubblicaInRivista(Articolo* a,Rivista* r);
       bool static comparePrezzoBasso(const Articolo* a1, const Articolo* a2); //compare Prezzo Basso
       QStringList articoliOrdinatiPerPrezzo(int row, int tipo); //SEZIONE D(2,3,4) row -> posizione dell'elemento; il tipo può essere 0 = autore, 1=conferenza, 2=rivista
       bool static comparePrezzoAlto(const Articolo* a1, const Articolo* a2); //compare Prezzo Alto
       float mediaPrezziArticoliAutore(int row); //SEZIONE C(3)
       QStringList prezzoPiuBassoArticoliAutore(int row); //SEZIONE C(1)
       QStringList prezzoPiuAltoArticoliAutore(int row); //SEZIONE C(2)
       bool static follow(const Articolo* a1, const Articolo* a2); //Sezione D(6) confronta Prezzo e Prima Keyword fra due articoli
       QStringList ordinaArticoliDiAutore(int row); //SEZIONE D(6)
       int getAnno(Articolo* a) const;
       QDate getData(Articolo* a) const;

        QStringList listaArticoliInfluenzati(int row); //SEZIONE F(1) restituisce la lista di tutti gli articoli influenzati da articolo in posizone "row"
        bool influenza(Articolo* a, Articolo* b); //verifica se "articolo_a" influenza "articolo_b"
        bool esiste(QList<Articolo*> lista_articoli, Articolo* a); //controlla se l'articolo "a" esiste nella lista di articoli passata

       QStringList listaKeywords(int row) const;
       QStringList listaKeywordsGuadagno() const; //SEZIONE C(6)

    private:
       QList<Autore*> autori;
       QList<Conferenza*> conferenze;
       QList<Rivista*> riviste;
       QList<Articolo*> articoli;
};

#endif // GESTORE_H
