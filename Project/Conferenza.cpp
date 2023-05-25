#include "Conferenza.h"
using namespace std;

Conferenza::Conferenza(){
    nome = "";
    acronimo = "";
    luogo = "";
    numeroPartecipanti = 0;
}

Conferenza::Conferenza(QString _nome, QString _acronimo, QString _luogo, QDate _data, QStringList _organizzatori, int _numeroPartecipanti){
    nome = _nome;
    acronimo = _acronimo;
    luogo = _luogo;
    data = _data;
    organizzatori = _organizzatori;
    numeroPartecipanti = _numeroPartecipanti;
}

Conferenza::~Conferenza(){
    articoli.clear();
}

void Conferenza::setNome(QString n){
    nome = n;
}

QString Conferenza::getNome() const{
    return nome;
}

void Conferenza::setAcronimo(QString a){
    acronimo = a;
}

QString Conferenza::getAcronimo() const{
    return acronimo;
}

void Conferenza::setLuogo(QString l){
    luogo = l;
}

QString Conferenza::getLuogo() const{
    return luogo;
}


void Conferenza::setData(QDate d){
    data = d;
}

QDate Conferenza::getData() const{
    return data;
}

void Conferenza::setOrganizzatori(QStringList o){
    organizzatori = o;
}

QStringList Conferenza::getOrganizzatori() const{
    return organizzatori;
}

void Conferenza::aggiungiOrganizzatore(QString o){
    organizzatori.push_back(o);
}

void Conferenza::aggiungiArticolo(Articolo* a){
    articoli.push_back(a);
}

void Conferenza::setNumPartecipanti(int n){
    numeroPartecipanti = n;
}

int Conferenza::getNumPartecipanti() const{
    return numeroPartecipanti;
}

QString Conferenza::toString(){
    QString data_string = data.toString(Qt::DateFormat::ISODate);
    QString numPartecipanti_string = QString::number(numeroPartecipanti);
    return nome + " ; " + acronimo + " ; " + luogo + " ; " + numPartecipanti_string + " ; " + data_string;
}

QStringList Conferenza::getListaArticoli(){
    Elemento<Articolo> e;
    return e.lista(articoli);
}

QList<Articolo*> Conferenza::getArticoli() const{
    return articoli;
}

bool Conferenza::articoloPresente(QString id){
    for(auto it=articoli.begin(); it!=articoli.end(); ++it){
            if((*it)->getId()==id){
                return true;
            }
    }
    return false;
}

QString Conferenza::operator[](const int& i) const{
    return organizzatori[i];
}

bool Conferenza::operator==(const Conferenza& c) const{
    return nome==c.nome && acronimo==c.acronimo && luogo==c.luogo && data==c.data && organizzatori==c.organizzatori && numeroPartecipanti==c.numeroPartecipanti;
}
