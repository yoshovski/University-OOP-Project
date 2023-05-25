#ifndef ELEMENTO_H
#define ELEMENTO_H

#include <iostream>
#include <QStringList>
#include <QList>
using namespace std;

template <class T>
class Elemento{
    public:
        Elemento();
        QStringList lista(QList<T*>& elementi); //converte una lista di oggetti in QStringList
    private:
};

template<class T>
Elemento<T>::Elemento(){}

template<class T>
QStringList Elemento<T>::lista(QList<T*>& elementi){
    QStringList lista;
    for(auto e : elementi){
        QString elemento = e->toString();
        lista.push_back(elemento);
}
    return lista;
}
#endif // ELEMENTO_H
