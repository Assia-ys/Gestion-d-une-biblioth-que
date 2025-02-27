# Options de compilation
CFLAGS = -g -Wno-unused-parameter
CC = gcc

# Liste des programmes à générer
PROGRAMS = main

.PHONY: all clean

# Compilation principale
all: $(PROGRAMS)
 
main: main.o entreeSortieLC.o biblioLc.o biblioH.o
	$(CC) -o $@ $^ -lm

entreeSortieLC.o: entreeSortieLC.c entreeSortieLC.h
	$(CC) $(CFLAGS) -c entreeSortieLC.c

biblioLC.o: biblioLc.c biblioLC.h
	$(CC) $(CFLAGS) -c biblioLc.c

biblioH.o: biblioH.c biblioH.h
	$(CC) $(CFLAGS) -c biblioH.c -lm

main.o: main.c biblioLC.h biblioH.h entreeSortieLC.h
	$(CC) $(CFLAGS) -c main.c

# Nettoyage des fichiers objets et exécutables
clean:
	rm -f *.o *~ $(PROGRAMS) *.swp
