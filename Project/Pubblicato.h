#ifndef PUBBLICATO_H
#define PUBBLICATO_H

#include <QString>
#include <QDate>
#include "Articolo.h"
using namespace std;

class Pubblicato{
    public:
        Pubblicato();
        Pubblicato(QString _nome, QString _acronimo, QDate _data);

        void setNome(QString n);
        QString getNome() const;

        void setAcronimo(QString a);
        QString getAcronimo() const;

        void setData(QDate d);
        QDate getData() const;

        QList<Articolo*> getArticoli() const;



    private:
        QString nome;
        QString acronimo;
        QDate data;
};

#endif // PUBBLICATO_H
