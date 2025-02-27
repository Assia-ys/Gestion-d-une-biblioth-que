
#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include <time.h>
#include "biblioLC.h"
#include "biblioH.h"
#include "entreeSortieLC.h"

//la fonction menu 
void menu() {
    printf("\n-- Menu de la bibliothèque --\n");
    printf("0 - Quitter\n");
    printf("1 - Afficher la bibliothèque\n");
    printf("2 - ajouter un livre\n");
    printf("3 - Rechercher un ouvrage par numéro\n");
    printf("4 - Rechercher un ouvrage par titre\n");
    printf("5 - Rechercher un ouvrage par auteur\n");
    printf("7 - Fusionner deux bibliothèques\n");
    printf("8 - Afficher les livres en doublon\n");
    printf("----------------------------\n");
    printf("Entrez le numéro de l'action: ");
}

//fonction qui permet enregister le temps de chercher le livre avec les 2 methodes :liste chainee et table de hachage 
void mesurer_temps_par_n(int n_max, int pas) {
    FILE *fichier = fopen("resultats_par_auteur.txt", "w");
    if (!fichier) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    int n;
    for (n = 1000; n <= n_max; n *= pas) {
        Biblio* biblio_liste = charger_n_entrees("GdeBiblio.txt", n, 0);
        BiblioH* biblio_hash = charger_n_entrees("GdeBiblio.txt", n, 1);

        clock_t debut, fin;
        double temps_liste, temps_hash;

        // Mesurer temps pour la table de hachage
        debut = clock();
        rechercher_par_auteur(biblio_hash, "uxwp", 1);
        fin = clock();
        temps_hash = ((double)(fin - debut)) / CLOCKS_PER_SEC;

        // Mesurer temps pour la liste chaînée
        debut = clock();
        rechercher_par_auteur(biblio_liste,"uxwp", 0);
        fin = clock();
        temps_liste = ((double)(fin - debut)) / CLOCKS_PER_SEC;

        // Enregistrer dans le fichier
        fprintf(fichier, "%d %f %f\n", n, temps_liste, temps_hash);
        printf("%d livres : Liste = %f s, Hachage = %f s\n", n, temps_liste, temps_hash);

        // Libérer la mémoire
        liberer_biblio(biblio_liste);
        liberer_biblioH(biblio_hash);
    }

    fclose(fichier);
}


//main

