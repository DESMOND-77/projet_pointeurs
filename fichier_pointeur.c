#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>

#define TAILLE_MAX_LIGNE 2000
#define MAX_UTILISATEURS 1000
#define MAX_CHAMPS 6
#define TAILLE_MAX_SAISIE 1000


typedef struct{
    char nom[20];
    char departements[20];
}classes;

typedef struct{
    char nom[20];
    char direction[10];
}departements;

typedef struct{
    char nom[10];
}direction;


typedef struct {
    char* nom;          // Pointeur vers le nom de l'utilisateur
    char* prenoms;      // Pointeur vers les prénoms de l'utilisateur
    char* departement; // Pointeur vers le département de l'utilisateur
    char* classe;       // Pointeur vers la classe de l'utilisateur (si etudiant)
    char* motDePasse;   // Pointeur vers le mot de passe
    char* statut;       // Pointeur vers le statut (étudiant, professeur, etc.)
} Utilisateur;

/**
 * Structure pour stocker le résultat de la vérification
 */
typedef struct {
    int trouve;                     // 1 si trouvé, 0 sinon
    Utilisateur* utilisateur;       // Pointeur vers l'utilisateur trouvé
    char message[TAILLE_MAX_SAISIE]; // Message d'information
} ResultatVerification;

// ============================================================================
// FONCTIONS DE GESTION DE LA MÉMOIRE
// ============================================================================

/**
 * Duplique une chaîne de caractères avec allocation dynamique
 * @param source La chaîne source à dupliquer
 * @return Pointeur vers la nouvelle chaîne ou NULL en cas d'erreur
 */
char* dupliquerChaine(const char* source) {
    // Vérification que la source n'est pas NULL
    if (source == NULL) {
        return NULL;
    }
    
    // Allocation de mémoire pour la nouvelle chaîne
    // strlen(source) donne la longueur sans le caractère nul
    // On ajoute 1 pour le caractère nul de fin '\0'
    char* nouvelleChaine = (char*)malloc((strlen(source) + 1) * sizeof(char));
    
    // Vérification que l'allocation a réussi
    if (nouvelleChaine == NULL) {
        printf("ERREUR : Impossible d'allouer la mémoire\n");
        return NULL;
    }
    
    // Copie de la chaîne source vers la nouvelle mémoire
    strcpy(nouvelleChaine, source);
    
    return nouvelleChaine;
}

/**
 * Libère la mémoire allouée pour un utilisateur
 * @param utilisateur Pointeur vers l'utilisateur à libérer
 */
void libererUtilisateur(Utilisateur* utilisateur) {
    // Vérification que l'utilisateur n'est pas NULL
    if (utilisateur == NULL) {
        return;  // Rien à libérer
    }
    
    // Libération de chaque champ alloué dynamiquement
    // free() ne fait rien si le pointeur est NULL
    free(utilisateur->nom);
    free(utilisateur->prenoms);
    free(utilisateur->departement);
    free(utilisateur->classe);
    free(utilisateur->motDePasse);
    free(utilisateur->statut);
    
    // Libération de la structure elle-même
    free(utilisateur);
}

// ============================================================================
// FONCTION DE LECTURE DU FICHIER CSV
// ============================================================================

/**
 * Lit un fichier CSV et extrait les identifiants dans un tableau d'utilisateurs
 * @param nomFichier Chemin vers le fichier CSV
 * @param utilisateurs Tableau de pointeurs pour stocker les utilisateurs
 * @return Nombre d'utilisateurs lus, ou -1 en cas d'erreur
 * 
 * Le fichier CSV doit avoir le format suivant :
 * Nom,Prénoms,MotDePasse,Statut
 * Dupont,Jean,secret123,Étudiant
 * Martin,Sophie,prof2024,Professeur
 */
