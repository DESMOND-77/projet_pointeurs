/**
 * @file main.c
 * @brief Programme principal du systeme de messagerie
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "memoire.h"
#include "csv.h"
#include "utils.h"
#include "banners.h"
#include "auth.h"
#include "messagerie.h"
#include "os_compat.h"

/* Nombre maximum de tentatives de connexion */
#define MAX_TENTATIVES 3

/* Variable globale pour le fichier CSV */
static const char *FICHIER_CSV = "data/utilisateurs.csv";

/**
 * Parse les entrees separees par des virgules
 */
static int parserEntreesLocal(const char* saisie, char entrees[][50], int maxEntrees) {
    if (saisie == NULL || entrees == NULL || maxEntrees <= 0) {
        return 0;
    }
    
    char copie[TAILLE_MAX_SAISIE];
    strncpy(copie, saisie, sizeof(copie) - 1);
    copie[sizeof(copie) - 1] = '\0';
    
    int count = 0;
    char* token = strtok(copie, ",");
    
    while (token != NULL && count < maxEntrees) {
        /* Supprimer les espaces */
        while (isspace((unsigned char)*token)) token++;
        char* fin = token + strlen(token) - 1;
        while (fin > token && isspace((unsigned char)*fin)) {
            *fin = '\0';
            fin--;
        }
        
        if (strlen(token) > 0) {
            strncpy(entrees[count], token, 49);
            entrees[count][49] = '\0';
            count++;
        }
        token = strtok(NULL, ",");
    }
    
    return count;
}

/**
 * Cree un utilisateur de test en cas d'absence de fichier CSV
 */
int creerUtilisateurTest(Utilisateur *utilisateurs[])
{
    printf("\nCreation d'un utilisateur de test pour le debogage...\n");

    Utilisateur *test = (Utilisateur *)malloc(sizeof(Utilisateur));
    if (test == NULL)
    {
        printf("ERREUR : Impossible de creer l'utilisateur de test\n");
        return 0;
    }

    test->nom = dupliquerChaine("Admin");
    test->prenoms = dupliquerChaine("System");
    test->departement = dupliquerChaine("Informatique");
    test->classe = dupliquerChaine("");
    test->motDePasse = dupliquerChaine("admin123");
    test->statut = dupliquerChaine("Administratif");

    utilisateurs[0] = test;

    printf("Utilisateur de test cree : Admin System / admin123\n");
    return 1;
}

/**
 * Initialise les fichiers de donnees
 */
void initialiserFichiers(void)
{
/* Creer le repertoire data si inexistant */
#ifdef OS_WINDOWS
    system("if not exist data mkdir data");
#else
    system("mkdir -p data");
#endif

    /* Creer les fichiers de base */
    creerFichierSiInexistant("data/direction.txt");
    creerFichierSiInexistant("data/departement_informatique.txt");
    creerFichierSiInexistant("data/departement_mathematiques.txt");
    creerFichierSiInexistant("data/departement_physique.txt");
    creerFichierSiInexistant("data/classe_gl.txt");
    creerFichierSiInexistant("data/classe_geii.txt");
}

/**
 * Affiche le menu selon le statut de l'utilisateur
 */
void afficherMenu(StatutUtilisateur statut)
{
    switch (statut)
    {
    case STATUT_ETUDIANT:
        afficherMenuEtudiant();
        break;
    case STATUT_PROFESSEUR:
        afficherMenuProfesseur();
        break;
    case STATUT_CHEF_DEP:
        afficherMenuChefDepartement();
        break;
    case STATUT_ADMINISTRATIF:
        afficherMenuAdministratif();
        break;
    case STATUT_DIRECTION:
        afficherMenuDirection();
        break;
    default:
        afficherMenuPrincipal();
    }
}

/**
 * Gere l'envoi de message selon le statut
 */
