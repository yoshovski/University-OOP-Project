#include "Gestore.h"
#include <algorithm>
#include <iostream>

Gestore::Gestore(){}

Gestore::~Gestore(){
    for(auto it=autori.begin(); it!=autori.end(); ++it){
        delete *it;
    }
    autori.clear();

    for(auto it=riviste.begin(); it!=riviste.end(); ++it){
        delete *it;
    }
    riviste.clear();

    for(auto it=articoli.begin(); it!=articoli.end(); ++it){
        delete *it;
    }
    articoli.clear();

    for(auto it=conferenze.begin(); it!=conferenze.end(); ++it){
        delete *it;
    }
    conferenze.clear();
}

//AUTORE
bool Gestore::aggiungiAutore(Autore* a){
    for(auto it=autori.begin(); it!=autori.end(); ++it){
        if((*it)->getId()==a->getId())
            return false;
    }
    autori.push_back(a);
    return true;
}

const QList<Autore*>& Gestore::getAutori() const{
    return autori;
}

const Autore* Gestore::getAutore(int i) const{
    auto elemento = autori.begin();
    advance(elemento, i);
    return *elemento;
}

QStringList Gestore::listaAfferenze(int row) const{
    auto elemento = autori.begin();
    advance(elemento, row);
    return (*elemento)->getAfferenze();
}

QStringList Gestore::listaAutori(){
    Elemento<Autore> e;
    return e.lista(autori);
}

QStringList Gestore::listaAutoriInArticoli(int row) const{
    auto elemento = articoli.begin();
    advance(elemento, row);
    QList<Autore*> autori = (*elemento)->getAutori();

    Elemento<Autore> e;
    return e.lista(autori);
}


//CONFERENZA
bool Gestore::aggiungiConferenza(Conferenza* c){
    for(auto it=conferenze.begin(); it!=conferenze.end(); ++it){
        if((*it)->operator==(*c))
            return false;
            }
    conferenze.push_back(c);
    return true;
}

QStringList Gestore::listaOrganizzatori(int row) const{
    auto elemento = conferenze.begin();
    advance(elemento, row);
    return (*elemento)->getOrganizzatori();
}

const QList<Conferenza*>& Gestore::getConferenze() const{
    return conferenze;
}

QStringList Gestore::listaConferenze(){
    Elemento<Conferenza> e;
    return e.lista(conferenze);
}

QStringList Gestore::listaConferenze(QList<Conferenza*> conferenze) const{
    Elemento<Conferenza> e;
    return e.lista(conferenze);
}

QStringList Gestore::listaConferenzeSimili(int row) const{
    Conferenza* conferenza_A = conferenze.at(row); //la conferenza specificata dall'utente
    QList<Articolo*> articoli_A = conferenza_A->getArticoli(); //prendo tutti gli articoli della conferenza in posizione "row"
    QStringList keywords_A;
    QList<Conferenza*> conferenzeSimili;

    //salvo in una lista tutte le keywords presenti nella conferenza
    for(auto a : qAsConst(articoli_A)){
       keywords_A = keywords_A + a->getKeywords();
    }
    keywords_A.sort();
    keywords_A.removeDuplicates(); //rimuovo i duplicati, così ottengo le keywords diverse
    int dim_A = keywords_A.size();

    if(dim_A>0){  //la conferenza deve avere almeno una keyword, per evitare di considerare le conferenze senza keywords
    for(auto c : conferenze){ //scorro tutte le conferenze alla ricerca di simili
        QList<Articolo*> articoli_B = c->getArticoli();
        QStringList keywords_B;
        for(auto a : qAsConst(articoli_B)){
            keywords_B = keywords_B + a->getKeywords();
        }

        keywords_B.sort();
        keywords_B.removeDuplicates(); //rimuovo i duplicati di keywords della conferenza B

        int dim_B = keywords_B.size();

        keywords_B = keywords_B + keywords_A; //concateno le keywords di Conferenza "A" e "B"
        int matches = keywords_B.removeDuplicates(); //rimuovendo i duplicati, viene restituito il numero di duplicati. Così si ottiene il numero esatto di keywords in comune

        if(matches >= (dim_A*0.8)){ //se almeno 80% di keywords sono in comune
            if(!(conferenza_A==c) && dim_B>0){ //controllo ulteriore per impedire che Conferenze senza keyword vengano aggiunte o che Conferenza A aggiunga se stessa alla lista
                conferenzeSimili.push_back(c);

                cout<<"Conferenze Simili Matches: "<<matches<<endl;
                cout<<"Dim A: "<<dim_A<<endl;
                cout<<"Dim B: "<<dim_B<<endl;
            }
          }
        keywords_B.clear();
    }
    }
    return listaConferenze(conferenzeSimili);
}