int lireCSV(const char* nomFichier, Utilisateur* utilisateurs[]) {
    // Vérification des paramètres d'entrée
    if (nomFichier == NULL || utilisateurs == NULL) {
        printf("ERREUR : Paramètres invalides pour la lecture du CSV\n");
        return -1;
    }
    
    printf("Ouverture du fichier CSV : %s\n", nomFichier);
    
    // 1. OUVERTURE DU FICHIER CSV en mode lecture ("r")
    FILE* fichier = fopen(nomFichier, "r");
    
    // Vérification que l'ouverture a réussi
    if (fichier == NULL) {
        printf("ERREUR : Impossible d'ouvrir le fichier CSV %s\n", nomFichier);
        printf("Vérifiez que le fichier existe et est accessible en lecture.\n");
        return -1;
    }
    
    // printf("Fichier ouvert avec succès. Lecture en cours...\n");
    
    // 2. LECTURE DE LA PREMIÈRE LIGNE (EN-TÊTE)
    // On lit et on ignore la ligne d'en-tête qui contient les noms des colonnes
    char ligne[TAILLE_MAX_LIGNE];
    if (fgets(ligne, sizeof(ligne), fichier) == NULL) {
        printf("ERREUR : Fichier CSV vide\n");
        fclose(fichier);
        return -1;
    }
    
    // printf("En-tête détecté : %s", ligne);
    
    // 3. LECTURE DES LIGNES DE DONNÉES
    int nbUtilisateursLus = 0;
    int numeroLigne = 1;  // On commence à la ligne 1 (après l'en-tête)
    
    // Boucle de lecture ligne par ligne jusqu'à la fin du fichier
    while (fgets(ligne, sizeof(ligne), fichier) != NULL && 
           nbUtilisateursLus < MAX_UTILISATEURS) {
        
        numeroLigne++;
        
        // Suppression du caractère de nouvelle ligne '\n' à la fin de la ligne
        size_t longueur = strlen(ligne);
        if (longueur > 0 && ligne[longueur - 1] == '\n') {
            ligne[longueur - 1] = '\0';  // Remplacement par le caractère nul
        }
        
        // Ignorer les lignes vides
        if (longueur <= 1) {
            continue;
        }
        
        // printf("Traitement ligne %d : %s\n", numeroLigne, ligne);
        
        // 4. DÉCOUPAGE DE LA LIGNE CSV EN CHAMPS (TOKENISATION)
        // strtok découpe une chaîne en utilisant un séparateur
        // Premier appel : on passe la chaîne à découper
        char* champs[MAX_CHAMPS];
        int nbChamps = 0;
        
        // strtok modifie la chaîne originale ! On fait une copie pour ne pas perdre les données
        char ligneCopie[TAILLE_MAX_LIGNE];
        strcpy(ligneCopie, ligne);
        
        // Obtention du premier champ (jusqu'à la première virgule)
        char* token = strtok(ligneCopie, ";");
        
        // Boucle pour extraire tous les champs
        while (token != NULL && nbChamps < MAX_CHAMPS) {
            // Suppression des espaces au début et à la fin (trim)
            while (isspace((unsigned char)*token)) token++;
            char* fin = token + strlen(token) - 1;
            while (fin > token && isspace((unsigned char)*fin)) fin--;
            fin[1] = '\0';
            
            // Stockage du champ dans le tableau
            champs[nbChamps] = token;
            nbChamps++;
            
            // Passage au champ suivant
            token = strtok(NULL, ";");
        }
        
        // Vérification qu'on a au moins 4 champs (nom, prénoms, mdp, statut)
        if (nbChamps < MAX_CHAMPS) {
            printf("AVERTISSEMENT : Ligne %d ignorée (pas assez de champs : %d)\n", 
                   numeroLigne, nbChamps);
            continue;
        }
        
        // 5. ALLOCATION D'UNE NOUVELLE STRUCTURE UTILISATEUR
        Utilisateur* nouvelUtilisateur = (Utilisateur*)malloc(sizeof(Utilisateur));
        
        // Vérification que l'allocation a réussi
        if (nouvelUtilisateur == NULL) {
            printf("ERREUR : Allocation mémoire pour utilisateur (ligne %d)\n", numeroLigne);
            break;  // Sortie de la boucle si problème mémoire
        }
        
        // Initialisation des pointeurs à NULL pour éviter les erreurs
        nouvelUtilisateur->nom = NULL;
        nouvelUtilisateur->prenoms = NULL;
        nouvelUtilisateur->departement = NULL;
        nouvelUtilisateur->classe = NULL;
        nouvelUtilisateur->motDePasse = NULL;
        nouvelUtilisateur->statut = NULL;
        
        // 6. COPIE DES CHAMPS DANS LA STRUCTURE AVEC ALLOCATION DYNAMIQUE
        // Champ 0 : NOM
        if (champs[0] != NULL && strlen(champs[0]) > 0) {
            nouvelUtilisateur->nom = dupliquerChaine(champs[0]);
        }
        
        // Champ 1 : PRÉNOMS
        if (champs[1] != NULL && strlen(champs[1]) > 0) {
            nouvelUtilisateur->prenoms = dupliquerChaine(champs[1]);
        }
        // Champ 2 : DEPARTEMENT
        if (champs[2] != NULL && strlen(champs[2]) > 0) {
            nouvelUtilisateur->departement = dupliquerChaine(champs[2]);
        }
        
        // Champ 3 : CLASSE
        if (champs[3] != NULL && strlen(champs[3]) > 0) {
            nouvelUtilisateur->classe = dupliquerChaine(champs[3]);
        }
        // Champ 4 : MOT DE PASSE
        if (champs[4] != NULL && strlen(champs[4]) > 0) {
            nouvelUtilisateur->motDePasse = dupliquerChaine(champs[4]);
        }
        
        // Champ 5 : STATUT
        if (champs[5] != NULL && strlen(champs[5]) > 0) {
            nouvelUtilisateur->statut = dupliquerChaine(champs[5]);
        }
        
        // Vérification que l'utilisateur a au moins un nom (champ obligatoire)
        if (nouvelUtilisateur->nom != NULL && strlen(nouvelUtilisateur->nom) > 0) {
            // Ajout au tableau des utilisateurs
            utilisateurs[nbUtilisateursLus] = nouvelUtilisateur;
            nbUtilisateursLus++;
            
            // printf("  → Utilisateur ajouté : %s %s (%s)\n", 
            //        nouvelUtilisateur->prenoms ? nouvelUtilisateur->prenoms : "",
            //        nouvelUtilisateur->nom ? nouvelUtilisateur->nom : "",
            //        nouvelUtilisateur->statut ? nouvelUtilisateur->statut : "statut inconnu");
        } else {
            // Libération de la mémoire si l'utilisateur est invalide (pas de nom)
            printf("  → Ligne ignorée : nom manquant\n");
            libererUtilisateur(nouvelUtilisateur);
        }
    }
    
    // 7. FERMETURE DU FICHIER
    fclose(fichier);
    
    printf("Lecture CSV terminée. %d utilisateurs chargés.\n", nbUtilisateursLus);
    return nbUtilisateursLus;
}

