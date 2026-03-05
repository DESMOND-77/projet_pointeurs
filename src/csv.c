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