//RIVISTA
bool Gestore::aggiungiRivista(Rivista* r){
    for(auto it=riviste.begin(); it!=riviste.end(); ++it){
        if((*it)->operator==(*r))
            return false;
            }
    riviste.push_back(r);
    return true;
}

const QList<Rivista*>& Gestore::getRiviste() const{
    return riviste;
}

QStringList Gestore::listaRiviste(){
    Elemento<Rivista> e;
    return e.lista(riviste);
}

QStringList Gestore::listaRiviste(QList<Rivista*> riv) const{
    Elemento<Rivista> e;
    return e.lista(riv);
}

QStringList Gestore::listaRivisteDiAutoreNoArticolo(int row) const{
    QList<Rivista*> riv = riviste; //creo una copia di tutte le riviste
    QString id = autori.at(row)->getId();

    for(auto it = riviste.begin(); it!=riviste.end(); ++it){
        if((*it)->autorePresente(id)){
            riv.erase(find(riv.begin(),riv.end(),*it)); //elimino dalla lista riv, la rivista alla quale appartiene l'autore
            }
        }
    return listaRiviste(riv);
}

QStringList Gestore::listaRivisteSpecialistiche() const{
    QList<Rivista*> rivisteSpecialistiche;

    QStringList keywords_R1;
    QStringList keywords_R2;

    for(auto r1 : riviste){
        QList<Articolo*> articoli_R1 = r1->getArticoli();
        for(auto a : qAsConst(articoli_R1)){
           keywords_R1 = keywords_R1 + a->getKeywords();
        }

        keywords_R1.removeDuplicates();

        for(auto r2 : riviste){
            if(!(*r1==*r2)){
                QList<Articolo*> articoli_R2 = r2->getArticoli();
                for(auto a : qAsConst(articoli_R2)){
                   keywords_R2 = keywords_R2 + a->getKeywords();
                }
                keywords_R2.removeDuplicates();

                bool trovato = true;

                if(keywords_R2.size()>keywords_R1.size()){
                    for(int i=0; i<keywords_R1.size(); i++){
                        if((find(keywords_R2.begin(),keywords_R2.end(),keywords_R1.at(i)))!=keywords_R2.end()){
                        }
                        else{
                            trovato = false;
                            break;
                        }
                    }

                    if(trovato){
                        rivisteSpecialistiche.push_back(r1);
                    }
                }
            }
            keywords_R2.clear();
        }
        keywords_R1.clear();
    }

    QStringList lista = listaRiviste(rivisteSpecialistiche);
    lista.removeDuplicates();
    return lista;
}

//ARTICOLO
bool Gestore::aggiungiArticolo(Articolo* a){
    for(auto it=articoli.begin(); it!=articoli.end(); ++it){
        if((*it)->getId()==(*a).getId()) //articoli completamente uguali
            return false;
            }
    articoli.push_back(a);
    return true;
}

void Gestore::aggiungiAutoriInArticolo(Articolo* a, QStringList id_autori){
    for(int i=0; i<id_autori.size(); i++){
        for(auto it=autori.begin(); it!=autori.end(); ++it){
           if((*it)->getId()==id_autori[i]){
               Autore* autore = (*it);
               a->aggiungiAutore(autore);
           }
        }
    }
}

void Gestore::aggiungiArticoliCorrelati(Articolo* a, QStringList id_articoli){
    for(int i=0; i<id_articoli.size(); i++){
        for(auto it=articoli.begin(); it!=articoli.end(); ++it){
           if((*it)->getId()==id_articoli[i]){
               Articolo* articolo = (*it);
               a->aggiungiArticoloCorrelato(articolo);
           }
        }
    }
}

const QList<Articolo*>& Gestore::getArticoli() const{
    return articoli;
}

bool Gestore::idEsistente(QString _id){
    for(auto it=articoli.begin(); it!=articoli.end(); ++it){
        if((*it)->getId()==_id)
            return true;
            }
    return false;
}

QStringList Gestore::listaKeywords(int row) const{
    auto elemento = articoli.begin();
    advance(elemento, row);
    return (*elemento)->getKeywords();
}

QStringList Gestore::listaArticoliCorrelati(int row) const{
    auto elemento = articoli.begin();
    advance(elemento, row);
    QList<Articolo*> artCorrelati = (*elemento)->getArticoliCorrelati();

    Elemento<Articolo> e;
    return e.lista(artCorrelati);
}

QStringList Gestore::listaArticoli(){
    Elemento<Articolo> e;
    return e.lista(articoli);
}

