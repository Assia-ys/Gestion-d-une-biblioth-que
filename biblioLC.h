#ifndef BIBLIOC_H
#define BIBLIOC_H
#include<string.h>
#include"biblioH.h"

typedef struct livre { 
    int num ;
    char * titre ;
    char * auteur ;
    struct livre * suiv ;
}Livre ;

typedef struct { 
    Livre * L ; 
}Biblio ;

//Qst 2

//cree un livre
Livre* creer_livre(int num,char* titre,char* auteur);
//realiser une desallocation
void liberer_livre(Livre* l);
//cree une bibliotheque vide
Biblio* creer_biblio();
//libere la memoire occupee par une bibliotheque
void liberer_biblio(Biblio *b);
//ajouter un nouveau livre a la bib
void inserer_en_tete(Biblio * b,int num,char* titre,char* auteur);


void afficher_livre(void* l, int type);
void afficher_biblio(void* b, int type);
void* rechercher_par_num(void* b, int num, int type);
void* rechercher_par_titre(void* b, char* titre,int type ) ;
void* rechercher_par_auteur(void* b, char* auteur, int type );
void supprimer_livre(void* b, int num, char* titre, char* auteur,int type ) ;
void fusionner_biblio(void* b1, void* b2,int type ) ;
void* livres_en_doublon(void* b, int type) ;

#endif