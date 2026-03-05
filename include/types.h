/**
 * @file types.h
 * @brief Definitions des types et structures du systeme de messagerie
 */

#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>

/* ==================== CONSTANTES ==================== */
#define TAILLE_MAX_LIGNE      2000
#define MAX_UTILISATEURS      1000
#define MAX_CHAMPS            6
#define TAILLE_MAX_SAISIE     1000
#define MAX_DEPARTEMENTS      10
#define MAX_CLASSES           20
#define MAX_DESTINATAIRES     15

/* ==================== TYPES ENUMERES ==================== */
typedef enum {
    STATUT_INCONNU = 0,
    STATUT_ETUDIANT,
    STATUT_PROFESSEUR,
    STATUT_CHEF_DEP,
    STATUT_ADMINISTRATIF,
    STATUT_DIRECTION
} StatutUtilisateur;

typedef enum {
    TYPE_DIRECTION = 0,
    TYPE_DEPARTEMENT,
    TYPE_CLASSE
} TypeDestination;

/* ==================== STRUCTURES ==================== */
typedef struct {
    char nom[50];
    char departement[50];
} Classe;

typedef struct {
    char nom[50];
    char direction[20];
} Departement;

typedef struct {
    char nom[20];
} Direction;

typedef struct {
    char* nom;
    char* prenoms;
    char* departement;
    char* classe;
    char* motDePasse;
    char* statut;
} Utilisateur;

typedef struct {
    int trouve;
    Utilisateur* utilisateur;
    char message[TAILLE_MAX_SAISIE];
} ResultatVerification;

typedef struct {
    char expediteur[100];
    char contenu[TAILLE_MAX_SAISIE];
    char timestamp[30];
} Message;

typedef struct {
    TypeDestination type;
    char nom[50];
} Destination;

#endif

