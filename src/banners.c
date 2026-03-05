/**
 * @file banners.c
 * @brief Implementations des fonctions d'affichage des bannieres
 */

#include <stdio.h>
#include "banners.h"

void afficherBanniereAccueil(void) {
    printf("\n");
    printf("========================================\n");
    printf("     SYSTEME DE MESSAGERIE INTERNE      \n");
    printf("========================================\n");
    printf("     Transmission d'information          \n");
    printf("     en temps reel                        \n");
    printf("========================================\n");
    printf("\n");
}

void afficherBanniereConnexion(void) {
    printf("\n");
    printf("========================================\n");
    printf("           CONNEXION                    \n");
    printf("========================================\n");
}

void afficherMenuPrincipal(void) {
    printf("\n");
    printf("========== MENU PRINCIPAL ==========\n");
    printf("1. Envoyer un message\n");
    printf("2. Lire mes messages\n");
    printf("3. Voir mes informations\n");
    printf("0. Deconnexion\n");
    printf("====================================\n");
    printf("Votre choix: ");
}

void afficherMenuEtudiant(void) {
    printf("\n");
    printf("============= MENU ETUDIANT ===========\n");
    printf("1. Envoyer un message a ma classe\n");
    printf("2. Lire mes messages\n");
    printf("3. Voir mes informations\n");
    printf("0. Deconnexion\n");
    printf("======================================\n");
    printf("Votre choix: ");
}

void afficherMenuProfesseur(void) {
    printf("\n");
    printf("============= MENU PROFESSEUR ===========\n");
    printf("1. Envoyer un message\n");
    printf("   - A ma classe\n");
    printf("   - A mon departement\n");
    printf("   - A d'autres departements\n");
    printf("2. Lire mes messages\n");
    printf("3. Voir mes informations\n");
    printf("0. Deconnexion\n");
    printf("========================================\n");
    printf("Votre choix: ");
}

void afficherMenuChefDepartement(void) {
    printf("\n");
    printf("====== MENU CHEF DEPARTEMENT ======\n");
    printf("1. Envoyer un message\n");
    printf("   - A la Direction\n");
    printf("   - A tous les departements\n");
    printf("   - A des classes специфик\n");
    printf("2. Lire mes messages\n");
    printf("3. Voir mes informations\n");
    printf("0. Deconnexion\n");
    printf("====================================\n");
    printf("Votre choix: ");
}

void afficherMenuAdministratif(void) {
    printf("\n");
    printf("========= MENU ADMINISTRATIF =========\n");
    printf("1. Envoyer un message\n");
    printf("   - A la Direction\n");
    printf("   - A tous les departements\n");
    printf("   - A toutes les classes\n");
    printf("2. Lire mes messages\n");
    printf("3. Voir mes informations\n");
    printf("0. Deconnexion\n");
    printf("======================================\n");
    printf("Votre choix: ");
}

void afficherMenuDirection(void) {
    printf("\n");
    printf("============ MENU DIRECTION ==========\n");
    printf("1. Envoyer un message\n");
    printf("   - A tous les departements\n");
    printf("   - A toutes les classes\n");
    printf("2. Lire mes messages\n");
    printf("3. Voir mes informations\n");
    printf("0. Deconnexion\n");
    printf("======================================\n");
    printf("Votre choix: ");
}

void afficherSeparateur(void) {
    printf("\n----------------------------------------\n");
}

void afficherMessageDeconnexion(void) {
    printf("\n");
    printf("========================================\n");
    printf("      DECONNEXION EFFECTUEE            \n");
    printf("      A tres vite!                     \n");
    printf("========================================\n");
    printf("\n");
}
