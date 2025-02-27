#include<stdio.h>
#include<stdlib.h>
#include"biblioLC.h"
#include"biblioH.h"
 
Livre* creer_livre(int num,char* titre,char* auteur){
    Livre*liv=(Livre*)malloc(sizeof(Livre));
    if (liv == NULL) return NULL;
    liv->num=num;
    liv->titre = strdup(titre);
    liv->auteur = strdup(auteur);
    liv -> suiv = NULL ;
    return liv;

}

/*----------------------------------------------------------------------------------*/

void liberer_livre(Livre* l){
    free(l->titre);
    free(l->auteur);
    free(l);

}

/*----------------------------------------------------------------------------------*/

Biblio* creer_biblio(){
    Biblio* biblio = (Biblio*)malloc(sizeof(Biblio));
    if (biblio == NULL) {
        return NULL; 
    }
    biblio->L = NULL;
    return biblio;

}

/*----------------------------------------------------------------------------------*/

void liberer_biblio(Biblio *b){
    if (b==NULL){
        return ;
    }
     Livre* temp;
    while (b->L) {
        temp = b->L;
        b->L = b->L->suiv;
        liberer_livre(temp);
    }
    free(b);
}

/*----------------------------------------------------------------------------------*/

void inserer_en_tete(Biblio * b,int num,char* titre,char* auteur){
    Livre* livre = creer_livre(num,titre,auteur);
    if(livre == NULL ) return ;

    livre->suiv = b ->L;
    b-> L = livre;

}

/*----------------------------------------------------------------------------------*/
/*-----------------------------Qst 1-6--------------------------------------------------------*/

void afficher_livre(void* l, int type) {
    if (l == NULL) {
        printf("Aucun livre à afficher.\n");
        return;
    }

    if (type == 0) {  // Liste chaînée
        Livre* livre = (Livre*)l;
        printf(" %d %s %s\n", livre->num, livre->titre, livre->auteur);
    } 
    else if (type == 1) {  // Table de hachage
        LivreH* livre = (LivreH*)l;
        printf(" : %d %s %s\n", livre->num, livre->titre, livre->auteur);
    } 
    else {
        printf("Type inconnu.\n");
    }
}

/*-------------------------------------------------------------------------*/

void afficher_biblio(void* b, int type) {
    if (b == NULL) {
        printf("Bibliothèque vide.\n");
        return;
    }

    if (type == 0) {  // Liste chaînée
        Biblio* biblio = (Biblio*)b;
        Livre* l = biblio->L;
        printf("Bibliotheque'liste chainee\n");
        while (l) {
            afficher_livre(l, 0);  // On précise que c'est un livre de liste chaînée
            l = l->suiv;
        }
    } 
    else if (type == 1) {  // Table de hachage
        BiblioH* biblio = (BiblioH*)b;
        printf("bivliotheque'table de hachage'\n");
        for (int i = 0; i < biblio->m; i++) {
            LivreH* l = biblio->T[i];
            while (l) {
                afficher_livre(l, 1);  // On précise que c'est un livre de table de hachage
                l = l->suivant;
            }
        }
    } 
    else {
        printf("Type inconnu.\n");
    }
}

/*--------------------------------------------------------------------------*/
// Fonction pour rechercher un livre par numéro
void* rechercher_par_num(void* b, int num, int type) {
    if (b == NULL) return NULL;

    if (type == 0) {  // Liste chaînée
        Biblio* biblio = (Biblio*)b;
        Livre* temp = biblio->L;
        while (temp) {
            if (temp->num == num) return (void*)temp;
            temp = temp->suiv;
        }
    } 
    else if (type == 1) {  // Table de hachage
        BiblioH* biblio = (BiblioH*)b;
        for(int index=0 ; index< biblio->m;index++){
    
            LivreH* temp = biblio->T[index];
            while (temp) {
                if (temp->num == num) return (void*)temp;
                temp = temp->suivant;
            }
    }

    return NULL;
}
}
/*------------------------------------------------------------------------------*/

void* rechercher_par_titre(void* b, char* titre,int type ) {
    if (b == NULL || titre == NULL) return NULL;

    if (type == 0) {  // Liste chaînée
        Biblio* biblio = (Biblio*)b;
        Livre* temp = biblio->L;
        while (temp) {
            if (strcmp(temp->titre, titre) == 0) return temp;
            temp = temp->suiv;
        }
    }
    else if (type == 1) {  // Table de hachage
        BiblioH* biblio = (BiblioH*)b;
        for (int i = 0; i < biblio->m; i++) {
            LivreH* temp = biblio->T[i];
            while (temp) {
                if (strcmp(temp->titre, titre) == 0) return (Livre*)temp;  // Cast vers Livre
                temp = temp->suivant;
            }
        }
    }

    return NULL;
}
/*-------------------------------------------------------------------------*/

void* rechercher_par_auteur(void* b, char* auteur, int type) {
    if (b == NULL || auteur == NULL) {
        fprintf(stderr, "Erreur : Bibliothèque ou auteur non valide.\n");
        return NULL;
    }

    // Création d'une bibliothèque pour stocker les résultats
    
    if (type == 0) {  // Liste chaînée

        Biblio* biblio = (Biblio*)b;
        Livre* temp = biblio->L;
        while (temp) {
            if (strcmp(temp->auteur, auteur) == 0) {
                return temp;
            }
            temp = temp->suiv;
        }
    }
    else if (type == 1) {  // Table de hachage
        BiblioH* biblio = (BiblioH*)b;
        int cle = fonctionClef(auteur);
        int index = fonctionHachage(cle, biblio->m);

        // Parcourir uniquement la liste chaînée à cet index
        LivreH* temp = biblio->T[index];
        while (temp) {
            if (strcmp(temp->auteur, auteur) == 0) return temp;
            temp = temp->suivant;
        }
    }
    else {
        fprintf(stderr, "Erreur : Type de bibliothèque invalide.\n");
        return NULL;
    }

  
}

