#ifndef CASE_H
#define CASE_H

#include "utils.h"

//Fonction qui initialise les cases comme vides
void init_void(Cell *c, int row, int col, int array_virus[R*C]);
/* procédure qui prend en paramètre une case, ses coordonnées et le tableau des virus
 * La procédure affecter un pointeur vide pour le champ bonhomme et une valeur 0 pour le gradient
 * Le virus avec l'indice correspondant aux coordonnées est initialisé comme non existant */

//Fonctions pour créer ou faire disparaître un bonhomme
void init_person(Person *pPerson, int i, int j, PersonType identity);
/* procédure prenant en paramètre un bonhomme avec les coordonnées de la case où on veut l' initialiser
 * et son type. Le bonhomme est initialisé comme sain, avec une direction attribuée aléatoirement et
 * les coordonnées de la cases lui sont attribuées.*/


void death_person(Cell grid[R][C], int *nb_Person, Person *pSick, Person array_person[R * C]);
/* procédure prenant en paramètre la grille, un malade et son tableau et nombre correspondants
 * la procédure supprime le bonhomme de la grille et du tableau.
 * Si le bonhomme ne correspond pas au dernier bonhomme existant du tableau, on déplace ce dernier
 * pour combler le vide causé par la disparition du bonhomme mort.
 * Le nombre de bonhomme est décrémenté pour ne pas itérer sur des bonhommes morts dans les autres fonctions*/


//Fonction pour tester si case est vide
Boolean empty_cell(Cell c);
/* fonction qui prend en paramètre une case et teste si celle ci contient un bonhomme
 * Si elle est vide, la fonction retourne TRUE, sinon elle retourne FALSE*/



//Fonctions pour ajouter/supprimer gradient
void add_gradient(Cell grid[R][C], int row, int col);
/* procédure qui prend en paramètre la grille et les coordonnées d'un malade.
 * Elle créé (si certaines cases sont déjà dans un gradient, elle somme les deux) un gradient autour du malade
 * en ajoutant +1 à toutes les cases à moins de 3 cases du malade, puis +1 à toutes les cases à moins de 2 cases
 * et +17 (+1 + initialisation à 16) à la case du malade. On pourra ainsi toujours la différencier des autres*/

void remove_gradient(Cell grid[R][C], int row, int col);
/* procédure qui prend en paramètre la grille et les coordonnées d'un malade.
 * Elle fait l'inverse de la procédure en enlevant -1 à toutes les cases à moins de 3 cases,
 * puis encore -1 à celle à moins de 2 cases de distance et -17 à la case du malade.*/


// Fonction pour gérer absence de "mur"
Coordinates teleportation(Coordinates coord);
/* procédure qui prend en paramètre les coordonnées d'une entité.
 * Si les coordonnées sont en dehors de la matrice, il les translate pour que l'entité
 * se "téléporte" à l'opposé dans la matrice*/
#endif
