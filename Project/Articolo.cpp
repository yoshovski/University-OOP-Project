#include "Articolo.h"
#include "Autore.h"

Articolo::Articolo(){
    id = "";
    titolo = "";
    numeroPagine = 0;
    prezzo = 0.0;
}

Articolo::Articolo(QString _id, QString _titolo, int _numeroPagine, QStringList _keywords, float _prezzo){
    id = _id;
    titolo = _titolo;
    numeroPagine = _numeroPagine;
    keywords = _keywords;
    prezzo = _prezzo;
}

Articolo::Articolo(QString _id, QString _titolo, int _numeroPagine, QList<Autore*> _autori, QStringList _keywords, float _prezzo){
    id = _id;
    titolo = _titolo;
    numeroPagine = _numeroPagine;
    autori = _autori;
    keywords = _keywords;
    prezzo = _prezzo;
}

Articolo::~Articolo(){
    articoliCorrelati.clear();
    autori.clear();
}

void Articolo::setId(QString _id){
    id = _id;
}

QString Articolo::getId() const{
    return id;
}

void Articolo::setTitolo(QString t){
    titolo = t;
}

QString Articolo::getTitolo() const{
    return titolo;
}

void Articolo::setNumeroPagine(int n){
    numeroPagine = n;
}

int Articolo::getNumeroPagine() const{
    return numeroPagine;
}

bool Articolo::aggiungiAutore(Autore* a){
    for(auto it=autori.begin(); it!=autori.end(); ++it){
        if((*it)->operator==(*a)) //se l'autore è già presente, non lo aggiunge nella lista
            return false;
            }
    autori.push_back(a);
    return true;
}

QList<Autore*> Articolo::getAutori() const{
    return autori;
}

void Articolo::aggiungiKeyword(QString k){
    keywords.push_back(k);
}

void Articolo::setKeywords(QStringList k){
    keywords=k;
}

QStringList Articolo::getKeywords() const{
    return keywords;
}

void Articolo::aggiungiArticoloCorrelato(Articolo* a){
    articoliCorrelati.push_back(a);
}

void Articolo::aggiungiArticoliCorrelati(QList<Articolo*> a){
    articoliCorrelati = a;
}

void Articolo::setPrezzo(float p){
    prezzo = p;
}

float Articolo::getPrezzo() const{
    return prezzo;
}

QList<Articolo*> Articolo::getArticoliCorrelati() const{
    return articoliCorrelati;
}

QString Articolo::toString() const{
    QString prezzo_string = QString::number(prezzo);
    QString numeroPagine_string = QString::number(numeroPagine);
    return id + " ; " + titolo + " ; " + prezzo_string + " ; " + numeroPagine_string;
}

bool Articolo::operator==(const Articolo& a) const{
    return id==a.id && autori==a.autori && titolo==a.titolo && numeroPagine==a.numeroPagine && keywords==a.keywords && prezzo==a.prezzo && articoliCorrelati==a.articoliCorrelati;
}

QString Articolo::operator[](const int& i) const{
    auto elemento = keywords.begin();
    advance(elemento, i);
    return (*elemento);
}

Articolo& Articolo::operator=(const Articolo& a){
    if(this != &a) {
        Articolo(a.id,a.titolo,a.numeroPagine,a.keywords,a.prezzo);
        articoliCorrelati = a.articoliCorrelati;
    }
    return *this;
}
