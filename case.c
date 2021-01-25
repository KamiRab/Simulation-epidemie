#include "case.h"
#include "deplacement.h"
#include "simulation.h"

// Initialise case vide de la matrice
void init_void(Cell *c, int row, int col, int *array_virus) {
    array_virus[row + col * R] = 0; /*row+col*R = identifiant unique de la case*/
    c->gradient = 0;
    c->person = NULL;
}

// Initialise bonhomme
void init_person(Person *pPerson, int i, int j, PersonType identity) {
    pPerson->identity = identity;
    pPerson->infection = HEALTHY;
    pPerson->roundInfection = 0;
    pPerson->coordPerson.col = j;
    pPerson->coordPerson.row = i;

    // Initialisation de la direction
    pPerson->direction = rand() % 8; /*les directions sont associées à des constantes entières,
                                    * on peut donc les manipuler comme des entiers*/
}

// Fait disparaitre bonhomme à sa mort
void death_person(Cell grid[R][C], int *nb_Person, Person *pSick, Person array_person[R*C]) {
    Person *pLast = &(array_person[(*nb_Person) - 1]);
    if (pSick == pLast) {/*si bonhomme mort est le pLast dans son tableau
                            * pas besoin de remplir case vide dans tableau*/
        grid[pSick->coordPerson.row][pSick->coordPerson.col].person = NULL;
    } else {
        grid[pSick->coordPerson.row][pSick->coordPerson.col].person = NULL; /*"vide" case de la grille*/
        *pSick = *pLast; /*rempli case vide par pLast bonhomme du tableau*/
        grid[pSick->coordPerson.row][pSick->coordPerson.col].person = pSick; /*réaffecte "pLast bonhomme" à sa case*/
    }
    *nb_Person -= 1;/*annule pointeur vers bonhomme décédé*/
}

// Teste si case sans bonhomme
Boolean empty_cell(Cell c) {
    Boolean flag = FALSE;
    if (c.person == NULL) flag = TRUE;
    return flag;
}

// Téléportation du bonhomme de l'autre côté de la matrice, si il en sort
Coordinates teleportation(Coordinates coord) {
    if (coord.col < 0) coord.col += C;
    else if (coord.col >= C) coord.col -= C;
    if (coord.row < 0) coord.row += R;
    else if (coord.row >= R) coord.row -= R;
    return coord;
}

// Ajout du gradient
void add_gradient(Cell grid[R][C], int row, int col) {
    Coordinates coord_gradient;
    grid[row][col].gradient += 16; // gradient initialisé à 16 pour être toujours supérieur aux autres cases sans malade
    for (int i = 1; i <= 3; i++) { // 3 niveaux de gradients +1,+2 et +3 selon si malade ou case adjacente
        for (int j = -3 + i; j <= 3 - i; ++j) {
            coord_gradient.row = row + j; /*ligne de la case à grader*/
            for (int k = -3 + i; k <= 3 - i; ++k) {
                coord_gradient.col = col + k; /*colonne de la case à grader*/
                coord_gradient = teleportation(coord_gradient); /*verifie que coordonnees dans grille*/
                grid[coord_gradient.row][coord_gradient.col].gradient += 1;
            }
        }
    }
}

//Suppression du gradient
void remove_gradient(Cell grid[R][C], int row, int col) {
    Coordinates coord_gradient;
    grid[row][col].gradient -= 16; //enleve initialisation du gradient du malade
    for (int i = 1; i <= 3; ++i) {
        for (int j = -3 + i; j <= 3 - i; ++j) {
            coord_gradient.row = row + j;
            for (int k = -3 + i; k <= 3 - i; ++k) {
                coord_gradient.col = col + k;
                coord_gradient = teleportation(coord_gradient);
                grid[coord_gradient.row][coord_gradient.col].gradient -= 1;
            }
        }
    }
}
