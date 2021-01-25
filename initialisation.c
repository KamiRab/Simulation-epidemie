#include "initialisation.h"
#include "case.h"

void init_grid (Cell grid[R][C], Person lambda_array[R * C], Person doctor_array[R * C], int virus_array[R * C],
                int *nb_lambda, int *nb_doctor, int *nb_free_virus) {
    int random;
    for (int row = 0; row < R; row++) {
        for (int col = 0; col < C; col++) {
            // Initialise case vide
            init_void(&(grid[row][col]), row, col, virus_array);

            //Initialise si besoin une entité
            random = rand() % 100; /*utilise nombre aléatoire à comparer avec probabilites d'apparition des entités*/
            if (random < PROBA_LAMBDA) {
                // Entité = lambda
                Person *lambda = &(lambda_array[*nb_lambda]); /*créé entité dans tableau*/
                init_person(lambda, row, col, LAMBDA);
                *nb_lambda += 1;
                grid[row][col].person = lambda;

            } else if (random < (PROBA_LAMBDA + PROBA_SOIGNANT)) {
                //Entite = soignant
                Person *soignant = &(doctor_array[*nb_doctor]);
                init_person(soignant, row, col, DOCTOR);
                *nb_doctor += 1;
                grid[row][col].person = soignant;
            } else if (random < (PROBA_LAMBDA + PROBA_SOIGNANT + PROBA_VIRUS)) {
                //Entité = virus
                virus_array[row + col * R] = 4;
                *nb_free_virus += 1;

            } //Aucune entité
        }
    }
}
