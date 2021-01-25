#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "utils.h"

void console_display(Cell grid[R][C], int array_virus[R*C], char display_gradient);
/*procédure prenant en paramètre un tableau en deux dimensions et
 * selon l' absence ou présence d' une entité et le type d' entité
 * affiche différents éléments sous forme de grille*/

void print_person(Person *pPerson);
/*procédure prenant en paramètre un bonhomme et
 * selon son stade d' infection, son type et orientation décide de l'élément à afficher*/

void print_direction(Person *pPerson);
/*procédure prenant en paramètre un person(lambda) et selon sa direction affiche un symbole différent*/



#endif