/**
 * @file utils.h
 * @brief Fonctions utilitaires (saisie, chaines, fichiers)
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "types.h"

/* Vide le buffer d'entree */
void viderBuffer(void);

/* Saisie securisee d'une chaine */
void saisirChaine(const char* message, char* buffer, int taille);

/* Saisie securisee d'un entier */
int saisirEntier(const char* message, int min, int max);

/* Compare deux chaines (insensible a la casse) */
int comparerChaines(const char* chain1, const char* chain2);

/* Supprime les espaces au debut et a la fin */
void trim(char* ch);

/* Verifie si une chaine est vide */
int estVide(const char* ch);

/* Lit une ligne entiere depuis un fichier */
char* lireLigneFichier(FILE* fichier);

/* Ecrit un message dans un fichier (au debut) */
int ecrireDebutFichier(const char* nomFichier, const char* message);

/* Lit tous les messages d'un fichier */
char** lireMessagesFichier(const char* nomFichier, int* nbMessages);

/* Affiche les messages d'un fichier */
void afficherMessages(const char* nomFichier);

/* Cree un fichier vide si inexistant */
int creerFichierSiInexistant(const char* nomFichier);

#endif

