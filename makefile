CC= gcc
FLAG=-c -Wall
SDL2_LIB = `pkg-config --cflags --libs sdl2 SDL2_image`


all: main


utils.o: utils.c
	$(CC) $(FLAG) utils.c

affichage_graphique.o: affichage_graphique.c
	$(CC) $(FLAG) affichage_graphique.c $(SDL2_LIB)

affichageconsole.o: affichageconsole.c
	$(CC) $(FLAG) affichageconsole.c

deplacement.o: deplacement.c
	$(CC) $(FLAG) deplacement.c

case.o: case.c
	gcc -c -Wall case.c

initialisation.o: initialisation.c
	$(CC) $(FLAG) initialisation.c

simulation.o: simulation.c
	$(CC) $(FLAG) simulation.c

main.o: main.c
	$(CC) $(FLAG) main.c $(SDL2_LIB)

main: main.o simulation.o initialisation.o case.o deplacement.o affichageconsole.o affichage_graphique.o utils.o
	$(CC) main.o simulation.o initialisation.o case.o deplacement.o affichageconsole.o affichage_graphique.o utils.o -o main $(SDL2_LIB)


clean:
	rm *o

mrproper:
	rm *o main