void gererEnvoiMessage(Utilisateur *utilisateur)
{
    if (utilisateur == NULL)
        return;

    StatutUtilisateur statut = getStatutFromString(utilisateur->statut);

    /* Afficher les destinataires disponibles */
    listerDestinataires(statut, utilisateur->departement, utilisateur->classe);

    /* Saisie du message */
    char message[TAILLE_MAX_SAISIE];
    printf("\n=== ENVOI DE MESSAGE ===\n");
    printf("Entrez votre message:\n");
    saisirChaine("> ", message, sizeof(message));

    if (estVide(message))
    {
        printf("Message vide. Annulation.\n");
        return;
    }

    /* Preparation des destinations */
    Destination destinations[MAX_DESTINATAIRES];
    int nbDestinations = 0;

    printf("\nSelectionnez les destinataires (entrez le numero, 0 pour terminer):\n");

    switch (statut)
    {
    case STATUT_ETUDIANT:
        if (utilisateur->classe != NULL && strlen(utilisateur->classe) > 0)
        {
            printf("1. Ma classe (%s)\n", utilisateur->classe);
            int choix = saisirEntier("Choix: ", 0, 1);
            if (choix == 1)
            {
                destinations[nbDestinations].type = TYPE_CLASSE;
                strcpy(destinations[nbDestinations].nom, utilisateur->classe);
                nbDestinations++;
            }
        }
        break;

    case STATUT_PROFESSEUR:
        printf("1. Classe(s) specifique(s)\n");
        printf("2. Departement(s)\n");
        int choixP = saisirEntier("Choix: ", 0, 2);
        
        if (choixP == 1)
        {
            /* Classe(s) specifique(s) - format: filiere_niveau (ex: grt_2,em_4) */
            printf("Entrez le(s) nom(s) de classe(s) separes par des virgules (ex: gl_2,geii_4):\n");
            char classesSaisies[TAILLE_MAX_SAISIE];
            char entrees[20][50];
            saisirChaine("> ", classesSaisies, sizeof(classesSaisies));
            
            int nbEntrees = parserEntrees(classesSaisies, entrees, 20);
            int classesValides = 0;
            
            for (int i = 0; i < nbEntrees; i++)
            {
                if (verifierClasse(entrees[i]))
                {
                    destinations[nbDestinations].type = TYPE_CLASSE;
                    strcpy(destinations[nbDestinations].nom, entrees[i]);
                    nbDestinations++;
                    classesValides++;
                }
                else
                {
                    printf("Classe '%s' introuvable.\n", entrees[i]);
                }
            }
            
            if (classesValides == 0)
            {
                printf("Aucune classe valide specifiee.\n");
                nbDestinations = 0;
            }
        }
        else if (choixP == 2)
        {
            /* Departement(s) - format: dept_filiere (ex: dept_grt,dept_gim) */
            printf("Entrez le(s) nom(s) de departement(s) separes par des virgules (ex: dept_informatique,dept_mathematiques):\n");
            char depsSaisis[TAILLE_MAX_SAISIE];
            char entrees[20][50];
            saisirChaine("> ", depsSaisis, sizeof(depsSaisis));
            
            int nbEntrees = parserEntrees(depsSaisis, entrees, 20);
            int depsValides = 0;
            
            for (int i = 0; i < nbEntrees; i++)
            {
                if (verifierDepartement(entrees[i]))
                {
                    destinations[nbDestinations].type = TYPE_DEPARTEMENT;
                    strcpy(destinations[nbDestinations].nom, entrees[i]);
                    nbDestinations++;
                    depsValides++;
                }
                else
                {
                    printf("Departement '%s' introuvable.\n", entrees[i]);
                }
            }
            
            if (depsValides == 0)
            {
                printf("Aucun departement valide specifie.\n");
                nbDestinations = 0;
            }
        }
        break;

    case STATUT_CHEF_DEP:
        printf("1. Direction\n");
        printf("2. Departement(s) d'une direction\n");
        printf("3. Classe(s) d'un departement\n");
        int choixCD = saisirEntier("Choix: ", 0, 3);
        
        if (choixCD == 1)
        {
            /* Direction - user specifies direction name */
            printf("Entrez le nom de la direction:\n");
            char directionSaisie[50];
            saisirChaine("> ", directionSaisie, sizeof(directionSaisie));
            
            if (verifierDirection(directionSaisie))
            {
                destinations[nbDestinations].type = TYPE_DIRECTION;
                strcpy(destinations[nbDestinations].nom, directionSaisie);
                nbDestinations++;
            }
            else
            {
                printf("Direction '%s' introuvable.\n", directionSaisie);
            }
        }
        else if (choixCD == 2)
        {
            /* Departement(s) d'une direction */
            printf("Entrez le nom de la direction:\n");
            char directionSaisie[50];
            saisirChaine("> ", directionSaisie, sizeof(directionSaisie));
            
            if (!verifierDirection(directionSaisie))
            {
                printf("Direction '%s' introuvable.\n", directionSaisie);
                break;
            }
            
            printf("Entrez le(s) departement(s) ou 'tous' pour tous les departements de cette direction:\n");
            char depsSaisis[TAILLE_MAX_SAISIE];
            char entrees[20][50];
            saisirChaine("> ", depsSaisis, sizeof(depsSaisis));
            
            if (strcmp(depsSaisis, "tous") == 0 || strcmp(depsSaisis, "Tous") == 0)
            {
                /* Tous les departements de la direction */
                int nbDepts = getDepartementsParDirection(directionSaisie, entrees, 20);
                for (int i = 0; i < nbDepts; i++)
                {
                    destinations[nbDestinations].type = TYPE_DEPARTEMENT;
                    strcpy(destinations[nbDestinations].nom, entrees[i]);
                    nbDestinations++;
                }
            }
            else
            {
                int nbEntrees = parserEntrees(depsSaisis, entrees, 20);
                int depsValides = 0;
                
                for (int i = 0; i < nbEntrees; i++)
                {
                    if (verifierDepartement(entrees[i]))
                    {
                        destinations[nbDestinations].type = TYPE_DEPARTEMENT;
                        strcpy(destinations[nbDestinations].nom, entrees[i]);
                        nbDestinations++;
                        depsValides++;
                    }
                    else
                    {
                        printf("Departement '%s' introuvable.\n", entrees[i]);
                    }
                }
                
                if (depsValides == 0)
                {
                    printf("Aucun departement valide specifie.\n");
                    nbDestinations = 0;
                }
            }
        }
        else if (choixCD == 3)
        {
            /* Classe(s) d'un departement */
            printf("Entrez le nom du departement:\n");
            char deptSaisie[50];
            saisirChaine("> ", deptSaisie, sizeof(deptSaisie));
            
            if (!verifierDepartement(deptSaisie))
            {
                printf("Departement '%s' introuvable.\n", deptSaisie);
                break;
            }
            
            printf("Entrez le(s) classe(s) ou 'toutes' pour toutes les classes de ce departement:\n");
            char classesSaisies[TAILLE_MAX_SAISIE];
            char entrees[20][50];
            saisirChaine("> ", classesSaisies, sizeof(classesSaisies));
            
            if (strcmp(classesSaisies, "toutes") == 0 || strcmp(classesSaisies, "Toutes") == 0)
            {
                /* Toutes les classes du departement */
                int nbClasses = getClassesParDepartement(deptSaisie, entrees, 20);
                for (int i = 0; i < nbClasses; i++)
                {
                    destinations[nbDestinations].type = TYPE_CLASSE;
                    strcpy(destinations[nbDestinations].nom, entrees[i]);
                    nbDestinations++;
                }
            }
            else
            {
                int nbEntrees = parserEntrees(classesSaisies, entrees, 20);
                int classesValides = 0;
                
                for (int i = 0; i < nbEntrees; i++)
                {
                    if (verifierClasse(entrees[i]))
                    {
                        destinations[nbDestinations].type = TYPE_CLASSE;
                        strcpy(destinations[nbDestinations].nom, entrees[i]);
                        nbDestinations++;
                        classesValides++;
                    }
                    else
                    {
                        printf("Classe '%s' introuvable.\n", entrees[i]);
                    }
                }
                
                if (classesValides == 0)
                {
                    printf("Aucune classe valide specifiee.\n");
                    nbDestinations = 0;
                }
            }
        }
        break;

    case STATUT_ADMINISTRATIF:
    case STATUT_DIRECTION:
        printf("1. Direction\n");
        printf("2. Departement(s) d'une direction\n");
        printf("3. Classe(s) d'un departement\n");
        int choixA = saisirEntier("Choix: ", 0, 3);
        
        if (choixA == 1)
        {
            /* Direction - user specifies direction name */
            printf("Entrez le nom de la direction:\n");
            char directionSaisie[50];
            saisirChaine("> ", directionSaisie, sizeof(directionSaisie));
            
            if (verifierDirection(directionSaisie))
            {
                destinations[nbDestinations].type = TYPE_DIRECTION;
                strcpy(destinations[nbDestinations].nom, directionSaisie);
                nbDestinations++;
            }
            else
            {
                printf("Direction '%s' introuvable.\n", directionSaisie);
            }
        }
        else if (choixA == 2)
        {
            /* Departement(s) d'une direction */
            printf("Entrez le nom de la direction:\n");
            char directionSaisie[50];
            saisirChaine("> ", directionSaisie, sizeof(directionSaisie));
            
            if (!verifierDirection(directionSaisie))
            {
                printf("Direction '%s' introuvable.\n", directionSaisie);
                break;
            }
            
            printf("Entrez le(s) departement(s) ou 'tous' pour tous les departements de cette direction:\n");
            char depsSaisis[TAILLE_MAX_SAISIE];
            char entrees[20][50];
            saisirChaine("> ", depsSaisis, sizeof(depsSaisis));
            
            if (strcmp(depsSaisis, "tous") == 0 || strcmp(depsSaisis, "Tous") == 0)
            {
                /* Tous les departements de la direction */
                int nbDepts = getDepartementsParDirection(directionSaisie, entrees, 20);
                for (int i = 0; i < nbDepts; i++)
                {
                    destinations[nbDestinations].type = TYPE_DEPARTEMENT;
                    strcpy(destinations[nbDestinations].nom, entrees[i]);
                    nbDestinations++;
                }
            }
            else
            {
                int nbEntrees = parserEntrees(depsSaisis, entrees, 20);
                int depsValides = 0;
                
                for (int i = 0; i < nbEntrees; i++)
                {
                    if (verifierDepartement(entrees[i]))
                    {
                        destinations[nbDestinations].type = TYPE_DEPARTEMENT;
                        strcpy(destinations[nbDestinations].nom, entrees[i]);
                        nbDestinations++;
                        depsValides++;
                    }
                    else
                    {
                        printf("Departement '%s' introuvable.\n", entrees[i]);
                    }
                }
                
                if (depsValides == 0)
                {
                    printf("Aucun departement valide specifie.\n");
                    nbDestinations = 0;
                }
            }
        }
        else if (choixA == 3)
        {
            /* Classe(s) d'un departement */
            printf("Entrez le nom du departement:\n");
            char deptSaisie[50];
            saisirChaine("> ", deptSaisie, sizeof(deptSaisie));
            
            if (!verifierDepartement(deptSaisie))
            {
                printf("Departement '%s' introuvable.\n", deptSaisie);
                break;
            }
            
            printf("Entrez le(s) classe(s) ou 'toutes' pour toutes les classes de ce departement:\n");
            char classesSaisies[TAILLE_MAX_SAISIE];
            char entrees[20][50];
            saisirChaine("> ", classesSaisies, sizeof(classesSaisies));
            
            if (strcmp(classesSaisies, "toutes") == 0 || strcmp(classesSaisies, "Toutes") == 0)
            {
                /* Toutes les classes du departement */
                int nbClasses = getClassesParDepartement(deptSaisie, entrees, 20);
                for (int i = 0; i < nbClasses; i++)
                {
                    destinations[nbDestinations].type = TYPE_CLASSE;
                    strcpy(destinations[nbDestinations].nom, entrees[i]);
                    nbDestinations++;
                }
            }
            else
            {
                int nbEntrees = parserEntrees(classesSaisies, entrees, 20);
                int classesValides = 0;
                
                for (int i = 0; i < nbEntrees; i++)
                {
                    if (verifierClasse(entrees[i]))
                    {
                        destinations[nbDestinations].type = TYPE_CLASSE;
                        strcpy(destinations[nbDestinations].nom, entrees[i]);
                        nbDestinations++;
                        classesValides++;
                    }
                    else
                    {
                        printf("Classe '%s' introuvable.\n", entrees[i]);
                    }
                }
                
                if (classesValides == 0)
                {
                    printf("Aucune classe valide specifiee.\n");
                    nbDestinations = 0;
                }
            }
        }
        break;

    default:
        printf("Vous n'etes pas autorise a envoyer des messages.\n");
        return;
    }

    /* Envoyer le message */
    if (nbDestinations > 0)
    {
        int result = envoyerMessage(utilisateur, destinations, nbDestinations, message);
        if (result > 0)
        {
            printf("\n=== MESSAGE ENVOYE AVEC SUCCES ===\n");
        }
        else
        {
            printf("\n=== ECHEC DE L'ENVOI DU MESSAGE ===\n");
        }
    }
    else
    {
        printf("Aucun destinataire selectionne.\n");
    }
}

