#ifndef UTILS_H
#define UTILS_H
//Constantes
//Taille grille
#define R 30 /*nombre de lignes*/
#define C 80 /*nombre de colonnes*/

//Taille fenêtre graphique
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

//Probabilités pour initialisation grille
#define PROBA_LAMBDA 20
#define PROBA_SOIGNANT 10
#define PROBA_VIRUS 50

//Probabilités pour déplacement
#define PROBA_DEPLACEMENT 70
#define PROBA_MAINTIEN_DIRECTION 75
#define PROBA_CHGT_DIR_OBSTACLE 50

//Probabilités pour infection
#define PROBA_MALADE_SI_INFECTE 33
#define PROBA_DISPERSION_VIRUS 25
#define VIRULENCE 10


//Couleur et effacement de l' console_display
//Couleur d'écriture
#define RED "\033[91m"
#define ORANGE "\033[38;5;202m"
#define GREEN "\033[92m"
#define MAGENTA "\033[35m"
#define YELLOW "\033[93m"
//Couleur de fond
#define YELLOW_BG "\033[43m"
#define BLUE_BG "\033[44m"
#define CYAN_BG "\033[46m"
#define GREY_BG "\033[47m"
//Effacement
#define RESET "\033[0m" /*réinitialise couleur affichage*/
#define CLEAR "\033[H\033[J" /*efface terminal*/

//Librairies
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//Structures
typedef enum {FALSE,TRUE} Boolean;
typedef enum {N,NE,E,SE,S,SW,W,NW} Orientation;
typedef enum {HEALTHY, INFECTED, SICK, SICK_IN_REMISSION} DiseaseStage;
typedef enum {LAMBDA, DOCTOR} PersonType;
typedef enum {CONSOLE, GRAPHIC} Display;
typedef struct Coordinates
{
    int row;
    int col;
}Coordinates;

typedef struct Person
{
    PersonType identity;
    Coordinates coordPerson;
    Orientation direction;
    DiseaseStage infection;
    int roundInfection;

}Person;

typedef struct Cell
{
    int gradient;
    Person* person;
}Cell;

#endif