#include "Autore.h"
using namespace std;

Autore::Autore(){
    id = "";
    nome = "";
    cognome = "";
}

Autore::Autore(QString _id, QString _nome, QString _cognome, QStringList _afferenze){
    id = _id;
    nome = _nome;
    cognome = _cognome;

    if(!_afferenze.empty())
        afferenze = _afferenze;
}

void Autore::setId(QString _id){
    id=_id;
}

QString Autore::getId() const{
    return id;
}

void Autore::setNome(QString _nome){
    nome = _nome;
}

QString Autore::geNome() const{
    return nome;
}

void Autore::setCognome(QString _cognome){
    cognome = _cognome;
}

QString Autore::getCognome() const{
    return cognome;
}

void Autore::aggiungiAfferenza(QString _afferenza){
    afferenze.push_back(_afferenza);
}

QStringList Autore::getAfferenze() const{
    return afferenze;
}

QString Autore::toString() const{
    return id + " ; " + nome + " ; " + cognome;
}

bool Autore::operator==(const Autore& a) const{
    return id == a.id && nome == a.nome && cognome == a.cognome;
}

QString Autore::operator[](const int& i) const{
    return afferenze[i];
}