// ============================================================================
// FONCTIONS DE SAISIE UTILISATEUR
// ============================================================================

/**
 * Vide le buffer d'entrée pour éviter les problèmes de lectures suivantes
 */
void viderBuffer() {
    int c;
    // Lit tous les caractères jusqu'à trouver un retour à la ligne ou la fin du fichier
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * Saisie sécurisée d'une chaîne de caractères
 * @param message Le message à afficher pour inviter l'utilisateur
 * @param buffer Le buffer pour stocker la saisie
 * @param taille La taille maximale du buffer
 */
void saisirChaine(const char* message, char* buffer, int taille) {
    // Affichage du message d'invite
    printf("%s", message);
    
    // Lecture sécurisée avec fgets (limite la taille pour éviter les débordements)
    if (fgets(buffer, taille, stdin) != NULL) {
        // Suppression du caractère de nouvelle ligne '\n' ajouté par fgets
        size_t longueur = strlen(buffer);
        if (longueur > 0 && buffer[longueur - 1] == '\n') {
            buffer[longueur - 1] = '\0';  // Remplacement par le caractère nul
        } else {
            // Si le buffer était trop petit, il reste des caractères à vider
            viderBuffer();
        }
    }
}

// ============================================================================
// FONCTIONS DE VÉRIFICATION DES IDENTIFIANTS
// ============================================================================

/**
 * Vérifie si les identifiants saisis correspondent à un utilisateur du tableau
 * @param utilisateurs Tableau des utilisateurs (pointeurs)
 * @param nbUtilisateurs Nombre d'utilisateurs dans le tableau
 * @param nomSaisi Nom saisi par l'utilisateur
 * @param prenomsSaisis Prénoms saisis
 * @param mdpSaisi Mot de passe saisi
 * @return Résultat de la vérification (structure ResultatVerification)
 */
ResultatVerification verifierIdentifiants(Utilisateur* utilisateurs[], 
                                          int nbUtilisateurs,
                                          const char* nomSaisi,
                                       
                                          const char* mdpSaisi) {
    
    // Initialisation de la structure de résultat
    ResultatVerification resultat;
    resultat.trouve = 0;
    resultat.utilisateur = NULL;
    resultat.message[0] = '\0';  // Chaîne vide
    
    // Vérification que tous les champs sont remplis
    if (nomSaisi == NULL || nomSaisi[0] == '\0') {
        strcpy(resultat.message, " Le nom est obligatoire.");
        return resultat;
    }
    
    // if (prenomsSaisis == NULL || prenomsSaisis[0] == '\0') {
    //     strcpy(resultat.message, " Les prénoms sont obligatoires.");
    //     return resultat;
    // }
    
    if (mdpSaisi == NULL || mdpSaisi[0] == '\0') {
        strcpy(resultat.message, " Le mot de passe est obligatoire.");
        return resultat;
    }
    
    // Parcours du tableau des utilisateurs pour trouver une correspondance
    for (int i = 0; i < nbUtilisateurs; i++) {
        Utilisateur* u = utilisateurs[i];
        
        // Vérification que l'utilisateur n'est pas NULL
        if (u == NULL) continue;
        
        // Comparaison du nom (strcmp compare deux chaînes caractère par caractère)
        if (u->nom != NULL && strcmp(u->nom, nomSaisi) == 0) {
            
            // Comparaison des prénoms
            // if (u->prenoms != NULL && strcmp(u->prenoms, prenomsSaisis) == 0) {
                printf("%s vs %s\t%s vs %s",u->nom,nomSaisi,u->motDePasse,mdpSaisi);
                // Comparaison du mot de passe
                if (u->motDePasse != NULL && strcmp(u->motDePasse, mdpSaisi) == 0) {
                    
                    // TOUS LES CRITÈRES SONT REMPLIS
                    resultat.trouve = 1;
                    resultat.utilisateur = u;
                    strcpy(resultat.message, " Connexion réussie !");
                    return resultat;
                }
            // }
        }
    }
    
    // Aucune correspondance trouvée
    strcpy(resultat.message, " Identifiants incorrects. Veuillez réessayer.");
    return resultat;
}

// ============================================================================
// FONCTION D'AFFICHAGE DU PROFIL
// ============================================================================

/**
 * Affiche les informations d'un utilisateur
 * @param user Pointeur vers l'utilisateur à afficher
 */
void afficherProfil(Utilisateur* user) {
    // Vérification que l'utilisateur n'est pas NULL
    if (user == NULL) return;
    
    printf("\n");
    printf("========================================\n");
    printf("         PROFIL UTILISATEUR             \n");
    printf("========================================\n");
    printf("Nom complet : %s %s\n", 
           user->prenoms ? user->prenoms : "", 
           user->nom ? user->nom : "");
    printf("Statut      : %s\n", user->statut ? user->statut : "Non spécifié");
    printf("========================================\n");
}

// ============================================================================
// FONCTION DE CRÉATION D'UTILISATEUR DE TEST
// ============================================================================

/**
 * Crée un utilisateur de test en cas d'absence de fichier CSV
 * @param utilisateurs Tableau pour stocker l'utilisateur
 * @return Nombre d'utilisateurs créés (1 ou 0)
 */
int creerUtilisateurTest(Utilisateur* utilisateurs[]) {
    printf("\nCréation d'un utilisateur de test pour le débogage...\n");
    
    // Allocation d'une nouvelle structure Utilisateur
    Utilisateur* test = (Utilisateur*)malloc(sizeof(Utilisateur));
    if (test == NULL) {
        printf("ERREUR : Impossible de créer l'utilisateur de test\n");
        return 0;
    }
    
    // Initialisation des champs avec des valeurs de test
    test->nom = dupliquerChaine("Admin");
    test->prenoms = dupliquerChaine("System");
    test->motDePasse = dupliquerChaine("admin123");
    test->statut = dupliquerChaine("Administrateur");
    
    // Ajout au tableau
    utilisateurs[0] = test;
    
    printf("Utilisateur de test créé : Admin System / admin123\n");
    return 1;
}

// ============================================================================
// FONCTION PRINCIPALE
// ============================================================================

int main() {
    // Déclaration des variables
    Utilisateur* tableauUtilisateurs[MAX_UTILISATEURS] = {NULL};
    int nbUtilisateurs = 0;
    
    // Nom du fichier CSV à lire (modifiable selon vos besoins)
    char nomFichier[TAILLE_MAX_SAISIE] = "users.csv";
    
    // Buffers pour la saisie utilisateur
    char nomSaisi[TAILLE_MAX_SAISIE];
    char prenomsSaisis[TAILLE_MAX_SAISIE];
    char mdpSaisi[TAILLE_MAX_SAISIE];
    
    // Variables pour la boucle de connexion
    int tentative = 0;
    int maxTentatives = 3;
    int connecte = 0;
    
    // Variable pour stocker le résultat de la vérification
    ResultatVerification resultat;
    
    // Affichage de la bannière du programme
    printf("\n");
    printf("========================================\n");
    printf("  SYSTÈME D'AUTHENTIFICATION CSV        \n");
    printf("========================================\n");
    printf("  Lecture des identifiants depuis       \n");
    printf("  un fichier CSV                         \n");
    printf("========================================\n");
    printf("\n");
    
    // 1. CHARGEMENT DES DONNÉES DEPUIS LE FICHIER CSV
    printf("Chargement de la base de données depuis %s...\n", nomFichier);
    
    // Appel de la fonction de lecture CSV (renommée de lireExcel à lireCSV)
    nbUtilisateurs = lireCSV(nomFichier, tableauUtilisateurs);
    
    // Si le chargement a échoué ou si aucun utilisateur n'a été trouvé
    if (nbUtilisateurs <= 0) {
        printf(" Aucun utilisateur trouvé dans le fichier CSV.\n");
        printf("Vérifiez que le fichier '%s' existe et contient des données au format :\n", nomFichier);
        printf("Nom,Prénoms,Departemet,classe,MotDePasse,Statut\n");
        printf("Dupont,Jean,secret123,Étudiant\n");
        printf("Martin,Sophie,prof2024,Professeur\n");
        
        // Création d'un utilisateur par défaut pour permettre les tests
        nbUtilisateurs = creerUtilisateurTest(tableauUtilisateurs);
        
        if (nbUtilisateurs <= 0) {
            printf(" ERREUR FATALE : Impossible de charger des utilisateurs.\n");
            printf("Programme terminé.\n");
            return 1;  // Sortie avec code d'erreur
        }
    } else {
        printf("%d utilisateurs chargés avec succès depuis le CSV.\n", nbUtilisateurs);
    }
    
    printf("\nAppuyez sur Entrée pour commencer l'authentification...");
    getchar();  // Attente de l'utilisateur
    
    // 2. BOUCLE PRINCIPALE D'AUTHENTIFICATION
    // On limite à maxTentatives essais pour des raisons de sécurité
    while (!connecte && tentative < maxTentatives) {
        printf("\n");
        printf("========================================\n");
        printf("   CONNEXION - Tentative %d/%d\n", tentative + 1, maxTentatives);
        printf("========================================\n");
        
        // Saisie des identifiants
        saisirChaine("Nom : ", nomSaisi, sizeof(nomSaisi));
        // saisirChaine("Prénom(s) : ", prenomsSaisis, sizeof(prenomsSaisis));
        saisirChaine("Mot de passe : ", mdpSaisi, sizeof(mdpSaisi));
        
        // Vérification des identifiants dans le tableau
        resultat = verifierIdentifiants(
            tableauUtilisateurs, 
            nbUtilisateurs, 
            nomSaisi, 
            // prenomsSaisis, 
            mdpSaisi
        );
        
        // Affichage du résultat
        printf("\n%s\n", resultat.message);
        
        if (resultat.trouve) {
            // Connexion réussie
            connecte = 1;
            
            // Affichage du profil de l'utilisateur connecté
            afficherProfil(resultat.utilisateur);
            
            // Message de connexion stabilisée
            printf("\nCONNEXION ÉTABLIE ET STABILISÉE.\n");
            printf("   Session utilisateur active.\n");
            printf("   Bienvenue %s %s !\n", 
                   resultat.utilisateur->prenoms,
                   resultat.utilisateur->nom);
            
            // Ici, on pourrait ajouter des fonctionnalités supplémentaires
            // accessibles uniquement après connexion
            printf("\nAppuyez sur Entrée pour vous déconnecter...");
            getchar();
            getchar();  // Deuxième getchar pour capturer l'Entrée
            printf("Déconnexion en cours...\n");
            
        } else {
            // Échec de la connexion
            tentative++;
            if (tentative < maxTentatives) {
                printf("Il vous reste %d tentative(s).\n", maxTentatives - tentative);
            }
        }
    }
    
    // 3. GESTION DE L'ÉCHEC APRÈS TROP DE TENTATIVES
    if (!connecte) {
        printf("\n");
        printf("========================================\n");
        printf("COMPTE VERROUILLÉ POUR SÉCURITÉ\n");
        printf("========================================\n");
        printf("Nombre maximum de tentatives atteint (%d).\n", maxTentatives);
        printf("Pour des raisons de sécurité, l'accès est temporairement bloqué.\n");
        printf("Veuillez réessayer plus tard ou contacter l'administrateur.\n");
    }
    
    // 4. LIBÉRATION DE LA MÉMOIRE
    // Il est crucial de libérer toute la mémoire allouée dynamiquement
    printf("\nNettoyage de la mémoire en cours...\n");
    
    int compteurLibere = 0;
    for (int i = 0; i < nbUtilisateurs; i++) {
        if (tableauUtilisateurs[i] != NULL) {
            libererUtilisateur(tableauUtilisateurs[i]);
            tableauUtilisateurs[i] = NULL;
            compteurLibere++;
        }
    } 
    
    printf("Mémoire libérée : %d utilisateurs supprimés.\n", compteurLibere);
    printf("Programme terminé avec succès.\n");
    
    // Pause pour Visual Studio (garde la console ouverte)
    printf("\nAppuyez sur Entrée pour quitter...");
    getchar();
    
    return 0;  // Retour 0 indique que le programme s'est terminé correctement
}