QStringList Gestore::listaArticoli(QList<Articolo*> a){
    Elemento<Articolo> e;
    return e.lista(a);
}

QList<Articolo*> Gestore::getArticoliDiAutore(int row){
    auto elemento = autori.begin();
    advance(elemento, row);

    QList<Articolo*> tmp_articoliDiAuore;

    for(auto it=articoli.begin(); it!=articoli.end(); ++it){
        QList<Autore*> aut = (*it)->getAutori();
            if(std::find(aut.begin(),aut.end(),*elemento)!=aut.end()){
                tmp_articoliDiAuore.push_back(*it);
            }
    }

    return tmp_articoliDiAuore;
}

QStringList Gestore::listaArticoliDiAutore(int row){
    QList<Articolo*> articoli = getArticoliDiAutore(row);
    return listaArticoli(articoli);
}

QStringList Gestore::listaArticoliDiConferenza(int row){
    auto elemento = conferenze.begin();
    advance(elemento, row);
    return (*elemento)->getListaArticoli();
}

QStringList Gestore::listaArticoliDiRivista(int row){
    auto elemento = riviste.begin();
    advance(elemento, row);
    return (*elemento)->getListaArticoli();
}

void Gestore::pubblicaInConferenza(Articolo* a,Conferenza* c){
    for(auto it=conferenze.begin(); it!=conferenze.end(); ++it){
        if((*it)->operator==(*c))
            (*it)->aggiungiArticolo(a);
    }
}

void Gestore::pubblicaInRivista(Articolo* a,Rivista* r){
    for(auto it=riviste.begin(); it!=riviste.end(); ++it){
        if((*it)->operator==(*r))
            (*it)->aggiungiArticolo(a);
    }
}

bool Gestore::comparePrezzoBasso(const Articolo* a1, const Articolo* a2){
    return a1->getPrezzo() < a2->getPrezzo();
}

bool Gestore::comparePrezzoAlto(const Articolo* a1, const Articolo* a2){
    return a1->getPrezzo() > a2->getPrezzo();
}

QStringList Gestore::articoliOrdinatiPerPrezzo(int row, int tipo){

    QList<Articolo*> art;
    QStringList articoliOrdinati;

    if(tipo==0){ //autore
        art = getArticoliDiAutore(row);
        std::sort(art.begin(),art.end(),comparePrezzoBasso);
    }
    else if(tipo==1){ //confereneze
        auto elemento = conferenze.begin();
        advance(elemento, row);
        art = (*elemento)->getArticoli();
        std::sort(art.begin(),art.end(),comparePrezzoBasso);
    }
    else if(tipo==2){ //riviste
        auto elemento = riviste.begin();
        advance(elemento, row);
        art = (*elemento)->getArticoli();
        std::sort(art.begin(),art.end(),comparePrezzoBasso);
    }
    return listaArticoli(art);
}

float Gestore::mediaPrezziArticoliAutore(int row){
    QList<Articolo*> art = getArticoliDiAutore(row);
    if(art.size()==0)
        return 0.0;

    float media=0;
    for(auto it=art.begin(); it!=art.end(); ++it){
        media+=(*it)->getPrezzo(); }

    media = media/art.size();

    float scala = 0.01;
    return (int)(media/scala)*scala; //ritorna il valore con 2 cifre dopo la virgola
}



QStringList Gestore::prezzoPiuBassoArticoliAutore(int row){
    QList<Articolo*> art = getArticoliDiAutore(row);
    QList<Articolo*> art_PrezziBassi;
    std::sort(art.begin(),art.end(),comparePrezzoBasso);
    auto it=art.begin();
    for(auto a : qAsConst(art)){
        if(a->getPrezzo()==(*it)->getPrezzo()){
            art_PrezziBassi.push_back(a);
        }
    }
    return listaArticoli(art_PrezziBassi);
}

QStringList Gestore::prezzoPiuAltoArticoliAutore(int row){
    QList<Articolo*> art = getArticoliDiAutore(row);
    QList<Articolo*> art_PrezziAlti;
    std::sort(art.begin(),art.end(),comparePrezzoAlto);
    auto it=art.begin();
    for(auto a : qAsConst(art)){
        if(a->getPrezzo()==(*it)->getPrezzo()){
            art_PrezziAlti.push_back(a);
        }
    }
    return listaArticoli(art_PrezziAlti);
}


bool Gestore::follow(const Articolo* a1, const Articolo* a2){
    if(a1->getPrezzo()>a2->getPrezzo()) //ordine per prezzo decrescente
        return true;
    if(a1->getPrezzo()<a2->getPrezzo())
        return false;
    return (*a1)[0]<(*a2)[0]; //ordine per ordine alfabetico per la prima keyword in lista
}


