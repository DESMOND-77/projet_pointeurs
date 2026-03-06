# Système de Messagerie Interne - Fafa

## Table des Matières

1. [Introduction](#introduction)
2. [Aperçu du Projet](#aperçu-du-projet)
3. [Architecture du Système](#architecture-du-système)
4. [Fonctionnalités](#fonctionnalités)
5. [Structure des Fichiers](#structure-des-fichiers)
6. [Installation et Compilation](#installation-et-compilation)
7. [Guide d'Utilisation](#guide-dutilisation)
8. [Gestion des Données](#gestion-des-données)
9. [Aspects Techniques](#aspects-techniques)
10. [Limitations et Améliorations](#limitations-et-améliorations)

---

## Introduction

Ce projet représente un système de messagerie interne développé en langage C pour un environnement éducatif (établissement d'enseignement supérieur). Il a été conçu dans le cadre d'un projet académique visant à mettre en pratique les concepts de la programmation avancée, notamment la gestion de la mémoire, la manipulation de fichiers, et la programmation modulaire.

Le système permet aux utilisateurs de s'authentifier et d'envoyer des messages à différents groupes au sein de l'établissement, en fonction de leur statut hiérarchique. Cette approche basée sur les rôles garantit que chaque utilisateur dispose des droits appropriés pour communiquer avec les bonnes personnes.

---

## Aperçu du Projet

### Objectifs Pédagogiques

Ce projet a été développé pour démontrer plusieurs compétences essentielles en programmation C :

- **Programmation modulaire** : Séparation du code en modules distincts avec des fichiers d'en-têtes correspondants
- **Gestion dynamique de la mémoire** : Allocation et libération manuelle de mémoire
- **Manipulation de fichiers** : Lecture et écriture de fichiers CSV et texte
- **Systèmes de types** : Définition de types personnalisés et d'énumérations
- **Interface console** : Interactions utilisateur via l'entrée/sortie standard
- **Compatibilité multiplateforme** : Support de différents systèmes d'exploitation

### Contexte d'Utilisation

Le système est conçu pour un établissement d'enseignement supérieur composed de plusieurs directions, départements et classes. Il permet la communication interne entre les différents acteurs de l'institution :

- La **Direction** gère l'établissement dans sa globalité
- Les **Départements** (Informatique, Mathématiques, Physique) regroupent les filières d'enseignement
- Les **Classes** (GL, GEII, GIM, etc.) constituent les groupes d'étudiants

---

## Architecture du Système

### Modèle Hiérarchique

L'architecture du système repose sur une hiérarchie à trois niveaux :

```text
Direction
    |
    +-- Departement
            |
            +-- Classe
```

Cette organisation permet un ciblage précis des destinataires lors de l'envoi de messages. Un utilisateur peut envoyer des messages à une direction entière, à un département spécifique, ou à une classe particulière.

### Système de Rôles

Le système implémente cinq statuts utilisateur avec des droits d'envoi différents :

| Statut | Description | Droits d'envoi |
| -------- | ------------- | ---------------- |
| `Etudiant` | Apprenant inscrit | Sa propre classe uniquement |
| `Professeur` | Enseignant | Classes et départements spécifiques |
| `ChefDepartement` | Responsable de département | Direction, départements, classes |
| `Administratif` | Personnel administratif | Direction, départements, classes |
| `Direction` | Direction de l'établissement | Direction, départements, classes |

### Flux d'Exécution

```text
+-------------------------+
|    Démarrage           |
+-------------------------+
            |
            v
+-------------------------+
| Chargement des données |
| (CSV files)            |
+-------------------------+
            |
            v
+-------------------------+
| Authentification       |
| (Usename + Mot de passe)   |
+-------------------------+
            |
            v
+-------------------------+
| Menu principal         |
| (Selon le statut)      |
+-------------------------+
            |
      +-----+-----+
      |           |
      v           v
+----------+ +----------+
| Envoyer  | | Lire     |
| Message  | | Messages |
+----------+ +----------+
      |
      v
+----------+
| Quitter  |
+----------+
```

---

## Fonctionnalités

### 1. Authentification des Utilisateurs

Le système vérifie les identifiants saisis par l'utilisateur contre une base de données CSV. Chaque utilisateur possède :

- Un **username** (identifiant unique de connexion)
- Un **nom** (nom de famille)
- Un **prénom**
- Un **département** d'appartenance
- Une **classe** (pour les étudiants)
- Un **mot de passe**
- Un **statut** déterminant les droits

En cas d'échec répété (plus de 3 tentatives), le compte est verrouillé pour des raisons de sécurité.

### 2. Envoi de Messages

Les utilisateurs peuvent envoyer des messages texte à destination de :

- **Une classe** : Tous les étudiants d'une classe donnée
- **Un département** : Tous les étudiants et personnels d'un département
- **Une direction** : L'ensemble du personnel d'une direction

Le système vérifie les autorisations avant l'envoi et s'assure que les destinataires spécifiés existent dans la base de données.

### 3. Lecture des Messages

Chaque utilisateur peut consulter les messages qui lui ont été adressés. Les messages sont stockés dans des fichiers texte spécifiques à chaque destination.

### 4. Consultation du Profil

Les utilisateurs peuvent visualiser leur profil contenant leurs informations personnelles et leur statut dans l'établissement.

---

## Structure des Fichiers

```text
Fafa/
├── Makefile                    # Script de compilation
├── README.md                   # Ce document
├── TODO.md                     # Liste des tâches à effectuer
├── .gitignore                  # Fichiers ignorés par Git
│
├── data/                       # Répertoire des données
│   ├── utilisateurs.csv        # Base des utilisateurs
│   ├── classes.csv             # Liste des classes
│   ├── departements.csv        # Liste des départements
│   └── directions.csv          # Liste des directions
│
├── include/                    # Fichiers d'en-têtes
│   ├── types.h                 # Définitions de types
│   ├── auth.h                  # Fonctions d'authentification
│   ├── csv.h                   # Fonctions CSV
│   ├── memoire.h               # Gestion de la mémoire
│   ├── messagerie.h            # Fonctions de messagerie
│   ├── utils.h                 # Utilitaires
│   ├── banners.h               # Bannières d'affichage
│   ├── os_compat.h             # Compatibilité OS
│
└── src/                        # Fichiers sources
    ├── main.c                  # Programme principal
    ├── auth.c                   # Implémentation auth
    ├── csv.c                    # Implémentation CSV
    ├── memoire.c               # Implémentation mémoire
    ├── messagerie.c             # Implémentation messagerie
    ├── utils.c                 # Implémentation utilitaires
    ├── banners.c               # Implémentation bannières
    └── os_compat.c             # Implémentation compatibilité
```

### Description des Modules

#### Fichiers d'En-tête (`include/`)

- **types.h** : Définit les structures de données (Utilisateur, Message, Destination) et les types énumérés (StatutUtilisateur, TypeDestination)

- **auth.h** : Déclare les fonctions d'authentification et de vérification des identifiants

- **csv.h** : Interface pour la lecture et l'écriture des fichiers CSV

- **memoire.h** : Fonctions d'allocation et de libération de mémoire

- **messagerie.h** : Fonctions d'envoi et de lecture des messages

- **utils.h** : Fonctions utilitaires générales (saisie utilisateur, validation)

- **banners.h** : Déclarations pour les fonctions d'affichage graphique

- **os_compat.h** : Abstraction des fonctionnalités spécifiques au système d'exploitation

#### Fichiers Sources (`src/`)

- **main.c** : Point d'entrée du programme, boucle principale, gestion du menu

- **auth.c** : Vérification des identifiants, gestion du profil utilisateur

- **csv.c** : Chargement des données depuis les fichiers CSV, validation des entités

- **memoire.c** : Gestion dynamique de la mémoire (allocation, libération)

- **messagerie.c** : Logique d'envoi et de réception des messages

- **utils.c** : Fonctions d'aide à la saisie et au traitement des chaînes

- **banners.c** : Affichage des bannières d'accueil et de menu

- **os_compat.c** : Implémentation des fonctions portables entre Windows et Linux

---

## Installation et Compilation

### Prérequis

- Compilateur C (GCC recommandé)
- Système d'exploitation : Linux ou Windows
- Make (pour utiliser le Makefile)

### Compilation

Pour compiler le projet, exécutez simplement la commande suivante à la racine du projet :

```bash
make
```

Cette commande génère l'exécutable `messagerie` à partir des fichiers sources.

### Exécution

Après une compilation réussie, lancez le programme avec :

```bash
make run
```

Ou directement :

```bash
./messagerie
```

### Nettoyage

Pour supprimer les fichiers objets et l'exécutable :

```bash
make clean
```

### Aide

Pour afficher les cibles disponibles :

```bash
make help
```

---

## Guide d'Utilisation

### Démarrage

1. Lancez le programme
2. Le système affiche une bannière de bienvenue
3. Les données sont chargées depuis les fichiers CSV
4. Appuyez sur Entrée pour accéder à l'authentification

### Authentification

1. Entrez votre **username** (nom d'utilisateur)
2. Entrez votre **mot de passe**
3. Le système vérifie les identifiants

**Comptes de test disponibles :**

| Username | Nom | Prénom | Mot de passe | Statut | Département | Classe |
| ---------- | ----- | -------- | -------------- | -------- | ------------- | -------- |
| jdupont | Dupont | Jean | secret123 | Etudiant | Informatique | GL |
| smartin | Martin | Sophie | prof2024 | Professeur | Informatique | - |
| pbernard | Bernard | Pierre | chef2024 | ChefDepartement | Informatique | GL |
| mdurand | Durand | Marie | admin2024 | Administratif | Mathematiques | - |
| llefebvre | Lefebvre | Luc | dir2024 | Direction | Direction | - |

### Menu Principal

Après connexion, le menu affiché dépend de votre statut :

#### Menu Étudiant

```text
========================================
           MENU ETUDIANT
========================================
1. Envoyer un message (ma classe)
2. Lire mes messages
3. Voir mon profil
0. Quitter
```

#### Menu Professeur

```text
========================================
         MENU PROFESSEUR
========================================
1. Envoyer un message
2. Lire mes messages
3. Voir mon profil
0. Quitter
```

#### Menu Direction / Administratif / Chef Département

```text
========================================
            MENU
========================================
1. Envoyer un message
2. Lire mes messages
3. Voir mon profil
0. Quitter
```

### Envoi d'un Message

1. Sélectionnez "Envoyer un message"
2. Saisissez le contenu de votre message
3. Sélectionnez le type de destinataire (direction/département/classe)
4. Spécifiez le nom du destinataire
5. Validez l'envoi

**Formats acceptés pour les destinataires :**

- **Classes** : `gl_1`, `gl_2`, `geii_1`, `info_1`, etc.
- **Départements** : `informatique`, `mathematiques`, `physique`
- **Directions** : `generale`, `des_etudes`, `de_la_recherche`

Vous pouvez spécifier plusieurs destinataires en les séparant par des virgules.

---

## Gestion des Données

### Fichiers CSV

Le système utilise des fichiers au format CSV (Comma-Separated Values) avec un point-virgule comme séparateur :

#### utilisateurs.csv

```text
Username;Nom;Prenoms;Departement;Classe;MotDePasse;Statut
jdupont;Dupont;Jean;Informatique;GL;secret123;Etudiant
```

#### classes.csv

```text
nom;departement
gl_1;dept_gl
gl_2;dept_gl
```

#### departements.csv

```text
nom;direction
informatique;direction_generale
```

#### directions.csv

```text
nom
generale
```

### Stockage des Messages

Les messages sont écrits dans des fichiers texte par destination :

- `data/direction_[nom].txt`
- `data/departement_[nom].txt`
- `data/classe_[nom].txt`

Format d'un message :

```python
[2024-01-15 14:30:25] Expediteur: Jean Dupont
Message: Bonjour à tous les étudiants de GL
```

---

## Aspects Techniques

### Gestion de la Mémoire

Le système utilise l'allocation dynamique de mémoire pour stocker les informations des utilisateurs. Chaque utilisateur estalloué individuellement avec `malloc()` et libéré avec `free()` à la fin du programme.

```c
Utilisateur* nouvelUtilisateur = (Utilisateur*)malloc(sizeof(Utilisateur));
```

Les chaînes de caractères sont gérées via des fonctions personnalisées de duplication et de libération.

### Parsing CSV

Le parser CSV implémente les fonctionnalités suivantes :

- Lecture ligne par ligne avec `fgets()`
- Tokenisation avec `strtok()`
- Suppression des espaces (trim)
- Validation du nombre de champs
- Gestion des erreurs

### Compatibilité Multiplateforme

Le module `os_compat.c` fournit une abstraction pour les fonctionnalités dépendantes du système :

- Création de répertoires
- Détection du système d'exploitation
- Chemins de fichiers

### Assertions et Validation

Le code inclut de nombreuses vérifications :

- Vérification des pointeurs NULL
- Validation des limites (bounds checking)
- Gestion des erreurs de fichier
- Messages d'erreur informatifs

---

## Limitations et Améliorations

### Limitations Actuelles

1. **Interface texte uniquement** : Pas d'interface graphique utilisateur (GUI)
2. **Pas de chiffrement** : Les mots de passe sont stockés en clair
3. **Messages textes simples** : Pas de formatage avancé ou de pièces jointes
4. **Base de données statique** : Pas de création/modification d'utilisateurs depuis l'interface
5. **Pas de notification** : Les utilisateurs doivent consulter activement leurs messages
6. **Pas de persistance des sessions** : Chaque exécution nécessite une nouvelle authentification

### Améliorations Possibles

1. **Interface graphique** : Utilisation de ncurses ou GTK pour une meilleure expérience
2. **Chiffrement** : Hachage des mots de passe (SHA-256, bcrypt)
3. **Base de données** : Migration vers SQLite pour une meilleure gestion des données
4. **Journalisation** : Ajout de logs pour le débogage et la traçabilité
5. **Notifications** : Système d'alertes pour les nouveaux messages
6. **Gestion des utilisateurs** : Interface d'administration pour CRUD utilisateur
7. **Messages enrichis** : Support du format Markdown ou HTML
8. **Communication réseau** : Version client-serveur avec sockets

---

## Conclusion

Ce projet constitue une application complète de système de messagerie interne en C. Il démontre les compétences fondamentales en programmation système et constitue une base solide pour des développements plus avancés.

L'architecture modulaire facilite la maintenance et les extensions futures. Le code est documenté et suit les bonnes pratiques de programmation C.

Pour toute question ou suggestion d'amélioration, n'hésitez pas à contribuer au projet.

---

*Document généré dans le cadre du projet de Programmation Avancée*
*USTM/EPM - DUT2 GRT S1*
