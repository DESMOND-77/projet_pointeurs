/**
 * @file messagerie.h
 * @brief Fonctions du systeme de messagerie
 */

#ifndef MESSAGERIE_H
#define MESSAGERIE_H

#include "types.h"

/* Verifie si l'utilisateur peut envoyer a la destination */
int peutEnvoyer(StatutUtilisateur statut, TypeDestination typeDest, const char* nomDest, const char* departementUser, const char* classeUser);

/* Liste les destinataires disponibles pour un utilisateur */
void listerDestinataires(StatutUtilisateur statut, const char* departement, const char* classe);

/* Envoie un message a un ou plusieurs destinataires */
int envoyerMessage(Utilisateur* expediteur, Destination destinations[], int nbDestinations, const char* message);

/* Lit les messages d'un utilisateur */
void lireMessages(Utilisateur* utilisateur);

/* Affiche les messages non lus */
void afficherNouveauxMessages(const char* nomFichier);

/* Prepare le format du message */
void formaterMessage(Message* msg, const char* expediteur, const char* contenu);

/* Sauvegarde le message dans les fichiers de destination */
int sauvegarderMessage(Message* msg, Destination* dest);

#endif
