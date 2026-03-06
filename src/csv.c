/**
 * @file csv.c
 * @brief Implementations des fonctions de lecture du fichier CSV
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "types.h"
#include "memoire.h"
#include "csv.h"

/* Definition des tableaux globaux */
Direction directions[MAX_DEPARTEMENTS];
int nbDirections = 0;

Departement departements[MAX_DEPARTEMENTS];
int nbDepartements = 0;

Classe classes[MAX_CLASSES];
int nbClasses = 0;

/**
 * Supprime les espaces au debut et a la fin d'une chaine
 */
static void trim_csv(char* ch) {
    if (ch == NULL) return;
    
    while (isspace((unsigned char)*ch)) ch++;
    
    if (ch != ch) {
        memmove(ch, ch, strlen(ch) + 1);
    }
    
    char* fin = ch + strlen(ch) - 1;
    while (fin > ch && isspace((unsigned char)*fin)) {
        *fin = '\0';
        fin--;
    }
}

int chargerDirections(const char* nomFichier) {
    if (nomFichier == NULL) {
        printf("ERREUR : Nom de fichier NULL pour les directions\n");
        return -1;
    }
    
    FILE* fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("ERREUR : Impossible d'ouvrir le fichier %s\n", nomFichier);
        return -1;
    }
    
    char ligne[TAILLE_MAX_LIGNE];
    
    /* Lire l'en-tete */
    if (fgets(ligne, sizeof(ligne), fichier) == NULL) {
        printf("ERREUR : Fichier directions vide\n");
        fclose(fichier);
        return -1;
    }
    
    nbDirections = 0;
    
    while (fgets(ligne, sizeof(ligne), fichier) != NULL && nbDirections < MAX_DEPARTEMENTS) {
        size_t len = strlen(ligne);
        if (len > 0 && ligne[len - 1] == '\n') {
            ligne[len - 1] = '\0';
        }
        
        if (strlen(ligne) == 0) continue;
        
        trim_csv(ligne);
        
        strncpy(directions[nbDirections].nom, ligne, 49);
        directions[nbDirections].nom[49] = '\0';
        
        nbDirections++;
    }
    
    fclose(fichier);
    printf("%d directions chargees depuis %s\n", nbDirections, nomFichier);
    return nbDirections;
}

int chargerDepartements(const char* nomFichier) {
    if (nomFichier == NULL) {
        printf("ERREUR : Nom de fichier NULL pour les departements\n");
        return -1;
    }
    
    FILE* fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("ERREUR : Impossible d'ouvrir le fichier %s\n", nomFichier);
        return -1;
    }
    
    char ligne[TAILLE_MAX_LIGNE];
    
    /* Lire l'en-tete */
    if (fgets(ligne, sizeof(ligne), fichier) == NULL) {
        printf("ERREUR : Fichier departements vide\n");
        fclose(fichier);
        return -1;
    }
    
    nbDepartements = 0;
    
    while (fgets(ligne, sizeof(ligne), fichier) != NULL && nbDepartements < MAX_DEPARTEMENTS) {
        size_t len = strlen(ligne);
        if (len > 0 && ligne[len - 1] == '\n') {
            ligne[len - 1] = '\0';
        }
        
        if (strlen(ligne) == 0) continue;
        
        /* Parser: nom;direction */
        char* token = strtok(ligne, ";");
        if (token != NULL) {
            trim_csv(token);
            strncpy(departements[nbDepartements].nom, token, 49);
            departements[nbDepartements].nom[49] = '\0';
            
            token = strtok(NULL, ";");
            if (token != NULL) {
                trim_csv(token);
                strncpy(departements[nbDepartements].direction, token, 49);
                departements[nbDepartements].direction[49] = '\0';
            } else {
                strcpy(departements[nbDepartements].direction, "");
            }
            
            nbDepartements++;
        }
    }
    
    fclose(fichier);
    printf("%d departements charges depuis %s\n", nbDepartements, nomFichier);
    return nbDepartements;
}

int chargerClasses(const char* nomFichier) {
    if (nomFichier == NULL) {
        printf("ERREUR : Nom de fichier NULL pour les classes\n");
        return -1;
    }
    
    FILE* fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("ERREUR : Impossible d'ouvrir le fichier %s\n", nomFichier);
        return -1;
    }
    
    char ligne[TAILLE_MAX_LIGNE];
    
    /* Lire l'en-tete */
    if (fgets(ligne, sizeof(ligne), fichier) == NULL) {
        printf("ERREUR : Fichier classes vide\n");
        fclose(fichier);
        return -1;
    }
    
    nbClasses = 0;
    
    while (fgets(ligne, sizeof(ligne), fichier) != NULL && nbClasses < MAX_CLASSES) {
        size_t len = strlen(ligne);
        if (len > 0 && ligne[len - 1] == '\n') {
            ligne[len - 1] = '\0';
        }
        
        if (strlen(ligne) == 0) continue;
        
        /* Parser: nom;departement */
        char* token = strtok(ligne, ";");
        if (token != NULL) {
            trim_csv(token);
            strncpy(classes[nbClasses].nom, token, 49);
            classes[nbClasses].nom[49] = '\0';
            
            token = strtok(NULL, ";");
            if (token != NULL) {
                trim_csv(token);
                strncpy(classes[nbClasses].departement, token, 49);
                classes[nbClasses].departement[49] = '\0';
            } else {
                strcpy(classes[nbClasses].departement, "");
            }
            
            nbClasses++;
        }
    }
    
    fclose(fichier);
    printf("%d classes chargees depuis %s\n", nbClasses, nomFichier);
    return nbClasses;
}

