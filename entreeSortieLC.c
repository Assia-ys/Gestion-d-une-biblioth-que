#include<stdio.h>
#include<stdlib.h>
#include"biblioLC.h" 
#include"biblioH.h" 
#include"entreeSortieLC.h"

#define LONGUEUR_TAB 100
void* charger_n_entrees(char* nomfic, int n, int type) {
    FILE *file = fopen(nomfic, "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier \n");
        return NULL;
    }

    void* biblio;  // Pointeur générique pour Biblio (liste) ou BiblioH (hachage)

    if (type == 0) { // Liste chaînée
        biblio = creer_biblio();
    } else { // Table de hachage
        //int taille_table = 500000; // Choisir une taille raisonnable
        biblio = creer_biblioH(LONGUEUR_TAB);
    }

    if (biblio == NULL) {
        fclose(file);
        return NULL;
    }

    int num;
    char titre[256], auteur[256];
    int count = 0;

    // Lire au maximum `n` lignes du fichier
    while (count < n && fscanf(file, "%d %255s %255s", &num, titre, auteur) == 3) {
        if (type == 0) { // Liste chaînée
            inserer_en_tete((Biblio*)biblio, num, titre, auteur);
        } else { // Table de hachage

            inserer((BiblioH*)biblio, num, titre, auteur);
        }
        count++;
    }

    fclose(file);
    return biblio;
}
/*----------------------------------------------------------------------------------*/

void enregistrer_biblio(void *b, char *nomfic, int type) {
    if (b == NULL) {
        printf("La bibliothèque est vide ou inexistante.\n");
        return;
    }

    FILE *file = fopen(nomfic, "w");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier en écriture.\n");
        return;
    }

    if (type == 0) {  // Liste chaînée
        Biblio *biblio = (Biblio*)b;
        Livre *l = biblio->L;
        while (l) {
            fprintf(file, "%d %s %s\n", l->num, l->titre, l->auteur);
            l = l->suiv;
        }
    } else {  // Table de hachage
        BiblioH *biblio = (BiblioH*)b;
        for (int i = 0; i < biblio->m; i++) {
            LivreH *l = biblio->T[i];
            while (l) {
                fprintf(file, "%d %s %s\n", l->clef, l->titre, l->auteur);
                l = l->suivant;
            }
        }
    }

    fclose(file);
    printf("Bibliothèque enregistrée dans %s.\n", nomfic);
}
