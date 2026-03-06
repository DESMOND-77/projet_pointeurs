# TODO - Projet Système de Messagerie

## Phase 1: Structure du Projet et Bibliothèques

- [x] Créer la structure des dossiers (include/, src/, data/)
- [x] Créer les fichiers d'en-tête (.h)
- [x] Implémenter les fonctions de base (memoire, chaines, fichiers)
- [x] Ajouter la gestion des caractères spéciaux Cross-OS

## Phase 2: Fonctions Utilitaires (utils)

- [x] Fonctions de saisie sécurisée
- [x] Fonctions de manipulation de chaînes
- [x] Fonctions de gestion des fichiers

## Phase 3: Fonctions de Bannières (banners)

- [x] Bannière d'accueil
- [x] Bannière de connexion
- [x] Bannières de menu

## Phase 4: Fonctions d'Authentification

- [x] Lecture du fichier CSV
- [x] Vérification des identifiants
- [x] Gestion de la session

## Phase 5: Système de Messagerie

- [x] Fonctions de gestion des permissions
- [x] Fonctions d'envoi de messages
- [ ] Fonctions de réception/lecture des messages

## Phase 6: Programme Principal

- [x] Intégrer tous les modules
- [x] Implémenter le menu principal
- [x] Tester le système

---

## NOUVELLES TÂCHES: Chargement CSV et Modification des Menus

### Step 1: Create CSV data files

- [x] Create data/directions.csv with header: nom
- [x] Create data/departements.csv with header: nom;direction
- [x] Create data/classes.csv with header: nom;departement

### Step 2: Update types.h

- [x] Add global array declarations for directions, departements, classes
- [x] Add number counters for each array

### Step 3: Update csv.h

- [x] Add function declarations for loading CSV data

### Step 4: Implement CSV loading in csv.c

- [x] Implement chargerDirections()
- [x] Implement chargerDepartements()
- [x] Implement chargerClasses()
- [x] Add validation functions

### Step 5: Update main.c

- [x] Load CSV data at startup
- [x] Modify professor menu options and add validation
- [x] Modify chef departement menu options and add validation
- [x] Modify administratif menu options and add validation
- [x] Modify direction menu options and add validation

---