int verifierDirection(const char* nom) {
    if (nom == NULL) return 0;
    
    for (int i = 0; i < nbDirections; i++) {
        if (strcmp(directions[i].nom, nom) == 0) {
            return 1;
        }
    }
    return 0;
}

int verifierDepartement(const char* nom) {
    if (nom == NULL) return 0;
    
    for (int i = 0; i < nbDepartements; i++) {
        if (strcmp(departements[i].nom, nom) == 0) {
            return 1;
        }
    }
    return 0;
}

int verifierClasse(const char* nom) {
    if (nom == NULL) return 0;
    
    for (int i = 0; i < nbClasses; i++) {
        if (strcmp(classes[i].nom, nom) == 0) {
            return 1;
        }
    }
    return 0;
}

int getDepartementsParDirection(const char* direction, char departementsResultats[][50], int maxResultats) {
    if (direction == NULL || departementsResultats == NULL || maxResultats <= 0) {
        return 0;
    }
    
    int count = 0;
    
    /* Si "tous" est specifie, retourner tous les departements */
    if (strcmp(direction, "tous") == 0 || strcmp(direction, "toutes") == 0) {
        for (int i = 0; i < nbDepartements && count < maxResultats; i++) {
            strncpy(departementsResultats[count], departements[i].nom, 49);
            departementsResultats[count][49] = '\0';
            count++;
        }
        return count;
    }
    
    for (int i = 0; i < nbDepartements && count < maxResultats; i++) {
        if (strcmp(departements[i].direction, direction) == 0) {
            strncpy(departementsResultats[count], departements[i].nom, 49);
            departementsResultats[count][49] = '\0';
            count++;
        }
    }
    
    return count;
}

int getClassesParDepartement(const char* departement, char classesResultats[][50], int maxResultats) {
    if (departement == NULL || classesResultats == NULL || maxResultats <= 0) {
        return 0;
    }
    
    int count = 0;
    
    /* Si "toutes" est specifie, retourner toutes les classes */
    if (strcmp(departement, "toutes") == 0 || strcmp(departement, "tous") == 0) {
        for (int i = 0; i < nbClasses && count < maxResultats; i++) {
            strncpy(classesResultats[count], classes[i].nom, 49);
            classesResultats[count][49] = '\0';
            count++;
        }
        return count;
    }
    
    for (int i = 0; i < nbClasses && count < maxResultats; i++) {
        if (strcmp(classes[i].departement, departement) == 0) {
            strncpy(classesResultats[count], classes[i].nom, 49);
            classesResultats[count][49] = '\0';
            count++;
        }
    }
    
    return count;
}

