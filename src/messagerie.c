/**
 * @file messagerie.c
 * @brief Implementations des fonctions du systeme de messagerie
 */

#include <string.h>
#include <stdio.h>
#include <time.h>
#include "types.h"
#include "messagerie.h"
#include "utils.h"
#include "auth.h"

int peutEnvoyer(StatutUtilisateur statut, TypeDestination typeDest, const char* nomDest, 
                const char* departementUser, const char* classeUser) {
    
    if (nomDest == NULL) return 0;
    
    switch (statut) {
        case STATUT_ETUDIANT:
            /* Etudiant: peut envoyer uniquement a sa classe */
            if (typeDest == TYPE_CLASSE && classeUser != NULL) {
                return strcmp(nomDest, classeUser) == 0;
            }
            return 0;
            
        case STATUT_PROFESSEUR:
            /* Professeur: peut envoyer a tous sauf Direction */
            if (typeDest == TYPE_DIRECTION) {
                return 0;
            }
            return 1;
            
        case STATUT_CHEF_DEP:
            /* Chef de departement: peut envoyer a tous */
            return 1;
            
        case STATUT_ADMINISTRATIF:
            /* Administratif: peut envoyer a tous */
            return 1;
            
        case STATUT_DIRECTION:
            /* Direction: peut envoyer a tous */
            return 1;
            
        default:
            return 0;
    }
}

void listerDestinataires(StatutUtilisateur statut, const char* departement, const char* classe) {
    printf("\n--- Destinataires disponibles ---\n");
    
    switch (statut) {
        case STATUT_ETUDIANT:
            printf("Votre classe: %s\n", classe ? classe : "Non defini");
            break;
            
        case STATUT_PROFESSEUR:
            printf("- Classes: %s\n", classe ? classe : "Non defini");
            printf("- Departements: Tous (sauf Direction)\n");
            break;
            
        case STATUT_CHEF_DEP:
            printf("- Direction\n");
            printf("- Tous les departements\n");
            printf("- Toutes les classes\n");
            break;
            
        case STATUT_ADMINISTRATIF:
            printf("- Direction\n");
            printf("- Tous les departements\n");
            printf("- Toutes les classes\n");
            break;
            
        case STATUT_DIRECTION:
            printf("- Tous les departements\n");
            printf("- Toutes les classes\n");
            break;
            
        default:
            printf("Aucun destinataire disponible\n");
    }
    printf("------------------------------------\n");
}

void formaterMessage(Message* msg, const char* expediteur, const char* contenu) {
    if (msg == NULL || expediteur == NULL || contenu == NULL) return;
    
    strncpy(msg->expediteur, expediteur, sizeof(msg->expediteur) - 1);
    msg->expediteur[sizeof(msg->expediteur) - 1] = '\0';
    
    strncpy(msg->contenu, contenu, sizeof(msg->contenu) - 1);
    msg->contenu[sizeof(msg->contenu) - 1] = '\0';
    
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    strftime(msg->timestamp, sizeof(msg->timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
}

int sauvegarderMessage(Message* msg, Destination* dest) {
    if (msg == NULL || dest == NULL) return -1;
    
    const char* fichier = NULL;
    
    if (dest->type == TYPE_DIRECTION) {
        fichier = "data/direction.txt";
    } else if (dest->type == TYPE_DEPARTEMENT) {
        static char chemin[150];
        snprintf(chemin, sizeof(chemin), "data/departement_%s.txt", dest->nom);
        fichier = chemin;
    } else if (dest->type == TYPE_CLASSE) {
        static char chemin[150];
        snprintf(chemin, sizeof(chemin), "data/classe_%s.txt", dest->nom);
        fichier = chemin;
    }
    
    if (fichier == NULL) return -1;
    
    char messageFormate[TAILLE_MAX_LIGNE];
    snprintf(messageFormate, sizeof(messageFormate), 
             "[%s] De: %s\n%s\n-------------------------------\n",
             msg->timestamp, msg->expediteur, msg->contenu);
    
    return ecrireDebutFichier(fichier, messageFormate);
}

int envoyerMessage(Utilisateur* expediteur, Destination destinations[], int nbDestinations, const char* message) {
    if (expediteur == NULL || destinations == NULL || message == NULL) {
        return -1;
    }
    
    if (nbDestinations <= 0) {
        printf("Aucun destinataire selectionne.\n");
        return -1;
    }
    
    StatutUtilisateur statut = getStatutFromString(expediteur->statut);
    
    /* Verifier les permissions pour chaque destination */
    for (int i = 0; i < nbDestinations; i++) {
        if (!peutEnvoyer(statut, destinations[i].type, destinations[i].nom,
                        expediteur->departement, expediteur->classe)) {
            printf("Vous n'etes pas autorise a envoyer a %s\n", destinations[i].nom);
            return -1;
        }
    }
    
    /* Preparer et envoyer le message */
    char nomComplet[200];
    snprintf(nomComplet, sizeof(nomComplet), "%s %s",
             expediteur->prenoms ? expediteur->prenoms : "",
             expediteur->nom ? expediteur->nom : "");
    
    Message msg;
    formaterMessage(&msg, nomComplet, message);
    
    int succes = 0;
    for (int i = 0; i < nbDestinations; i++) {
        if (sauvegarderMessage(&msg, &destinations[i]) == 0) {
            printf("Message envoye a %s\n", destinations[i].nom);
            succes++;
        } else {
            printf("Echec de l'envoi a %s\n", destinations[i].nom);
        }
    }
    
    return succes > 0 ? succes : -1;
}

void lireMessages(Utilisateur* utilisateur) {
    if (utilisateur == NULL) return;
    
    StatutUtilisateur statut = getStatutFromString(utilisateur->statut);
    
    printf("\n=== MES MESSAGES ===\n");
    
    char fichier[150];
    
    /* Lire les messages selon le type d'utilisateur */
    if (statut == STATUT_ETUDIANT && utilisateur->classe != NULL) {
        snprintf(fichier, sizeof(fichier), "data/classe_%s.txt", utilisateur->classe);
        afficherMessages(fichier);
    } else if (statut == STATUT_PROFESSEUR || statut == STATUT_CHEF_DEP) {
        if (utilisateur->departement != NULL) {
            snprintf(fichier, sizeof(fichier), "data/departement_%s.txt", utilisateur->departement);
            afficherMessages(fichier);
        }
    }
    
    /* Lire les messages de la direction */
    afficherMessages("data/direction.txt");
    
    printf("====================\n");
}

void afficherNouveauxMessages(const char* nomFichier) {
    (void)nomFichier;
    printf("Fonction a implementer: nouveaux messages\n");
}
