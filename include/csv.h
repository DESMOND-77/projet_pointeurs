/**
 * @file csv.h
 * @brief Fonctions de lecture du fichier CSV
 */

#ifndef CSV_H
#define CSV_H

#include "types.h"

/* Lit le fichier CSV et charge les utilisateurs */
int lireCSV(const char* nomFichier, Utilisateur* utilisateurs[]);

/* Ecrit un nouvel utilisateur dans le fichier CSV */
int ecrireUtilisateurCSV(const char* nomFichier, Utilisateur* utilisateur);

/* Met a jour un utilisateur dans le fichier CSV */
int mettreAJourUtilisateurCSV(const char* nomFichier, const char* nomRecherche, Utilisateur* utilisateur);

#endif

