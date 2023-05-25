#include "Rivista.h"
using namespace std;

Rivista::Rivista(){
    nome = "";
    acronimo = "";
    editore = "";
    volume = 0;
}

Rivista::Rivista(QString _nome, QString _acronimo, QString _editore, QDate _data, int _volume){
    nome = _nome;
    acronimo = _acronimo;
    editore = _editore;
    data = _data;
    volume = _volume;
}

Rivista::~Rivista(){
    articoli.clear();
}

void Rivista::setNome(QString n){
    nome = n;
}

QString Rivista::getNome() const{
    return nome;
}

void Rivista::setAcronimo(QString a){
    acronimo = a;
}

QString Rivista::getAcronimo() const{
    return acronimo;
}

void Rivista::setEditore(QString e){
    editore = e;
}

QString Rivista::getEditore() const{
    return editore;
}

void Rivista::setData(QDate d){
    data = d;
}

QDate Rivista::getData() const{
    return data;
}

void Rivista::setVolume(int v){
    volume = v;
}

int Rivista::getVolume() const{
    return volume;
}

void Rivista::aggiungiArticolo(Articolo* a){
    articoli.push_back(a);
}

QString Rivista::toString() const{
    QString data_string = data.toString(Qt::DateFormat::ISODate);
    QString volume_string = QString::number(volume);
    return nome + " ; " + acronimo + " ; " + editore + " ; " + data_string + " ; " + volume_string;
}

bool Rivista::operator==(const Rivista& r) const{
    return nome==r.nome && acronimo==r.acronimo && editore==r.editore && data==r.data && volume==r.volume;
}
QList<Articolo*> Rivista::getArticoli() const{
    return articoli;
}

QStringList Rivista::getListaArticoli(){
    Elemento<Articolo> e;
    return e.lista(articoli);
}

bool Rivista::autorePresente(QString id){
    QList<Autore*> autori;
    for(auto it=articoli.begin(); it!=articoli.end(); ++it){
        autori = (*it)->getAutori();
        for(auto it2=autori.begin(); it2!=autori.end(); ++it2){
            if((*it2)->getId()==id){
                return true;
            }
        }
    }
    return false;
}

bool Rivista::articoloPresente(QString id){
    for(auto it=articoli.begin(); it!=articoli.end(); ++it){
            if((*it)->getId()==id){
                return true;
            }
    }
    return false;
}
