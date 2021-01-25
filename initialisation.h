#ifndef INITIALISATION_H
#define INITIALISATION_H
#include "utils.h"
//initialise grille et tableau lambda, virus, soignant
void init_grid (Cell grid[R][C], Person lambda_array[R * C], Person doctor_array[R * C], int virus_array[R * C],
                int *nb_lambda, int *nb_doctor, int *nb_free_virus);
/* procédure prenant en paramètre les tableaux des bonhommes et du virus et leur nombres associés
 * Elle teste la probabilite d'apparition des entités et le cas échéant initialise
 * les entités sur la grille et incrémente leur nombre associés et virus sur grille et remplit tableau si apparition de l'entité associée*/

#endif