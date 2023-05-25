#ifndef AUTORE_H
#define AUTORE_H

#include <QString>
#include <QList>
#include<list>
#include <iostream>
using namespace std;

class Autore{
    public:
        Autore();
        Autore(QString _id, QString _nome, QString _cognome, QStringList _afferenze);

        void setId(QString _id);
        QString getId() const;

        void setNome(QString _nome);
        QString geNome() const;

        void setCognome(QString _cognome);
        QString getCognome() const;

        void aggiungiAfferenza(QString _afferenza);
        QStringList getAfferenze() const;

        QString toString() const; //restituisce l'autore sottoforma di stringa

        bool operator==(const Autore& a) const;
        QString operator[](const int& i) const;

    private:
        QString id;
        QString nome;
        QString cognome;
        QStringList afferenze;
};


#endif // AUTORE_H
