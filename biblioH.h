
#ifndef BIBLIOH_H
#define BIBLIOH_H
#include<string.h>
#include <math.h>


typedef struct livreh {
    int clef ;
    int num;
    char *titre;
    char *auteur;  /* int num; ... toutes les donnees permettant de representer un livre */
    struct livreh * suivant ;
} LivreH ;

typedef struct table {
    int nE ; /*nombre d’elements contenus dans la table de hachage */
    int m ; /*taille de la table de hachage */
    LivreH ** T ; /*table de hachage avec resolution des collisions par chainage */
}BiblioH ;

int fonctionClef(char* auteur);

//creer un livre
LivreH* creer_livreH(int num,char* titre,char* auteur);
//une d´esallocation
void liberer_livreH(LivreH* l);
//creer une Biblio
BiblioH* creer_biblioH(int m);
//liberer une Biblio
void liberer_biblioH(BiblioH* b);
//fonction de hachage
int fonctionHachage(int cle, int m);
//inserer la fonction;
void inserer(BiblioH* b,int num,char* titre,char* auteur);



#endif