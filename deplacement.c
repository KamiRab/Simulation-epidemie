#include "deplacement.h"
#include "case.h"

// Vérification de si malade à proximite
Boolean reaction_sick(Cell grid[R][C], Coordinates *pCoord_person) {
    Cell *pCell_person = &(grid[pCoord_person->row][pCoord_person->col]);
    Person *pPerson = pCell_person->person;
    Coordinates oldCoord = *pCoord_person;
    Boolean movement_done = FALSE;

    // Création d'un tableau de direction à tester
    Orientation order_test_array[8] = {N, NE, E, SE, S, SW, W, NW};
    bubble_sort_orientation_sick(grid, pPerson, order_test_array); /* trie  du tableau de direction de déplacement
                                                                    * selon le gradient de la case associée*/
    // Case avec le gradient le plus élévé (lambda veut la fuire et le soigant s'en approcher)
    Coordinates coord_to_test = coordinates_change(oldCoord, order_test_array[7]);
    Cell cell_tested = grid[coord_to_test.row][coord_to_test.col];

    // Test pour savoir déplacement à faire, si besoin est
    if (cell_tested.gradient > pCell_person->gradient) {
        movement_done = TRUE;
        if (pPerson->identity == LAMBDA) {
            escape_attempt(grid, pCoord_person, order_test_array);
        } else if (cell_tested.gradient < 16) { /*soignant pas déjà à côté de malade*/
            approach_attempt(grid, pCoord_person, order_test_array);
        }
    } else movement_done = FALSE; /*aucun malade à proximité*/
    return movement_done;
}

// Trie du tableau des directions de déplacement
void bubble_sort_orientation_sick(Cell grid[R][C], Person *pPerson, Orientation order_array[8]) {
    int exchange = 1;
    Orientation buffer;
    Coordinates coord, next_coord;
    int gradient, next_gradient;
    do {
        exchange = 0;
        // Test du gradient pour chaque case autour du bonhomme
        for (int direction = 0; direction < 7; ++direction) {
            coord = coordinates_change(pPerson->coordPerson, order_array[direction]);
            gradient = grid[coord.row][coord.col].gradient;

            // Gradient de la case de la direction suivante
            next_coord = coordinates_change(pPerson->coordPerson, order_array[direction + 1]);
            next_gradient = grid[next_coord.row][next_coord.col].gradient;

            //Comparaison des deux valeurs de gradient
            if (gradient > next_gradient) { /*la direction actuelle est associée à un gradient supérieur,
                                            * on échange les deux directions, pour les trier dans l'ordre croissant*/
                buffer = order_array[direction];
                order_array[direction] = order_array[direction + 1];
                order_array[direction + 1] = buffer;
                exchange = 1;
            }
        }
    } while (exchange);
}


// Fuite à l'opposé du gradient
void escape_attempt(Cell grid[R][C], Coordinates *pCoord_person, Orientation order_array[8]) {
    Cell origin_cell = grid[pCoord_person->row][pCoord_person->col], new_case;
    Person *pPerson = grid[pCoord_person->row][pCoord_person->col].person;
    Coordinates newCoord, old_Coord = *pCoord_person;
    Orientation reaction_orientation;
    Boolean movement_done = FALSE;
    int direction_to_test = 0;


    // Direction de fuite préférée est la direction opposée au malade
    reaction_orientation = fixing_direction(order_array[7] - 4);
    newCoord = coordinates_change(old_Coord, reaction_orientation);
    new_case = grid[newCoord.row][newCoord.col];

    //Teste si case préférée est libre, si non teste autre direction
    if (empty_cell(new_case) == TRUE && new_case.gradient < origin_cell.gradient) {
        movement_without_obstacle(grid, &(pPerson->coordPerson), newCoord);
        pPerson->direction = reaction_orientation; /*affecte direction de fuite au bonhomme*/

    } else { /*case de la direction préférée n'est pas libre, doit tester tout le tableau,
                * sauf la dernière qui correspond à celle du gradient le plus élevé*/
        while (direction_to_test < 7 && movement_done == FALSE) {
            reaction_orientation = fixing_direction(order_array[direction_to_test]);
            newCoord = coordinates_change(old_Coord, reaction_orientation);
            new_case = grid[newCoord.row][newCoord.col];

            //Verification si la case à tester a un gradient supérieur à celle d'origine
            if (new_case.gradient > origin_cell.gradient) {
                movement_done = TRUE; /* toutes les cases testées après auront un gradient supérieur,
                                    * le bonhomme doit rester à sa place*/
            } else {
                //Verification que la case est libre
                if (empty_cell(new_case) == TRUE) {
                    movement_without_obstacle(grid, &(pPerson->coordPerson), newCoord);
                    pPerson->direction = reaction_orientation;
                    movement_done = TRUE;
                } else {
                    direction_to_test++;
                }
            }
        }
    }
}