int main(int argc, char** argv){
    int choix;
    int type;
    mesurer_temps_par_n(50000, 2);

    if (argc != 3) {
        fprintf(stderr, "Usage : %s <nom_fichier> <nombre_lignes>\n", argv[0]);
        return 0 ;
    }

 
    char *nom_fichier = argv[1];
    int n = atoi(argv[2]); 

    if (n <= 0) {
        fprintf(stderr, "Erreur : le nombre de lignes doit être un entier strictement positif.\n");
        return EXIT_FAILURE;
    }

    // Choisir entre liste chaînée et table de hachage
    printf("Choisissez le type de bibliothèque :\n");
    printf("0 - Liste chaînée\n");
    printf("1 - Table de hachage\n");
    scanf("%d", &type);
   
    void* biblio = charger_n_entrees(nom_fichier, n, type);
    if (biblio == NULL) {
        fprintf(stderr, "Erreur lors du chargement de la bibliothèque.\n");
        return EXIT_FAILURE;
    }

    // Affichage de la bibliothèque chargée (liste chaînée ou table de hachage)
    printf("Bibliothèque chargée avec %d livres depuis %s :\n", n, nom_fichier);

    if (type == 0) {  // Liste chaînée
        Biblio* biblioListe = (Biblio*)biblio;
        Livre* livre = biblioListe->L;
        while (livre != NULL) {
            printf("%d %s %s\n", livre->num, livre->titre, livre->auteur);
            livre = livre->suiv;
        }
    } else {  // Table de hachage
        BiblioH* biblioHachage = (BiblioH*)biblio;
        for (int i = 0; i < biblioHachage->m; i++) {
            LivreH* livre = biblioHachage->T[i];
            while (livre != NULL) {
                printf("%d %s %s\n", livre->num, livre->titre, livre->auteur);
                livre = livre->suivant;
            }
        }
    }
    //Qst 1-7/////////////////////////////

    do {
        menu();
        scanf("%d", &choix);

        switch (choix) {
            case 0:
                printf("Sortie du programme.\n");
                break;
            case 1:
                
                afficher_biblio(biblio, type);
                
                break;
                case 2:
                {
                    int num;
                    char titre[256], auteur[256]; 
            
                    printf("Entrez le numéro, le titre et l'auteur :\n");
            
                    // Vérifier si scanf fonctionne bien
                    if (scanf("%d %255s %255s", &num, titre, auteur) != 3) {
                        fprintf(stderr, "Erreur lors de la saisie.\n");
                        while (getchar() != '\n');  // Nettoyer le buffer d'entrée
                        break;
                    }
            
                    // Insertion selon le type de bibliothèque
                    if (type == 0) {
                        inserer_en_tete((Biblio*)biblio, num, titre, auteur);
                    } else {
                        inserer((BiblioH*)biblio, num, titre, auteur);
                    }
            
                    printf("Livre ajouté avec succès !\n");
                }
                break;
            
            
                case 3:
                // Recherche par numéro
                int num;
                printf("Entrez le numéro de l'ouvrage à rechercher: ");
                scanf("%d", &num);
                if (type == 0) {
                    Livre* l = rechercher_par_num((Biblio*)biblio, num,type);
                    if (l) {
                        afficher_livre(l, type);
                    } else {
                        printf("Ouvrage non trouvé.\n");
                    }
                } else {
                    LivreH* l = rechercher_par_num((BiblioH*)biblio, num,type);
                    if (l) {
                        afficher_livre((Livre*)l, type); // Cast vers Livre
                    } else {
                        printf("Ouvrage non trouvé.\n");
                    }
                }
                break;
            case 4:
                // Recherche par titre
                char titreRecherche[256];
                printf("Entrez le titre de l'ouvrage à rechercher: ");
                scanf("%s", titreRecherche);
                if (type == 0) {
                    Livre* l = rechercher_par_titre((Biblio*)biblio, titreRecherche,type);
                    if (l) {
                        afficher_livre(l, type);
                    } else {
                        printf("Ouvrage non trouvé.\n");
                    }
                } else {
                    LivreH* l = rechercher_par_titre((BiblioH*)biblio, titreRecherche,type);
                    if (l) {
                        afficher_livre((Livre*)l, type); // Cast vers Livre
                    } else {
                        printf("Ouvrage non trouvé.\n");
                    }
                }
                break;
            case 5:
                // Recherche par auteur
                char auteurRecherche[256];
                printf("Entrez le nom de l'auteur à rechercher: ");
                scanf("%s", auteurRecherche);
                if (type == 0) {
                    Biblio* result = rechercher_par_auteur((Biblio*)biblio, auteurRecherche,type);
                    afficher_biblio(result, type);
                } else {
                    BiblioH* result = rechercher_par_auteur((BiblioH*)biblio, auteurRecherche,type);
                    afficher_biblio(result, type);
                }
                break;
            case 6:
                {
                
                char titre[256], auteur[256];
                printf("Entrez le numéro, le titre et l'auteur : ");
        
                // Vérification de la saisie
                if (scanf("%d %255s %255s", &num, titre, auteur) != 3) {
                    fprintf(stderr, "Erreur lors de la saisie.\n");
                    while (getchar() != '\n');  // Nettoyer le buffer d'entrée
                    break;
                }
            
                if (type == 0) {
                    supprimer_livre((Biblio*)biblio, num, titre, auteur, type);
                } else {
                    supprimer_livre((BiblioH*)biblio, num, titre, auteur, type);
                }
            
                printf("Livre supprimé avec succès !\n");
                }
                break;
            case 7:
                // Fusionner deux bibliothèques
                {
                    printf("Fusion des bibliothèques...\n");
                    Biblio* biblio2 = creer_biblio();
                    fusionner_biblio(biblio, biblio2, type);
                }
                break;
            case 8:
                // Afficher les livres en doublon
                {
                    Biblio* doublons = livres_en_doublon(biblio, type);
                    afficher_biblio(doublons, type);
                }
                break;
            default:
                printf("Choix invalide. Essayez encore.\n");
        }
    } while (choix != 0);

    if(type==0){
        liberer_biblio((Biblio*)biblio);
    }else{
        liberer_biblioH((BiblioH*)biblio);

    }

    return 0;
}
/* EXERCICE 03
||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
3-1/

----------------------------------------------------------------------------------------------
Type de recherche	        |Liste chaînée	               |Table de hachage
🔍 Recherche par auteur	    |❌ O(n) (parcours total)	  |✅ O(1) en moyenne, O(k) en pire cas
🔍 Recherche par numéro	    |✅ O(n) (parcours total)	  |❌ O(n) (recherche dans toute la table)
🔍 Recherche par titre	    |✅ O(n) (parcours total)	  |❌ O(n) (recherche dans toute la table)

Si on cherche par auteur → Table de hachage ✅ (très rapide) car la fonction de hachage utilise "AUTEUR" pour trouver la cle de la table donc on peut y acceder directemnt a la case de table en utilisant la fct de hachage 
Si on cherche par titre ou numéro → Liste chaînée ✅ (car la table de hachage n’aide pas ici) car on n'a pas la cle pour y acceder directemnt a la case de la table donc on parcours chaque case de table et puis la liste chainee de chaue case parcontre la liste chainee on la parcours directement 


3-2//
quand on modifie la longueur de la table om trouve que le temps de trouver le livre diminue car y aura moin de collision dans la table 

3-3//
creer une fonction :mesurer_temps_par_n(int n_max, int pas) et on change a chaque fois
        --> le noms de fichier 
        --> la fonction qu'on veut faire une modification 
puis on cree le graphe :
-->
    dans le Terminal et on tape: gnuplot
    puis:
        set title "Comparaison des temps de recherche"
        set xlabel "Nombre de livres"
        set ylabel "Temps (s)"
        set grid
        plot "resultats.txt" using 1:2 with lines title "Liste chaînée", \
            "resultats.txt" using 1:3 with lines title "Table de hachage"

        set terminal png
        set output "graph.png"
        replot
        exit




*/