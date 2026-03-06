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

/* Charge les directions depuis le fichier CSV */
int chargerDirections(const char* nomFichier);

/* Charge les departements depuis le fichier CSV */
int chargerDepartements(const char* nomFichier);

/* Charge les classes depuis le fichier CSV */
int chargerClasses(const char* nomFichier);

/* Verifie si une direction existe */
int verifierDirection(const char* nom);

/* Verifie si un departement existe */
int verifierDepartement(const char* nom);

/* Verifie si une classe existe */
int verifierClasse(const char* nom);

/* Retourne les departements d'une direction donnee */
int getDepartementsParDirection(const char* direction, char departementsResultats[][50], int maxResultats);

/* Retourne les classes d'un departement donne */
int getClassesParDepartement(const char* departement, char classesResultats[][50], int maxResultats);

/* Charge toutes les donnees (directions, departements, classes) */
int chargerDonnees(void);

/* Parse une chaine separee par des virgules */
int parserEntrees(const char* entree, char resultats[][50], int maxResultats);

#endif