QStringList Gestore::ordinaArticoliDiAutore(int row){
    QList<Articolo*> art = getArticoliDiAutore(row);
    if(art.size()>1){
           std::sort(art.begin(),art.end(),follow); //ordine per prezzo decrescente, con prezzo uguale ordina per prima keyword in lista

//a questo punto, anche se l'ordine per anno viene eseguito come secondo, gli articoli avranno l'ordine desiderato

           //#1 start bubble sort
           bool swap = true;
               while(swap) {
                   swap = false;
                   for(auto it = art.begin(); it != art.end(); ++it) {
                       auto it2 = it;
                       ++it2;
                       if(it2 == art.end()) break;
                       if(getAnno(*it) > getAnno(*it2)) {
                           swap = true;
                           Articolo* tmp = *it;
                           *it = *it2;
                           *it2 = tmp;
                       }
                   }
               }
           //#1 end
        }
    return listaArticoli(art);
}

int Gestore::getAnno(Articolo* a) const{
    for(auto r : riviste){
        if(r->articoloPresente(a->getId())){
            return r->getData().year(); //l'anno della data
        }
    }

    for(auto c : conferenze){
        if(c->articoloPresente(a->getId())){
            return c->getData().year();
        }
    }

    return 0;
}

QDate Gestore::getData(Articolo* a) const{
    for(auto r : riviste){
        if(r->articoloPresente(a->getId())){
            return r->getData();
        }
    }

    for(auto c : conferenze){
        if(c->articoloPresente(a->getId())){
            return c->getData();
        }
    }

    QDate data_null;
    data_null.setDate(0000,00,0);

    return data_null;
}

QStringList Gestore::listaKeywordsGuadagno() const{
    float somma = 0;
    float max_guadagno = INT_MIN;
    QString current_keyword;
    QStringList keywords_guadagno;

    for(auto a : articoli){
        QStringList keywords = a->getKeywords();
        for(int i=0; i<keywords.size(); i++){
            current_keyword = keywords.at(i);
            for(auto art : articoli){
                QStringList keywords2 = art->getKeywords();
                if((find(keywords2.begin(),keywords2.end(),current_keyword))!=keywords2.end()){
                    somma+=art->getPrezzo();
                }
            }

            if(somma>max_guadagno){
                keywords_guadagno.clear();
                max_guadagno=somma;
                keywords_guadagno.push_back(current_keyword);
            }
            else if(somma==max_guadagno){
                keywords_guadagno.push_back(current_keyword);
            }
            somma=0;
        }
    }
    cout<<"Guadagno max: "<<max_guadagno<<endl;
    keywords_guadagno.removeDuplicates();
    return keywords_guadagno;
}

QStringList Gestore::listaArticoliInfluenzati(int row){

    if(articoli.empty()){
        QStringList lista;
        return lista;
    }

    //prende l'iteratore dell'articolo scelto
    auto articolo = articoli.begin();
    advance(articolo, row);

    QList<Articolo*> articoli_influenzati;

    for(auto a : qAsConst(articoli)){
        QList<Articolo*> visitati;
        QList<Articolo*> daVisitare;

        if(influenza(*articolo,a)){
            daVisitare.push_back(a);

            while(!daVisitare.empty()){
                Articolo* articolo_corrente = daVisitare.back();
                daVisitare.pop_back();
                visitati.push_back(articolo_corrente);

                if(!esiste(articoli_influenzati,articolo_corrente))
                    articoli_influenzati.push_back(articolo_corrente);

                for(auto a2 : qAsConst(articoli)){
                    if(influenza(articolo_corrente,a2)){
                            if(!esiste(visitati,a2) && !esiste(daVisitare,a2))
                              daVisitare.push_back(a2);
                        }
                }
            }
         }
    }
    return listaArticoli(articoli_influenzati);
}

bool Gestore::influenza(Articolo* articolo_a, Articolo* articolo_b){
   if(articolo_a->getId() != articolo_b->getId()){ //impedisce i duplicati
        QList<Articolo*> artCorrelati = articolo_b->getArticoliCorrelati();

        if(find(artCorrelati.begin(),artCorrelati.end(),articolo_a) != artCorrelati.end()){
            if(getData(articolo_a) < getData(articolo_b)){ //se la (data articolo A) < (data Articolo B)
                return true;
                }
             }
    }
    return false;
}

bool Gestore::esiste(QList<Articolo*> lista_articoli, Articolo* a){
    return find(lista_articoli.begin(), lista_articoli.end(), a) != lista_articoli.end();
}
