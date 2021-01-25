#ifndef SIMULATION_H
#define SIMULATION_H

#include "utils.h"

// Simulation pour chaque entité
void simul_lambda(Cell grid[R][C], Person lambda_array[R * C], int *nb_lambda, int nb_round,
                  int virus_array[R * C], int *nb_free_virus);

/* procédure qui prend en paramètre la grille, le tableau des lambdas et des virus et le nombre de virus , de lambdas et
 * de tour.
 * Elle parcourt le tableau des lambdas et selon le stade d'infection décide de la démarche à suivre :

 * Si le lambda est sain : la procédure teste si il s'est déplace dans la direction opposée au gradient, si non, elle
 * déclenche le déplacement normal.
 * Puis elle teste si il est sur case avec virus.

 * Si le lambda est infecté : la procédure teste si il s'est déplacer dans la direction opposée au gradient, si non, elle
 * déclenche le déplacement normal.
 * Puis elle teste, si il est encore infecté et si son virus s'est dispersé.

 * Si le lambda est malade: la procédure teste si il est mort*/

void simul_doctor(Cell grid[R][C], Person doctor_array[C * R], int *nb_doctor, int nb_round,
                  int virus_array[R * C], int *nb_free_virus);
/* procédure qui prend en paramètre la grille, le tableau des soignants et des virus et le nombre de virus , de soignants et
 * de tour.
 * Elle parcourt le tableau des soignants et selon le stade d'infection décide de la démarche à suivre :

 * Si le soignant est sain : la procédure teste si il est en train de soigner un malade, si non si il peut
 * s'approcher d'un malade. Si non, elle déclenche le déplacement normal.
 * Puis elle teste, si il est sur case avec virus.

 * Si le soignant est infecté : la procédure teste si il est en train de soigner un malade, si non si il peut
 * s'approcher d'un malade. Si non, elle déclenche le déplacement normal.
 * Puis elle teste, si il est encore infecté et si son virus s'est dispersé.

 * Si le soignant est malade : la procédure teste si il est mort*/

void simulation_virus(int virus_array[R * C], int *nb_free_virus);
/* procédure qui prend en paramètre le tableau des virus et le nombre de virus
 * Elle décrémente les points de vie restants des virus et si un virus n'en a plus, elle le fait disparaitre.*/



// Tests sur bonhommes liés au virus
void test_infection(Cell grid[R][C], Person *pPerson, int nb_round, int virus_array[C * R], int* nb_free_virus);
/* procédure qui prend en paramètre la grille, un bonhomme, le nombre de tour et de virus et le tableau des virus
 * Si un virus est présent dans la case, soit le bonhomme devient infecté, soit il tombe malade. Dans les deux cas,
 * le virus n'est plus considéré comme "libre" et disparait du tableau et on change le statut des bonhommes.
 * Si le bonhomme est infecté symptomatique, c'est à dire malade, on crée un gradient autour de lui.
 * Si le bonhomme est infecté asymptomatique, on lui affecte le tour où il a été infecté*/

Boolean test_death(Cell grid[R][C], Person *pSick, Person array_person[R * C], int *nb_person,
                   int virus_array[R*C], int* nb_free_virus);
/* fonction qui prend en paramètre la grille, un malade, son tableau et nombre associés, le tableau de virus et le
 * nombre de virus libres.
 * Si le malade meurt, il disparait de la grille et de son tableau. Un virus est libéré dans sa case.
 * Dans le cas des malades pas en rémission, le gradient est enlevé
 *La fonction retourne TRUE, si le malade est mort et FALSE sinon*/

Boolean curing_sick(Cell grid[R][C], Person doctor);
/* fonction qui prend en paramètre la grille et un soignant.
 * Si le soignant est à côté d'un malade pas déja en cours de soin, le gradient associé est enlevé et le malade devient
 * en rémission
 * Si le soignant est à côté d'un malade en rémission, le malade en rémission devient sain*/

// Dispersion du virus
void virus_dispersal (Cell grid[R][C], Person *pInfected, int virus_array[R * C], int *nb_free_virus, int round);
/* procédure qui prend en paramètre la grille, un bonhomme infecté, le nombre de virus, le tableau des virus
 * et le tour actuel
 * Si un bonhomme est infecté, il contamine sa case ou l'une des cases adjacentes selon une probabilité définie dans utils.h
 * On met ensuite à jour le statut du potentiel lambda qui se trouverait sur la case nouvellement contaminée.*/
#endif
