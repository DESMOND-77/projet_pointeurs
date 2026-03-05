/**
 * @file os_compat.h
 * @brief Compatibilite cross-OS pour les caracteres speciaux
 */

#ifndef OS_COMPAT_H
#define OS_COMPAT_H

/* Detecte le système d'exploitation */
#if defined(_WIN32) || defined(_WIN64)
    #define OS_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
    #define OS_MAC
#elif defined(__linux__)
    #define OS_LINUX
#else
    #define OS_UNKNOWN
#endif

/* Gestion des caracteres speciaux selon l'OS */
#ifdef OS_WINDOWS
    #define NOUVELLE_LIGNE "\r\n"
    #define SEPARATEUR_REPERTOIRE "\\"
#elif defined(OS_MAC)
    #define NOUVELLE_LIGNE "\r"
    #define SEPARATEUR_REPERTOIRE "/"
#else
    #define NOUVELLE_LIGNE "\n"
    #define SEPARATEUR_REPERTOIRE "/"
#endif

/* Fonctions de compatibilite */
void convertirNouvelleLigne(char* ch);
void normaliserChemin(char* chemin);
int detecterOS(void);
const char* getNomOS(void);

#endif
