#ifndef RIVISTA_H
#define RIVISTA_H

#include <QDate>
#include <QString>
#include "Articolo.h"
#include "Elemento.h"
using namespace std;

class Rivista{
    public:
        Rivista();
        Rivista(QString _nome, QString _acronimo, QString editore, QDate _data, int _volume);
        ~Rivista();

        void setNome(QString n);
        QString getNome() const;

        void setAcronimo(QString a);
        QString getAcronimo() const;

        void setEditore(QString e);
        QString getEditore() const;

        void setData(QDate d);
        QDate getData() const;

        void setVolume(int v);
        int getVolume() const;

        bool autorePresente(QString id); //verifica se un autore è presente a partire dall'id di un autore
        bool articoloPresente(QString id); //verifica se un articolo è presente a partire dall'id dell'articolo

        void aggiungiArticolo(Articolo* a);
        QList<Articolo*> getArticoli() const;
        QStringList getListaArticoli(); //restituisce gli articoli della rvisita come una StringList

        QString toString() const; //restituisce la Rivista come una stinga

        bool operator==(const Rivista& c) const;

    private:
        QString nome;
        QString acronimo;
        QString editore;
        QDate data;
        int volume;
        QList<Articolo*> articoli; //articoli pubblicati per la rivista
};

#endif // RIVISTA_H
