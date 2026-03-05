# Makefile pour le systeme de messagerie

CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -std=c99
LDFLAGS =

# Fichiers sources
SRCS = src/main.c src/memoire.c src/csv.c src/utils.c src/banners.c src/auth.c src/messagerie.c src/os_compat.c
OBJS = $(SRCS:.c=.o)
TARGET = messagerie

# Cible par defaut
all: $(TARGET)

# Compilation du programme principal
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compilation des fichiers objets
%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -f $(OBJS) $(TARGET)

# Execution
run: $(TARGET)
	./$(TARGET)

# Creation des repertoires et fichiers
setup:
	mkdir -p data
	touch data/direction.txt
	touch data/departement_informatique.txt
	touch data/departement_mathematiques.txt
	touch data/departement_physique.txt
	touch data/classe_gl.txt
	touch data/classe_geii.txt

# Aide
help:
	@echo "Cibles disponibles:"
	@echo "  all     - Compile le programme"
	@echo "  clean  - Supprime les fichiers objets"
	@echo "  run    - Execute le programme"
	@echo "  setup  - Cree les fichiers de donnees"
	@echo "  help   - Affiche cette aide"

.PHONY: all clean run setup help