// Rapprochement dans le sens du gradient
void approach_attempt(Cell grid[R][C], Coordinates *pCoord_Person, Orientation order_array[8]) {
    Cell origin_cell = grid[pCoord_Person->row][pCoord_Person->col], new_case;
    Person *pPerson = grid[pCoord_Person->row][pCoord_Person->col].person;
    Coordinates newCoord, old_Coord = *pCoord_Person;
    Orientation reaction_orientation;
    Boolean movement_done = FALSE;
    int direction_to_test = 7;

    //Teste direction dans l'ordre décroissant des gradients associées aux directions
    while (direction_to_test >= 0 && movement_done == FALSE) {
        reaction_orientation = fixing_direction(order_array[direction_to_test]);
        newCoord = coordinates_change(old_Coord, reaction_orientation);
        new_case = grid[newCoord.row][newCoord.col];
        if (new_case.gradient < origin_cell.gradient) {
            movement_done = TRUE; /* toutes les cases testées après auront un gradient inférieur,
                                    * le bonhomme doit rester à sa place*/
        } else {
            //Verification que la case est libre
            if (empty_cell(new_case) == TRUE) {
                movement_without_obstacle(grid, &(pPerson->coordPerson), newCoord);
                pPerson->direction = reaction_orientation;
                movement_done = TRUE;
            } else {
                direction_to_test--;
            }
        }
    }
}

// Deplacement si aucun malade à proximité
void movement_without_reaction(Cell grid[R][C], Person *pPerson) {
    int random = rand() % 100;
    if (random < PROBA_DEPLACEMENT) {/* Bonhomme se déplace */
        random = rand() % 100;
        if (random < PROBA_MAINTIEN_DIRECTION) maintain_direction(grid, pPerson);
        else random_direction_change(grid, pPerson);
    }/*Bonhomme ne se déplace pas*/

}

void maintain_direction(Cell grid[R][C], Person *pPerson) {
    // Case dans la direction du bonhomme
    Coordinates oldCoord = pPerson->coordPerson;
    Coordinates newCoord = coordinates_change(oldCoord, pPerson->direction);

    //Test pour vérifier si case est libre
    if (empty_cell(grid[newCoord.row][newCoord.col]) == TRUE) { // teste si case libre
        movement_without_obstacle(grid, &(pPerson->coordPerson), newCoord); //si case libre fait deplacement
    } else movement_with_obstacle(grid, pPerson); /*présence d'un obstacle*/
}

void random_direction_change(Cell grid[R][C], Person *pPerson) {
    Coordinates oldCoord = pPerson->coordPerson;

    // Case dans direction aléatoire
    Orientation newDirection = random_new_direction(pPerson->direction);
    Coordinates newCoord = coordinates_change(oldCoord, newDirection);

    // Test pour vérifier si case est libre
    if (empty_cell(grid[newCoord.row][newCoord.col]) == TRUE) {
        movement_without_obstacle(grid, &(pPerson->coordPerson), newCoord);
        pPerson->direction = newDirection;
    } else movement_with_obstacle(grid, pPerson); /*il y a un obstacle*/
}

void movement_without_obstacle(Cell grid[R][C], Coordinates *pOld_coord, Coordinates new_coord) {
    // Deplacement sur la grille
    grid[new_coord.row][new_coord.col].person = grid[pOld_coord->row][pOld_coord->col].person;
    grid[pOld_coord->row][pOld_coord->col].person = NULL;

    //Mise à jour des coordonnées
    *pOld_coord = new_coord;
}

void movement_with_obstacle(Cell grid[R][C], Person *pPerson) {
    int random = rand() % 100;

    if (random < PROBA_CHGT_DIR_OBSTACLE) { /*il change de direction aléatoirement*/
        random_direction_change(grid, pPerson);
    } /*le bonhomme ne se déplace pas*/
}

Coordinates coordinates_change(Coordinates coord, Orientation orientation) {
    if (orientation == N) coord.row -= 1;
    else if (orientation == S) coord.row += 1;
    else if (orientation == E) coord.col += 1;
    else if (orientation == W) coord.col -= 1;
    else if (orientation == NE) {
        coord.row -= 1;
        coord.col += 1;
    } else if (orientation == SE) {
        coord.row += 1;
        coord.col += 1;
    } else if (orientation == SW) {
        coord.row += 1;
        coord.col -= 1;
    } else { /*orientation== NW*/
        coord.row -= 1;
        coord.col -= 1;
    }

    coord = teleportation(coord); /*si coordonnées en dehors de la matrice, "téléportation"*/

    return coord;
}

Orientation random_new_direction(Orientation actual) {
    int random = rand() % 7;
    Orientation new;
    if (random == 0) new = N;
    else if (random == 1) new = NW;
    else if (random == 2) new = W;
    else if (random == 3) new = SW;
    else if (random == 4) new = S;
    else if (random == 5) new = SE;
    else new = E;

    if (new == actual) {
        new = NE;
    }
    return new;
}

Orientation fixing_direction(Orientation actual) {
    // Direction présente dans l'énumération
    if (actual < 8) return actual;

        // Direction en dehors de l'énumération
    else if (actual >= (N - 7)) return actual + 8;
        /*si la direction est "inférieure" à N (premier élément de l'énumération), la constante associée devient
         * supérieure à 4294967289 (constante qui aurait du correspondre à NE le dernier élément de l'énumération),
         * donc on corrige par rapport à (N-7)*/

    else return actual - 8;
}
