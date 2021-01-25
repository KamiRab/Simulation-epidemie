#include "simulation.h"
#include "deplacement.h"
#include "case.h"



// Mise à jour des entités
void simul_lambda(Cell grid[R][C], Person lambda_array[C * R], int *nb_lambda, int nb_round,
                  int virus_array[C * R], int *nb_free_virus) {
    Person *pLambda;
    Boolean movement_done, is_dead;
    for (int index= 0; index < *nb_lambda; index++) {
        pLambda = &(lambda_array[index]);
        if (pLambda->infection != SICK && pLambda->infection != SICK_IN_REMISSION) {
            // Deplacement
            movement_done = reaction_sick(grid, &(pLambda->coordPerson));
            if (movement_done == FALSE) {
                movement_without_reaction(grid, pLambda);
            }
            if (pLambda->infection == INFECTED) {
                // Teste si infecté est guéri
                if (nb_round - pLambda->roundInfection == 7) {
                    pLambda->infection = HEALTHY;
                    pLambda->roundInfection = 0;
                    // Teste si contamination d' une autre case
                } else {
                    virus_dispersal(grid, pLambda, virus_array, nb_free_virus, nb_round);
                }
            } else { // sain
                // Verifie si bH sur case avec virus et met à jour stade d' infection
                test_infection(grid, pLambda, nb_round, virus_array, nb_free_virus);
            }
        } else { //malade ou en rémission
            //Teste si le malade est mort
            is_dead = test_death(grid, pLambda, lambda_array, nb_lambda, virus_array, nb_free_virus);
            if (is_dead == TRUE){ /*si le dernier élément du tableau a été déplacé, on décrémente le compteur de la boucle,
                                    * pour que la prochaine itération soit la sienne*/
                index--;
            }
        }
    }
}

void simul_doctor(Cell grid[R][C], Person doctor_array[C * R], int *nb_doctor, int nb_round,
                  int virus_array[R * C], int *nb_free_virus) {
    Person *pDoctor;
    Boolean movement_done, curing_done, is_dead;
    for (int index = 0; index < *nb_doctor; index++) {
        pDoctor = &(doctor_array[index]);
        if (pDoctor->infection != SICK && pDoctor->infection != SICK_IN_REMISSION) {
            //Soin
            curing_done = curing_sick(grid, *pDoctor);
            // Deplacement
            if (curing_done == FALSE) {
                movement_done = reaction_sick(grid, &(pDoctor->coordPerson));
                if (movement_done == FALSE) {
                    movement_without_reaction(grid, pDoctor);
                }
            }
            if (pDoctor->infection == INFECTED) {
                // Teste si infecté est guéri
                if (nb_round - pDoctor->roundInfection == 7) {
                    pDoctor->infection = HEALTHY;
                    pDoctor->roundInfection = 0;
                    // Teste si contamination d' une autre case
                } else {
                    virus_dispersal(grid, pDoctor, virus_array, nb_free_virus, nb_round);
                }
            } else { // sain
                // Verifie si bonhomme est sur case avec virus et le cas échéant met à jour son stade d' infection
                test_infection(grid, pDoctor, nb_round, virus_array, nb_free_virus);
            }
        } else { //malade ou en remission
            is_dead=test_death(grid, pDoctor, doctor_array, nb_doctor,virus_array,nb_free_virus);
            if (is_dead == TRUE){
                index--;
            }
        }
    }
}


void simulation_virus(int virus_array[R * C], int *nb_free_virus) {
    int index;
    for (int row = 0; row < R; row++) {
        for (int col = 0; col < C; col++) {
            index = row + col * R;
            if (virus_array[index] > 0) { /*décrémente vie que des virus existants*/
                if (virus_array[index] == 1) { /*si le virus n'a plus qu' un point de vie il meurt */
                    (*nb_free_virus)--;
                }
                virus_array[index] -= 1;  /*enlève un point de vie à tous les virus*/
            }
        }
    }
}


