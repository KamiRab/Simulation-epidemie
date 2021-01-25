#include "affichageconsole.h"

void console_display(Cell grid[R][C], int array_virus[R * C], char display_gradient) {
    for (int row = 0; row < R; row++) {
        printf("%2d  ", row); // affiche numéro de la ligne
        for (int col = 0; col < C; col++) {

            // Si l' utilisateur veut afficher le gradient : couleur de fond sont modifiées selon gradient de la case
            if ((display_gradient == 'Y' || display_gradient == 'y') && grid[row][col].gradient != 0) {
                if (grid[row][col].gradient > 16) printf(YELLOW_BG);
                else if (grid[row][col].gradient >= 2) printf(BLUE_BG);
                else if (grid[row][col].gradient == 1) printf(CYAN_BG);
            }
            if (grid[row][col].person != NULL) {
                print_person(grid[row][col].person); /*affiche bonhomme*/

            } else if (array_virus[row + col * R] > 0) {
                printf(MAGENTA "V"RESET); /*affiche virus*/

            } else {
                printf("." RESET); /*si aucune entité */
            }
        }
        printf(RESET"\n");
    }
}


void print_person(Person *pPerson) {
    if (pPerson->infection == SICK) {
        printf(RED "M" RESET);
    } else if (pPerson->infection == SICK_IN_REMISSION) {
        printf(YELLOW "M" RESET);
    } else if (pPerson->identity == LAMBDA) {
        if (pPerson->infection == INFECTED) {
            printf(ORANGE);
        } else if (pPerson->infection == HEALTHY) {
            printf(GREEN);
        }
        print_direction(pPerson);

    } else if (pPerson->identity == DOCTOR) {
        if (pPerson->infection == HEALTHY) {
            printf(GREEN "S" RESET);
        } else if (pPerson->infection == INFECTED) {
            printf(ORANGE "S" RESET);
        }
    }
}

void print_direction(Person *pPerson) {
    if (pPerson->direction == N) printf("\u2193");
    else if (pPerson->direction == E) printf("\u2192");
    else if (pPerson->direction == S) printf("\u2191");
    else if (pPerson->direction == W) printf("\u2190");
    else if (pPerson->direction == SE) printf("\u2198");
    else if (pPerson->direction == NW) printf("\u2196");
    else if (pPerson->direction == SW) printf("\u2199");
    else printf("\u2197");
    printf(RESET);
}