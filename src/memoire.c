/**
 * @file memoire.c
 * @brief Implementations des fonctions de gestion de la memoire
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#include "memoire.h"

char* dupliquerChaine(const char* source) {
    if (source == NULL) {
        return NULL;
    }
    
    char* nouvelleChaine = (char*)malloc((strlen(source) + 1) * sizeof(char));
    if (nouvelleChaine == NULL) {
        printf("ERREUR : Impossible d'allouer la memoire\n");
        return NULL;
    }
    
    strcpy(nouvelleChaine, source);
    return nouvelleChaine;
}

void libererUtilisateur(void* utilisateur) {
    if (utilisateur == NULL) {
        return;
    }
    
    Utilisateur* u = (Utilisateur*)utilisateur;
    free(u->nom);
    free(u->prenoms);
    free(u->departement);
    free(u->classe);
    free(u->motDePasse);
    free(u->statut);
    free(utilisateur);
}

void* allouerUtilisateur(void) {
    Utilisateur* utilisateur = (Utilisateur*)malloc(sizeof(Utilisateur));
    if (utilisateur == NULL) {
        return NULL;
    }
    
    utilisateur->nom = NULL;
    utilisateur->prenoms = NULL;
    utilisateur->departement = NULL;
    utilisateur->classe = NULL;
    utilisateur->motDePasse = NULL;
    utilisateur->statut = NULL;
    
    return utilisateur;
}

void libererTableauUtilisateurs(void* utilisateurs[], int nbUtilisateurs) {
    if (utilisateurs == NULL) {
        return;
    }
    
    for (int i = 0; i < nbUtilisateurs; i++) {
        if (utilisateurs[i] != NULL) {
            libererUtilisateur(utilisateurs[i]);
            utilisateurs[i] = NULL;
        }
    }
}
