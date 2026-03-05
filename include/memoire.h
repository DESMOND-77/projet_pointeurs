/**
 * @file memoire.h
 * @brief Fonctions de gestion de la memoire
 */

#ifndef MEMOIRE_H
#define MEMOIRE_H

#include <stdlib.h>

/* Duplique une chaine de caracteres */
char* dupliquerChaine(const char* source);

/* Libere la memoire d un utilisateur */
void libererUtilisateur(void* utilisateur);

/* Alloue et initialise une structure Utilisateur */
void* allouerUtilisateur(void);

/* Libere un tableau d utilisateurs */
void libererTableauUtilisateurs(void* utilisateurs[], int nbUtilisateurs);

#endif

