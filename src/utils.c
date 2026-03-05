/**
 * @file utils.c
 * @brief Implementations des fonctions utilitaires
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "utils.h"
#include "os_compat.h"

void viderBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void saisirChaine(const char* message, char* buffer, int taille) {
    printf("%s", message);
    if (fgets(buffer, taille, stdin) != NULL) {
        size_t longueur = strlen(buffer);
        if (longueur > 0 && buffer[longueur - 1] == '\n') {
            buffer[longueur - 1] = '\0';
        } else {
            viderBuffer();
        }
    }
}

int saisirEntier(const char* message, int min, int max) {
    int valeur;
    char buffer[50];
    
    while (1) {
        printf("%s", message);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &valeur) == 1) {
                if (valeur >= min && valeur <= max) {
                    return valeur;
                }
            }
        }
        printf("Veuillez entrer un entier entre %d et %d\n", min, max);
    }
}

int comparerChaines(const char* ch1, const char* ch2) {
    if (ch1 == NULL || ch2 == NULL) {
        return -1;
    }
    
    while (*ch1 && *ch2) {
        char c1 = (char)tolower((unsigned char)*ch1);
        char c2 = (char)tolower((unsigned char)*ch2);
        if (c1 != c2) {
            return c1 - c2;
        }
        ch1++;
        ch2++;
    }
    
    return tolower((unsigned char)*ch1) - tolower((unsigned char)*ch2);
}

void trim(char* ch) {
    if (ch == NULL) return;
    
    char* debut = ch;
    while (isspace((unsigned char)*debut)) {
        debut++;
    }
    
    if (debut != ch) {
        memmove(ch, debut, strlen(debut) + 1);
    }
    
    char* fin = ch + strlen(ch) - 1;
    while (fin > ch && isspace((unsigned char)*fin)) {
        *fin = '\0';
        fin--;
    }
}

int estVide(const char* ch) {
    if (ch == NULL || ch[0] == '\0') {
        return 1;
    }
    
    while (*ch) {
        if (!isspace((unsigned char)*ch)) {
            return 0;
        }
        ch++;
    }
    return 1;
}

char* lireLigneFichier(FILE* fichier) {
    if (fichier == NULL) return NULL;
    
    static char ligne[TAILLE_MAX_LIGNE];
    if (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        size_t len = strlen(ligne);
        if (len > 0 && ligne[len - 1] == '\n') {
            ligne[len - 1] = '\0';
        }
        return ligne;
    }
    return NULL;
}

int ecrireDebutFichier(const char* nomFichier, const char* message) {
    if (nomFichier == NULL || message == NULL) {
        return -1;
    }
    
    FILE* fichier = fopen(nomFichier, "r");
    char contenu[10000] = "";
    size_t tailleExistante = 0;
    
    if (fichier != NULL) {
        size_t lu = fread(contenu, 1, sizeof(contenu) - 1, fichier);
        contenu[lu] = '\0';
        tailleExistante = strlen(contenu);
        fclose(fichier);
        
        if (tailleExistante > 9000) {
            printf("AVERTISSEMENT: Fichier trop grand, troncature possible\n");
            contenu[9000] = '\0';
        }
    }
    
    fichier = fopen(nomFichier, "w");
    if (fichier == NULL) {
        return -1;
    }
    
    fprintf(fichier, "%s\n%s", message, contenu);
    fclose(fichier);
    return 0;
}

char** lireMessagesFichier(const char* nomFichier, int* nbMessages) {
    (void)nomFichier;
    (void)nbMessages;
    return NULL;
}

void afficherMessages(const char* nomFichier) {
    if (nomFichier == NULL) return;
    
    FILE* fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Aucun message trouve.\n");
        return;
    }
    
    char ligne[TAILLE_MAX_LIGNE];
    int nb = 0;
    
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        nb++;
        if (nb <= 5) {
            printf("%s", ligne);
        }
    }
    
    if (nb > 5) {
        printf("\n... et %d autres messages\n", nb - 5);
    }
    
    fclose(fichier);
}

int creerFichierSiInexistant(const char* nomFichier) {
    if (nomFichier == NULL) return -1;
    
    FILE* fichier = fopen(nomFichier, "a");
    if (fichier == NULL) {
        return -1;
    }
    fclose(fichier);
    return 0;
}