/**
 * Boucle principale de l'application
 */
void bouclePrincipale(Utilisateur *utilisateur)
{
    if (utilisateur == NULL)
        return;

    int quitter = 0;
    StatutUtilisateur statut = getStatutFromString(utilisateur->statut);

    while (!quitter)
    {
        afficherMenu(statut);
        int choix = saisirEntier("Choix: ", 0, 3);

        switch (choix)
        {
        case 0:
            quitter = 1;
            afficherMessageDeconnexion();
            break;
        case 1:
            gererEnvoiMessage(utilisateur);
            break;
        case 2:
            lireMessages(utilisateur);
            break;
        case 3:
            afficherProfil(utilisateur);
            break;
        default:
            printf("Choix invalide.\n");
        }
    }
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    /* Afficher l'OS detecte */
    printf("Systeme detecte: %s\n", getNomOS());

    /* Initialiser les fichiers et charger les donnees */
    initialiserFichiers();
    chargerDonnees();

    /* Declarations des variables */
    Utilisateur *tableauUtilisateurs[MAX_UTILISATEURS] = {NULL};
    int nbUtilisateurs = 0;

    /* Buffers pour la saisie utilisateur */
    char nomSaisi[TAILLE_MAX_SAISIE];
    char mdpSaisi[TAILLE_MAX_SAISIE];

    /* Variables pour la boucle de connexion */
    int tentative = 0;
    int connecte = 0;

    /* Variable pour stocker le resultat de la verification */
    ResultatVerification resultat;

    /* Affichage de la banniere du programme */
    afficherBanniereAccueil();

    /* Chargement des donnees depuis le fichier CSV */
    printf("Chargement de la base de donnees depuis %s...\n", FICHIER_CSV);
    nbUtilisateurs = lireCSV(FICHIER_CSV, tableauUtilisateurs);

    /* Si le chargement a echoue */
    if (nbUtilisateurs <= 0)
    {
        printf("Aucun utilisateur trouve dans le fichier CSV.\n");
        printf("Creation d'un utilisateur de test...\n");
        nbUtilisateurs = creerUtilisateurTest(tableauUtilisateurs);

        if (nbUtilisateurs <= 0)
        {
            printf("ERREUR FATALE : Impossible de charger des utilisateurs.\n");
            return 1;
        }
    }
    else
    {
        printf("%d utilisateurs charges avec succes depuis le CSV.\n", nbUtilisateurs);
    }

    printf("\nAppuyez sur Entree pour commencer l'authentification...");
    getchar();

    /* Boucle principale d'authentification */
    while (!connecte && tentative < MAX_TENTATIVES)
    {
        afficherBanniereConnexion();
        printf("Connexion - Tentative %d/%d\n", tentative + 1, MAX_TENTATIVES);

        /* Saisie des identifiants */
        saisirChaine("Nom: ", nomSaisi, sizeof(nomSaisi));
        saisirChaine("Mot de passe: ", mdpSaisi, sizeof(mdpSaisi));

        /* Verification des identifiants */
        resultat = verifierIdentifiants(tableauUtilisateurs, nbUtilisateurs, nomSaisi, mdpSaisi);

        /* Affichage du resultat */
        printf("\n%s\n", resultat.message);

        if (resultat.trouve)
        {
            connecte = 1;
            afficherProfil(resultat.utilisateur);
            printf("\nCONNEXION ETABLIE.\n");
            printf("Bienvenue %s %s !\n",
                   resultat.utilisateur->prenoms,
                   resultat.utilisateur->nom);

            /* Demarrer la boucle principale */
            bouclePrincipale(resultat.utilisateur);
        }
        else
        {
            tentative++;
            if (tentative < MAX_TENTATIVES)
            {
                printf("Il vous reste %d tentative(s).\n", MAX_TENTATIVES - tentative);
            }
        }
    }

    /* Gestion de l'echec apres trop de tentatives */
    if (!connecte)
    {
        printf("\n========================================\n");
        printf("COMPTE VERROUILLE POUR SECURITE\n");
        printf("========================================\n");
        printf("Nombre maximum de tentatives atteint (%d).\n", MAX_TENTATIVES);
        printf("Veuillez reessayer plus tard ou contacter l'administrateur.\n");
    }

    /* Liberation de la memoire */
    printf("\nNettoyage de la memoire en cours...\n");
    libererTableauUtilisateurs((void **)tableauUtilisateurs, nbUtilisateurs);
    printf("Memoire liberee.\n");
    printf("Programme termine avec succes.\n");

    printf("\nAppuyez sur Entree pour quitter...");
    getchar();

    return 0;
}