int lireCSV(const char* nomFichier, Utilisateur* utilisateurs[]) {
    if (nomFichier == NULL || utilisateurs == NULL) {
        printf("ERREUR : Parametres invalides pour la lecture du CSV\n");
        return -1;
    }
    
    printf("Ouverture du fichier CSV : %s\n", nomFichier);
    
    FILE* fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("ERREUR : Impossible d'ouvrir le fichier CSV %s\n", nomFichier);
        return -1;
    }
    
    char ligne[TAILLE_MAX_LIGNE];
    if (fgets(ligne, sizeof(ligne), fichier) == NULL) {
        printf("ERREUR : Fichier CSV vide\n");
        fclose(fichier);
        return -1;
    }
    
    int nbUtilisateursLus = 0;
    int numeroLigne = 1;
    
    while (fgets(ligne, sizeof(ligne), fichier) != NULL && 
           nbUtilisateursLus < MAX_UTILISATEURS) {
        
        numeroLigne++;
        
        size_t longueur = strlen(ligne);
        if (longueur > 0 && ligne[longueur - 1] == '\n') {
            ligne[longueur - 1] = '\0';
        }
        
        if (longueur <= 1) {
            continue;
        }
        
        char* champs[MAX_CHAMPS];
        int nbChamps = 0;
        
        char ligneCopie[TAILLE_MAX_LIGNE];
        strcpy(ligneCopie, ligne);
        
        char* token = strtok(ligneCopie, ";");
        
        while (token != NULL && nbChamps < MAX_CHAMPS) {
            while (isspace((unsigned char)*token)) token++;
            char* fin = token + strlen(token) - 1;
            while (fin > token && isspace((unsigned char)*fin)) fin--;
            fin[1] = '\0';
            
            champs[nbChamps] = token;
            nbChamps++;
            
            token = strtok(NULL, ";");
        }
        
        if (nbChamps < MAX_CHAMPS) {
            printf("AVERTISSEMENT : Ligne %d ignoree (pas assez de champs : %d)\n", 
                   numeroLigne, nbChamps);
            continue;
        }
        
        Utilisateur* nouvelUtilisateur = (Utilisateur*)malloc(sizeof(Utilisateur));
        if (nouvelUtilisateur == NULL) {
            printf("ERREUR : Allocation memoire pour utilisateur (ligne %d)\n", numeroLigne);
            break;
        }
        
        nouvelUtilisateur->nom = NULL;
        nouvelUtilisateur->prenoms = NULL;
        nouvelUtilisateur->departement = NULL;
        nouvelUtilisateur->classe = NULL;
        nouvelUtilisateur->motDePasse = NULL;
        nouvelUtilisateur->statut = NULL;
        
        if (champs[0] != NULL && strlen(champs[0]) > 0) {
            nouvelUtilisateur->nom = dupliquerChaine(champs[0]);
        }
        
        if (champs[1] != NULL && strlen(champs[1]) > 0) {
            nouvelUtilisateur->prenoms = dupliquerChaine(champs[1]);
        }
        
        if (champs[2] != NULL && strlen(champs[2]) > 0) {
            nouvelUtilisateur->departement = dupliquerChaine(champs[2]);
        }
        
        if (champs[3] != NULL && strlen(champs[3]) > 0) {
            nouvelUtilisateur->classe = dupliquerChaine(champs[3]);
        }
        
        if (champs[4] != NULL && strlen(champs[4]) > 0) {
            nouvelUtilisateur->motDePasse = dupliquerChaine(champs[4]);
        }
        
        if (champs[5] != NULL && strlen(champs[5]) > 0) {
            nouvelUtilisateur->statut = dupliquerChaine(champs[5]);
        }
        
        if (nouvelUtilisateur->nom != NULL && strlen(nouvelUtilisateur->nom) > 0) {
            utilisateurs[nbUtilisateursLus] = nouvelUtilisateur;
            nbUtilisateursLus++;
        } else {
            printf("  -> Ligne ignoree : nom manquant\n");
            libererUtilisateur(nouvelUtilisateur);
        }
    }
    
    fclose(fichier);
    printf("Lecture CSV terminee. %d utilisateurs charges.\n", nbUtilisateursLus);
    return nbUtilisateursLus;
}

int ecrireUtilisateurCSV(const char* nomFichier, Utilisateur* utilisateur) {
    if (nomFichier == NULL || utilisateur == NULL) {
        return -1;
    }
    
    FILE* fichier = fopen(nomFichier, "a");
    if (fichier == NULL) {
        return -1;
    }
    
    fprintf(fichier, "%s;%s;%s;%s;%s;%s\n",
            utilisateur->nom ? utilisateur->nom : "",
            utilisateur->prenoms ? utilisateur->prenoms : "",
            utilisateur->departement ? utilisateur->departement : "",
            utilisateur->classe ? utilisateur->classe : "",
            utilisateur->motDePasse ? utilisateur->motDePasse : "",
            utilisateur->statut ? utilisateur->statut : "");
    
    fclose(fichier);
    return 0;
}

int mettreAJourUtilisateurCSV(const char* nomFichier, const char* nomRecherche, Utilisateur* utilisateur) {
    (void)nomFichier;
    (void)nomRecherche;
    (void)utilisateur;
    return -1;
}

int chargerDonnees(void) {
    printf("\n=== Chargement des donnees ===\n");
    
    int total = 0;
    total += chargerDirections("data/directions.csv");
    total += chargerDepartements("data/departements.csv");
    total += chargerClasses("data/classes.csv");
    
    printf("Total: %d entrees chargees\n", total);
    return total;
}

int parserEntrees(const char* entree, char resultats[][50], int maxResultats) {
    int count = 0;
    if (entree == NULL || strlen(entree) == 0) return 0;
    
    char copie[500];
    strncpy(copie, entree, sizeof(copie) - 1);
    copie[sizeof(copie) - 1] = '\0';
    
    char* token = strtok(copie, ",");
    while (token != NULL && count < maxResultats) {
        /* Supprimer les espaces */
        while (isspace((unsigned char)*token)) token++;
        char* fin = token + strlen(token) - 1;
        while (fin > token && isspace((unsigned char)*fin)) {
            *fin = '\0';
            fin--;
        }
        
        if (strlen(token) > 0) {
            strncpy(resultats[count], token, 49);
            resultats[count][49] = '\0';
            count++;
        }
        token = strtok(NULL, ",");
    }
    
    return count;
}
