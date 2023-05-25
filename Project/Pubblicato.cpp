#include "Pubblicato.h"

Pubblicato::Pubblicato(){
    nome = "";
    acronimo = "";
}

Pubblicato::Pubblicato(QString _nome, QString _acronimo, QDate _data){
    nome = _nome;
    acronimo = _acronimo;
    data = _data;
}

void Pubblicato::setNome(QString n){
    nome = n;
}

QString Pubblicato::getNome() const{
    return nome;
}

void Pubblicato::setAcronimo(QString a){
    acronimo = a;
}

QString Pubblicato::getAcronimo() const{
    return acronimo;
}

void Pubblicato::setData(QDate d){
    data = d;
}

QDate Pubblicato::getData() const{
    return data;
}
