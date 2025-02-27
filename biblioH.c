#include<stdio.h>
#include<stdlib.h>
#include <math.h> //pour utiliser floor
#include"biblioH.h" 


#define A 0.6180339887  // (Nombre d'or)-1 =(1+ racine 5)/2 - 1

int fonctionClef(char* auteur){
    int somme=0;

    while(*auteur ){
        somme += (int)(*auteur);
        auteur ++;
    }
    return somme;
}

LivreH* creer_livreH(int num,char* titre,char* auteur){
    LivreH * livre = (LivreH*)malloc(sizeof(LivreH));
    if(!livre ){
        return NULL;
    }

    livre->clef = fonctionClef(auteur);
    livre->num = num;
    livre->titre = strdup(titre);
    livre->auteur = strdup(auteur);
    if (!livre->titre || !livre->auteur) {
        free(livre->titre);
        free(livre->auteur);
        free(livre);
        return NULL;
    }
    livre->suivant = NULL;

    return livre;
}

void liberer_livreH(LivreH* l){
    if(l!=NULL) {
   
        free(l->titre);
        free(l->auteur);
        free(l);
        
    }
}

BiblioH* creer_biblioH(int m){
    BiblioH * biblio =(BiblioH*)malloc(sizeof(BiblioH));
    if(!biblio) return NULL;
    biblio->nE = 0;
    biblio-> m = m;
    biblio->T = (LivreH**)calloc(m, sizeof(LivreH*)); // Initialize hash table with NULLs
    if (biblio->T == NULL) {
      
        free(biblio);
        return NULL;
    }
    return biblio;
}
void liberer_biblioH(BiblioH* b) {
    if (b != NULL) {
        for (int i = 0; i < b->m; i++) {
            LivreH* livre = b->T[i];
            while (livre != NULL) {
                LivreH* temp = livre;
                livre = livre->suivant;
                liberer_livreH(temp);
            }
        }
        free(b->T);
        free(b);
    }
}


int fonctionHachage(int cle, int m) {//h(K)={m(ka-[ka])}
    double ka = cle * A;
    double fraction = ka - floor(ka);  // Récupère la partie fractionnaire de kA
    return (int)(m * fraction);        // Retourne la valeur de h(k)
}

void inserer(BiblioH* b, int num, char* titre, char* auteur) {
    if (b == NULL) {
        fprintf(stderr, "Erreur : Bibliothèque hachée non initialisée.\n");
        return;
    }

    int cle = fonctionClef(auteur);
    int cleHachage = fonctionHachage(cle, b->m);

    // Création du livre
    LivreH *livre_nv = (LivreH*)malloc(sizeof(LivreH));
    if (livre_nv == NULL) {
        fprintf(stderr, "Erreur : Allocation mémoire échouée pour le livre.\n");
        return;
    }

    // Copie des données du livre
    livre_nv->num = num;
    livre_nv->clef = cle;
    livre_nv->titre = strdup(titre);
    livre_nv->auteur = strdup(auteur);

    if (livre_nv->titre == NULL || livre_nv->auteur == NULL) {
        fprintf(stderr, "Erreur : Allocation mémoire échouée pour les chaînes de caractères.\n");
        free(livre_nv->titre);
        free(livre_nv->auteur);
        free(livre_nv);
        return;
    }

    // Insertion dans la table de hachage (ajout en tête de liste)
    livre_nv->suivant = b->T[cleHachage];
    b->T[cleHachage] = livre_nv;

    // Incrémentation du nombre d'éléments
    b->nE++;
}