/*--------------------------------------------------------------------------*/

void supprimer_livre(void* b, int num, char* titre, char* auteur,int type ) {
    if (b == NULL || titre == NULL || auteur == NULL) return;

    if (type == 0) {  // Liste chaînée
        Biblio* bibliotheque = (Biblio*)b;
        Livre* tmp = bibliotheque->L;
        Livre* prec = NULL;
        while (tmp) {
            if (tmp->num == num && strcmp(tmp->titre, titre) == 0 && strcmp(tmp->auteur, auteur) == 0) {
                if (prec) {
                    prec->suiv = tmp->suiv;
                } else {
                    bibliotheque->L = tmp->suiv;
                }
                liberer_livre(tmp);
                return;
            }
            prec = tmp;
            tmp = tmp->suiv;
        }
    }
    else if (type == 1) {  // Table de hachage
        BiblioH* bibliothequeH = (BiblioH*)b;
        int cle=fonctionClef(auteur);
        int index = fonctionHachage(cle, bibliothequeH->m);  // Calcul de l'index avec la fonction de hachage
        LivreH* tmp = bibliothequeH->T[index];
        LivreH* prec = NULL;

        while (tmp) {
            if (tmp->num == num && strcmp(tmp->titre, titre) == 0 && strcmp(tmp->auteur, auteur) == 0) {
                if (prec) {
                    prec->suivant = tmp->suivant;
                } else {
                    bibliothequeH->T[index] = tmp->suivant;
                }
                liberer_livreH(tmp);
                return;
            }
            prec = tmp;
            tmp = tmp->suivant;
        }
    }
}


/*----------------------------------------------------------------------------------*/


void fusionner_biblio(void* b1, void* b2,int type ) {
    if (b1 == NULL || b2 == NULL) {
        printf("Une des bibliothèques est vide ou inexistante.\n");
        return;
    }

    // Tester si b1 est une bibliothèque en liste chaînée en vérifiant si le champ 'L' existe
    if (type == 0) {  // Liste chaînée
        Biblio* biblio1 = (Biblio*)b1;
        Biblio* biblio2 = (Biblio*)b2;

        Livre* temp = biblio2->L;
        while (temp) {
            if (!rechercher_par_num(biblio1, temp->num,type)) {
                inserer_en_tete(biblio1, temp->num, temp->titre, temp->auteur);
            }
            temp = temp->suiv;
        }
        liberer_biblio(biblio2);  // Libérer la mémoire de b2 après la fusion
    }
    else if (type == 1) {  // Table de hachage
        BiblioH* biblio1 = (BiblioH*)b1;
        BiblioH* biblio2 = (BiblioH*)b2;

        for (int i = 0; i < biblio2->m; i++) {
            Livre* temp = (Livre*) biblio2->T[i];
            while (temp) {
                if (!rechercher_par_num(biblio1, temp->num,type)) {
                    inserer(biblio1, temp->num, temp->titre, temp->auteur);
                }
                temp = temp->suiv;
            }
        }
        liberer_biblioH(biblio2);  // Libérer la mémoire de b2 après la fusion
    }
}


/*----------------------------------------------------------------------------------*/


void* livres_en_doublon(void* b, int type) {
    void* res;  // Bibliothèque résultante (peut être une liste chaînée ou une table de hachage)
    
    // Créer la bibliothèque résultante
    if (type == 0) {  // Liste chaînée
        res = creer_biblio();  // Crée une bibliothèque de type liste chaînée
    } else {  // Table de hachage
        res = creer_biblioH(10);  // Crée une bibliothèque de type table de hachage (taille de 10 par exemple)
    }

    if (res == NULL) {
        return NULL;
    }

    if (type == 0) {  // Liste chaînée
        Biblio* biblio = (Biblio*)b;
        Livre* temp1 = biblio->L;
        while (temp1) {
            Livre* temp2 = temp1->suiv;
            while (temp2) {
                if (strcmp(temp1->titre, temp2->titre) == 0 && strcmp(temp1->auteur, temp2->auteur) == 0) {
                    // Ajouter les doublons à la bibliothèque res
                    if (!rechercher_par_num(res, temp1->num,type)) {
                        inserer_en_tete(res, temp1->num, temp1->titre, temp1->auteur);
                    }
                    if (!rechercher_par_num(res, temp2->num,type)) {
                        inserer_en_tete(res, temp2->num, temp2->titre, temp2->auteur);
                    }
                }
                temp2 = temp2->suiv;
            }
            temp1 = temp1->suiv;
        }
    } else if (type == 1) {  // Table de hachage
        BiblioH* biblio = (BiblioH*)b;
        for (int i = 0; i < biblio->m; i++) {
            LivreH* temp1 = biblio->T[i];
            while (temp1) {
                LivreH* temp2 = temp1->suivant;
                while (temp2) {
                    if (strcmp(temp1->titre, temp2->titre) == 0 && strcmp(temp1->auteur, temp2->auteur) == 0) {
                        // Ajouter les doublons à la bibliothèque res
                        if (!rechercher_par_num(res, temp1->clef,type)) {
                            inserer(res, temp1->clef, temp1->titre, temp1->auteur);  // Adapté pour LivreH
                        }
                        if (!rechercher_par_num(res, temp2->clef,type)) {
                            inserer(res, temp2->clef, temp2->titre, temp2->auteur);  // Adapté pour LivreH
                        }
                    }
                    temp2 = temp2->suivant;
                }
                temp1 = temp1->suivant;
            }
        }
    }

    return res;
}

