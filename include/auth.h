/**
 * @file auth.h
 * @brief Fonctions d'authentification
 */

#ifndef AUTH_H
#define AUTH_H

#include "types.h"

/* Verifie les identifiants de l'utilisateur */
ResultatVerification verifierIdentifiants(Utilisateur* utilisateurs[], int nbUtilisateurs, const char* nomSaisi, const char* mdpSaisi);

/* Affiche le profil d'un utilisateur */
void afficherProfil(Utilisateur* user);

/* Convertit le statut en enum */
StatutUtilisateur getStatutFromString(const char* statut);

/* Obtient le type de fichier de destination */
const char* getNomFichierDestination(TypeDestination type, const char* nom);

#endif
