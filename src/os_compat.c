/**
 * @file os_compat.c
 * @brief Implementations des fonctions de compatibilite OS
 */

#include <string.h>
#include <stdio.h>
#include "os_compat.h"

void convertirNouvelleLigne(char* ch) {
    if (ch == NULL) return;
    
    char* src = ch;
    char* dst = ch;
    
    while (*src) {
        if (*src == '\r') {
            src++;
            if (*src == '\n') {
                src++;
            }
            *dst++ = '\n';
        } else if (*src == '\n') {
            #ifdef OS_WINDOWS
                *dst++ = '\r';
            #endif
            *dst++ = *src++;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
}

void normaliserChemin(char* chemin) {
    if (chemin == NULL) return;
    
    #ifdef OS_WINDOWS
    for (int i = 0; chemin[i]; i++) {
        if (chemin[i] == '/') {
            chemin[i] = '\\';
        }
    }
    #else
    for (int i = 0; chemin[i]; i++) {
        if (chemin[i] == '\\') {
            chemin[i] = '/';
        }
    }
    #endif
}

int detecterOS(void) {
    #ifdef OS_WINDOWS
        return 1;
    #elif defined(OS_MAC)
        return 2;
    #elif defined(OS_LINUX)
        return 3;
    #else
        return 0;
    #endif
}

const char* getNomOS(void) {
    #ifdef OS_WINDOWS
        return "Windows";
    #elif defined(OS_MAC)
        return "macOS";
    #elif defined(OS_LINUX)
        return "Linux";
    #else
        return "Inconnu";
    #endif
}