// Fonction de soin
Boolean curing_sick(Cell grid[R][C], Person doctor) {
    int nb_of_direction_tested= 0;
    Coordinates research_coor;
    Boolean near_sick = FALSE;
    Person *hypo_sick;
    while (nb_of_direction_tested< 8 && near_sick == FALSE) {
        research_coor = coordinates_change(doctor.coordPerson, nb_of_direction_tested); /*coordonnées de la case
                                                                                        * dans la direction testée*/
        hypo_sick = grid[research_coor.row][research_coor.col].person; /*bonhomme dans la case testée*/
        if (hypo_sick != NULL) {

            // Si bonhomme est malade, le gradient est enlevé et il devient malade en rémission
            if (hypo_sick->infection == SICK) {
                near_sick = TRUE;
                remove_gradient(grid, research_coor.row, research_coor.col);
                hypo_sick->infection = SICK_IN_REMISSION;

                // Si le bonhomme est déjà en remission, il devient sain
            } else if (hypo_sick->infection == SICK_IN_REMISSION) {
                near_sick = TRUE;
                hypo_sick->infection = HEALTHY;
            } else {
                near_sick = FALSE;
            }
        } else {
            near_sick = FALSE;
        }
        nb_of_direction_tested++;
    }
    return near_sick;
}


// Test si infecté est symptomatique ou asymptomatique
void test_infection(Cell grid[R][C], Person *pPerson, int nb_round,
                    int virus_array[C * R], int *nb_free_virus) {
    int random;
    int person_row = pPerson->coordPerson.row;
    int person_col = pPerson->coordPerson.col;

    // Si le malade est sur une case avec un virus existant
    if (virus_array[person_row + person_col * R] > 0) {
        random = rand() % 100;

        // Bonhomme tombe malade
        if (random < PROBA_MALADE_SI_INFECTE) {
            pPerson->infection = SICK;
            // initialisation gradient
            add_gradient(grid, person_row, person_col);


            // Bonhomme devient infecté asymptomatique
        } else {
            pPerson->infection = INFECTED;
            pPerson->roundInfection = nb_round;
        }
        virus_array[person_row + person_col * R] = 0; /*le virus entre dans le corps de l' infecté, il n' est plus libre sur la grille*/
        (*nb_free_virus)++;
    }
}

//Test si malade meurt
Boolean test_death(Cell grid[R][C], Person *pSick, Person array_person[R * C], int *nb_person,
                   int virus_array[R*C], int* nb_free_virus) {
    Boolean is_dead = FALSE;
    int sick_row = pSick->coordPerson.row;
    int sick_col = pSick->coordPerson.col;
    int random = rand() % 100;
    if (random < VIRULENCE) {
        is_dead = TRUE;
        if (pSick->infection == SICK){ //pas en remission
            remove_gradient(grid, sick_row, sick_col); /*enlève gradient*/
        }
        death_person(grid, nb_person, pSick, array_person); /*bonhomme disparait de la grille et tableau*/

        // Apparition d' un virus sur la grille
        virus_array[sick_row+sick_col*R] = 4;
        nb_free_virus+=1;
    }
    return is_dead;
}

// Dispersion du virus
void virus_dispersal (Cell grid[R][C], Person *pInfected, int virus_array[R * C], int *nb_free_virus, int round) {
    Coordinates dispersion_coord;
    Person *pLambda;
    Coordinates infected_coord = pInfected->coordPerson;
    Cell infected_cell;
    int random = rand() % 100;
    if (random < PROBA_DISPERSION_VIRUS) { //dispersion des virus sur case de l'infecté ou une case adjacente
        random = rand() % 9;
        if (random < 8){
            dispersion_coord = coordinates_change(infected_coord, random); /*case infectée est adjacente*/
        } else {
            dispersion_coord = infected_coord; /*case infectée est celle du bonhomme*/
        }

        if(virus_array[dispersion_coord.row + dispersion_coord.col * R] == 0){
            //Ajout du virus
            virus_array[dispersion_coord.row + dispersion_coord.col * R] = 4;
            *nb_free_virus +=1 ;

            infected_cell = grid[dispersion_coord.row][dispersion_coord.col];

            //Update du statut du bonhomme dans la case infectée
            if (infected_cell.person != NULL && infected_cell.person->identity == LAMBDA){
                pLambda = infected_cell.person;
                if (pLambda->infection == HEALTHY){
                    test_infection(grid, pLambda, round, virus_array, nb_free_virus);
                }
            }
        }
    }
}







