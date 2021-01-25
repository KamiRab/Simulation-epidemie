#ifndef DEPLACEMENT_H
#define DEPLACEMENT_H

#include "utils.h"

// Fonctions pour gérer réaction de deplacement face à un malade
Boolean reaction_sick(Cell grid[R][C], Coordinates *pCoord_person);
/* fonction qui prend en paramètre la grille et les coordonnées d' un bonhomme
 * selon type de bonhomme et le gradient des cases autour de lui décide de s' il a besoin de fuire/se rapprocher.
 * Si la case avec le plus haut gradient autour de lui a un gradient supérieur:
 * si c'est un lambda, il essaie de la fuire
 * si c' est un soignant, il essaie de s'en approcher, sauf si il est déja à côté du malade.
 * La fonction renvoie TRUE si un déplacement a été fait et FALSE sinon */

    // Tri du tableau des directions de déplacement
void bubble_sort_orientation_sick(Cell grid[R][C], Person *pPerson, Orientation order_array[8]);
/* procédure qui prend en paramètre une grille, un bonhomme et un tableau de direction
 * Elle compare les gradients des cases associées aux directions de déplacements 2 à 2
 * Puis elle échange si besoin les directions pour que le tableau soit ordonné de façon croissante
 * selon le gradient des cases associées*/

    // Deplacement dans le sens opposée au gradient
void escape_attempt(Cell grid[R][C], Coordinates *pCoord_person, Orientation order_array[8]);
/* procédure prenant en paramètres la grille, les coordonnées d'un lambda et le tableau des directions ordonnées
* La procédure commence par tester la direction opposée au malade, puis toutes les directions du tableau.
* Pour se déplacer le gradient de la case doit être inférieure à celle d'origine et la case doit être libre.
* Si aucun deplacement est possible, le bonhomme reste sur sa case*/

    // Deplacement dans le sens du gradient
void approach_attempt(Cell grid[R][C], Coordinates *pCoord_Person, Orientation order_array[8]);
/* procédure prenant en paramètres la grille, les coordonnées d'un soignant et le tableau des directions ordonnées
* La procédure teste toutes les directions du tableau dans l' ordre décroissant de gradient associé par tester la direction opposée au malade, puis toutes les directions du tableau.
* Pour se déplacer le gradient de la case doit être supérieur à celle d'origine et la case doit être libre.
* Si aucun deplacement est possible, le bonhomme reste sur sa case*/



// Déplacement si aucun gradient à proximité
void movement_without_reaction(Cell grid[R][C], Person *pPerson);
/* procédure qui prend en paramètre la grille et un bonhomme
 * Elle teste la probabilite de se déplacer et le cas échéant, si la direction est maintenue ou aléatoire */

// Directions du déplacement
void maintain_direction(Cell grid[R][C], Person *pPerson);
/* procédure qui prend en paramètre la grille et un bonhomme
 * Elle déplace le bonhomme dans la direction qu'il avait au tour précédent, si la case est libre.
 * Sinon elle appelle movement_with_obstacle */

void random_direction_change(Cell grid[R][C], Person *pPerson);
/* procédure qui prend en paramètre la grille et un bonhomme
 * Elle déplace le bonhomme dans une direction aléatoire différente du tour précédent, si la case est libre.
 * Sinon elle appelle movement_with_obstacle */

//Type de déplacement selon présence ou non d'un obstacle
void movement_without_obstacle(Cell grid[R][C], Coordinates *pOld_coord, Coordinates new_coord);
/* procédure qui prend en paramètre la grille et les coordonnées actuelles et de déplacement du bonhomme
 * elle déplace le bonhomme sur la grille et actualise ses coordonnées*/

void movement_with_obstacle(Cell grid[R][C], Person *pPerson);
/* procédure qui prend en paramètre la grille et un bonhomme
 * elle teste la probabilité de déplacer dans une direction aléatoire ou de reste sur place */


//Modification de coordonnées selon direction de déplacement
Coordinates coordinates_change(Coordinates coord, Orientation orientation);
/* fonction prenant en paramètre des coordonnées et une direction de déplacement
 * selon la direction, elle modifie la valeur de la colonne et/ou la ligne
 * Puis elle teste, si les coordonnées sont dans la grille, si non elle "téléporte" les coordonnées*/

Orientation random_new_direction(Orientation actual);
/* fonction prenant en paramètre une direction de déplacement
 * elle renvoie une direction différente de celle donnée en paramètre*/

Orientation fixing_direction(Orientation actual);
/* fonction prenant en paramètre une direction de déplacement
 * elle vérifie que la direction est présente dans l'énumération des directions,
 * si non elle corrige la constante associée pour avoir celle qui était voulue.
 * Elle permet de corriger les opérations faites sur les constantes des directions*/

#endif
