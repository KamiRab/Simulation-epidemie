#ifndef AFFICHAGE_GRAPHIQUE_H
#define AFFICHAGE_GRAPHIQUE_H

#include "utils.h"

// Initialisation de la fenêtre
void init_graphic(SDL_Window **pWindow, SDL_Renderer **pRenderer);
/* procédure créant la fenêtre et le rendu nécessaires à l'affichage graphique
 * et donnant une couleur de fond au rendu*/

// Affichage image
void display_image(SDL_Renderer *pRenderer, SDL_Texture *texture, int coord_x, int coord_y,
                   int window_width, int window_heigth, int color);
/* procédure prenant en paramètre un rendu, une image (texture), des coordonnées, les dimensions de la fenêtre
 * et la couleur du gradient
 * Elle copie l'image (texture) aux coordonnées sur le rendu et colore le fond*/

//Choix des images à afficher
void display_entity(Cell grid[R][C], int virus_array[R * C], SDL_Renderer *pRenderer,
                    int window_width, int window_heigth, char display_gradient);
/* procédure qui prend en paramètre la grille, le tableau des virus, le rendu, les dimensions de la fenêtre
 * et la réponse à la question du gradient
 * Parcourt la grille et le tableau des virus et si une entité est présence dans la case,
 * affiche l' image correspondante
 * Si la case a un gradient, elle donne la valeur de la couleur, sauf si le malade est en rémission,
 * ils ont une couleur verte pour pouvoir les différencier des autres malades*/

// Affichage de la grille et gestion des évènements
Boolean graphic_display(Cell grid[R][C], int virus_array[R * C], SDL_Renderer *pRenderer, int round,
                        SDL_Window *pWindow, char display_gradient);
/* fonction qui prend en paramètre la grille, le tableau des virus, le rendu, le tour actuel et la réponse au gradient
 * Elle permet de redimensionner les images, si la fenêtre est redimensionnée.
 * Si on est au tour d' initialisation, elle affiche la grille directement, sinon elle attend un clic gauche, un appui
 * sur espace ou flèche droite ou entrée pour passer au tour suivant.
 * Si l'utilisateur ferme la fenêtre ou appuie sur echap, elle renvoie FALSE, sinon tant qu'un affichage a eu lieu,
 * elle renvoie TRUE*/

// Fermeture de la fenêtre
void quit(SDL_Window *pWindow, SDL_Renderer *pRenderer);
/* procédure pour sortir de l' affichage graphique :
 * elle détruit le rendu et la fenêtre et libère les fonctions SDL*/

#endif
