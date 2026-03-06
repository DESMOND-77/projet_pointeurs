/**
 * @file auth.c
 * @brief Implementations des fonctions d'authentification
 */

#include <string.h>
#include <stdio.h>
#include "types.h"
#include "auth.h"
#include "memoire.h"

ResultatVerification verifierIdentifiants(Utilisateur* utilisateurs[], 
                                          int nbUtilisateurs,
                                          const char* usernameSaisi,
                                          const char* mdpSaisi) {
    
    ResultatVerification resultat;
    resultat.trouve = 0;
    resultat.utilisateur = NULL;
    resultat.message[0] = '\0';
    
    if (usernameSaisi == NULL || usernameSaisi[0] == '\0') {
        strcpy(resultat.message, " Le username est obligatoire.");
        return resultat;
    }
    
    if (mdpSaisi == NULL || mdpSaisi[0] == '\0') {
        strcpy(resultat.message, " Le mot de passe est obligatoire.");
        return resultat;
    }
    
    for (int i = 0; i < nbUtilisateurs; i++) {
        Utilisateur* u = utilisateurs[i];
        
        if (u == NULL) continue;
        
        if (u->username != NULL && strcmp(u->username, usernameSaisi) == 0) {
            printf("[DEBUG] %s vs %s\t%s vs %s\n", u->username, usernameSaisi, u->motDePasse, mdpSaisi);
            if (u->motDePasse != NULL && strcmp(u->motDePasse, mdpSaisi) == 0) {
                resultat.trouve = 1;
                resultat.utilisateur = u;
                strcpy(resultat.message, " Connexion reussie !");
                return resultat;
            }
        }
    }
    
    strcpy(resultat.message, " Identifiants incorrects. Veuillez reessayer.");
    return resultat;
}

void afficherProfil(Utilisateur* user) {
    if (user == NULL) return;
    
    printf("\n");
    printf("========================================\n");
    printf("         PROFIL UTILISATEUR             \n");
    printf("========================================\n");
    printf("Username    : %s\n", user->username ? user->username : "");
    printf("Nom complet : %s %s\n", 
           user->prenoms ? user->prenoms : "", 
           user->nom ? user->nom : "");
    printf("Departement: %s\n", user->departement ? user->departement : "Non specifie");
    printf("Classe      : %s\n", user->classe ? user->classe : "Non specifie");
    printf("Statut      : %s\n", user->statut ? user->statut : "Non specifie");
    printf("========================================\n");
}

StatutUtilisateur getStatutFromString(const char* statut) {
    if (statut == NULL) return STATUT_INCONNU;
    
    if (strcmp(statut, "Etudiant") == 0 || strcmp(statut, "ETUDIANT") == 0) {
        return STATUT_ETUDIANT;
    }
    if (strcmp(statut, "Professeur") == 0 || strcmp(statut, "PROFESSEUR") == 0) {
        return STATUT_PROFESSEUR;
    }
    if (strcmp(statut, "ChefDepartement") == 0 || strcmp(statut, "CHEF_DEPARTEMENT") == 0) {
        return STATUT_CHEF_DEP;
    }
    if (strcmp(statut, "Administratif") == 0 || strcmp(statut, "ADMINISTRATIF") == 0) {
        return STATUT_ADMINISTRATIF;
    }
    if (strcmp(statut, "Direction") == 0 || strcmp(statut, "DIRECTION") == 0) {
        return STATUT_DIRECTION;
    }
    
    return STATUT_INCONNU;
}

const char* getNomFichierDestination(TypeDestination type, const char* nom) {
    static char chemin[200];
    
    if (nom == NULL) return NULL;
    
    switch (type) {
        case TYPE_DIRECTION:
            snprintf(chemin, sizeof(chemin), "data/direction_%s.txt", nom);
            break;
        case TYPE_DEPARTEMENT:
            snprintf(chemin, sizeof(chemin), "data/departement_%s.txt", nom);
            break;
        case TYPE_CLASSE:
            snprintf(chemin, sizeof(chemin), "data/classe_%s.txt", nom);
            break;
        default:
            return NULL;
    }
    
    return chemin;
}

