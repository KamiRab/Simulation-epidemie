#include "affichage_graphique.h"

void init_graphic(SDL_Window **pWindow, SDL_Renderer **pRenderer) {
    // Initialisation du sous système pour video, si une erreur a lieu, il affiche message*/
    if (SDL_Init(SDL_INIT_VIDEO) ==
        -1) { /*initialise sous système pour video, si une erreur a lieu, il affiche message*/
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //Initialisation du support pour les images en format PNG
    IMG_Init(IMG_INIT_PNG);

    //Initialisation écran
    (*pWindow) = SDL_CreateWindow("Simulation infection virus",
                                  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    if ((*pWindow) == NULL) { /*si impossible, affiche message d'erreur*/
        fprintf(stderr, "Impossible d'afficher la fenêtre : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Initialise rendu
    (*pRenderer) = SDL_CreateRenderer(*pWindow, -1, SDL_RENDERER_ACCELERATED);
    if ((*pRenderer) == NULL) {/*si impossible, affiche message d'erreur*/
        fprintf(stderr, "Problème : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Affecte couleur de fond au rendu
    SDL_SetRenderDrawColor(*pRenderer, 0, 0, 0, 255);
    SDL_RenderClear(*pRenderer);
}

void display_image(SDL_Renderer *pRenderer, SDL_Texture *texture, int coord_x, int coord_y,
                   int window_width, int window_heigth, int color) {
    SDL_Rect dest; /*rectangle pour afficher les images*/
    int w = 100, h=100;
    //Coordonnées du rectangle
    dest.x = coord_x;
    dest.y = coord_y;

    dest.w = (window_width-C)/C;
    dest.h = (window_heigth-R)/R;

    //Couleur rectangle
        //Gradient
    if (color == 0) SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 100); /*blanc*/
    else if (color == 1) SDL_SetRenderDrawColor(pRenderer, 185, 255, 255, 100); /*bleu clair*/
    else if (color == 2) SDL_SetRenderDrawColor(pRenderer, 79, 167, 255, 100); /*cyan clair*/
    else if (color == 3) SDL_SetRenderDrawColor(pRenderer, 255, 255, 128, 100); /*jaune*/
        //Malade en rémission
    else if (color == 4) SDL_SetRenderDrawColor(pRenderer, 176 , 255, 176, 100); /*vert*/
    SDL_RenderFillRect(pRenderer,&dest);

    //Affecte image à afficher
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);

    //Copie rectangle sur le rendu de l'affichage
    SDL_RenderCopy(pRenderer, texture, NULL, &dest);
}


void display_entity(Cell grid[R][C], int virus_array[R * C], SDL_Renderer *pRenderer,
                    int window_width, int window_heigth, char display_gradient) {
    //Créé texture pour toute les images d'entité
    SDL_Texture *lambda_texture = IMG_LoadTexture(pRenderer, "./image/lambda.png");
    SDL_Texture *lambda_infected_texture = IMG_LoadTexture(pRenderer, "./image/lambda-infecte.png");
    SDL_Texture *sick_texture = IMG_LoadTexture(pRenderer, "./image/bonhomme-malade.png");
    SDL_Texture *doctor_texture = IMG_LoadTexture(pRenderer, "./image/soignant.png");
    SDL_Texture *doctor_infected_texture = IMG_LoadTexture(pRenderer, "./image/soignant_infecte.png");
    SDL_Texture *virus_texture = IMG_LoadTexture(pRenderer, "./image/virus.png");
    SDL_Texture *empty_texture = IMG_LoadTexture(pRenderer, "./image/vide.png");

    int color_gradient;
    //Parcourt grille pour afficher entité correspondante
    // La taille de l' image est de 20*20, donc pour créér un aspect de grille, on a positionné
    // les images tous les 21 pixels
    for (int row = 0; row < R; ++row) {
        for (int col = 0; col < C; ++col) {
            //Couleur gradient
            if (display_gradient == 'Y' || display_gradient == 'y'){
                if (grid[row][col].gradient == 0) color_gradient = 0;
                else if (grid[row][col].gradient == 1) color_gradient = 1;
                else if (grid[row][col].gradient > 16) color_gradient = 3;
                else if (grid[row][col].gradient >= 2) color_gradient = 2;
            }else color_gradient = 0;


            //Presence d'un bonhomme
            if (grid[row][col].person != NULL) {
                Person *pPerson = grid[row][col].person;
                if (pPerson->infection == SICK) {
                    display_image(pRenderer, sick_texture, window_width / C * col, window_heigth / R * row,
                                  window_width, window_heigth, color_gradient);
                } else if (pPerson->infection == SICK_IN_REMISSION) {
                    display_image(pRenderer, sick_texture, window_width / C * col, window_heigth / R * row,
                                  window_width, window_heigth, 4);
                }else if (pPerson->identity == LAMBDA && pPerson->infection == HEALTHY) {
                    display_image(pRenderer, lambda_texture, window_width / C * col, window_heigth / R * row,
                                  window_width, window_heigth, color_gradient);
                } else if (pPerson->identity == LAMBDA && pPerson->infection == INFECTED) {
                    display_image(pRenderer, lambda_infected_texture, window_width / C * col, window_heigth / R * row,
                                  window_width, window_heigth, color_gradient);
                } else if (pPerson->identity == DOCTOR && pPerson->infection == HEALTHY) {
                    display_image(pRenderer, doctor_texture, window_width / C * col, window_heigth / R * row,
                                  window_width, window_heigth, color_gradient);
                } else {
                    display_image(pRenderer, doctor_infected_texture, window_width / C * col, window_heigth / R * row,
                                  window_width, window_heigth, color_gradient);
                }
            } else if (virus_array[row + col * R] > 0) {
                display_image(pRenderer, virus_texture, window_width / C * col, window_heigth / R * row,
                              window_width, window_heigth, color_gradient);
            } else
                display_image(pRenderer, empty_texture, window_width / C * col, window_heigth / R * row,
                              window_width, window_heigth, color_gradient);
        }
    }
}

Boolean
graphic_display(Cell grid[R][C], int virus_array[R * C], SDL_Renderer *pRenderer, int round,
                SDL_Window *pWindow,char display_gradient) {
    int window_width;
    int window_heigth;
    SDL_GetWindowSize(pWindow, &window_width, &window_heigth);
    // Gestion des évènements au clavier et souris
    SDL_Event ev;

    // Si on est au tour d'initialisation, affiche directement la grille
    if (round == 0) {
        display_entity(grid, virus_array, pRenderer, window_width, window_heigth,display_gradient); /*continue affichage*/
        SDL_RenderPresent(pRenderer);
        return TRUE;
    } else {
        //Attend évènement pour afficher tour suivant ou fermer fenêtre
        while (TRUE) {
            SDL_PollEvent(&ev); /*regarde si évènement a eu lieu*/

            switch (ev.type) {
                //Evènements de fenêtre
                    //Fermeture de la fenêtre
                case SDL_WINDOWEVENT: //gestion de la fermeture de la fenetre avec la croix
                    if (ev.window.event == SDL_WINDOWEVENT_CLOSE) {
                        return FALSE; /*empêche affichage du tour suivant et signal pour arrêter simulation*/

                    // Redimensionnement de la fenêtre
                    } else if (ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                        window_width = ev.window.data1; /*récupère largeur de la fenêtre*/
                        window_heigth = ev.window.data2; /*récupère hauteur*/
                        display_entity(grid, virus_array, pRenderer, window_width, window_heigth,display_gradient);
                        SDL_RenderPresent(pRenderer); /*affiche le nouveau rendu*/
                        return TRUE;
                    }

                    //Evènements d'appuie sur clavier
                case SDL_KEYDOWN:

                        //Appuie sur entrée (RETURN) ou flèche droite ou espace : passe au tour suivant
                    if (ev.key.keysym.sym == SDLK_RETURN
                        || ev.key.keysym.sym == SDLK_SPACE
                        || ev.key.keysym.sym == SDLK_RIGHT) {
                        display_entity(grid, virus_array, pRenderer, window_width,
                                       window_heigth,display_gradient); /*modifie rendu en mémoire*/
                        SDL_RenderPresent(pRenderer); /*affiche le nouveau rendu*/
                        return TRUE; /*autorise le passage au tour suivant*/

                        //Appuie sur echap
                    } else if (ev.key.keysym.sym == SDLK_ESCAPE) {
                        return FALSE;
                    }
                    // Clic gauche : passage au tour suivant
                case SDL_MOUSEBUTTONDOWN:
                    if (ev.button.button == SDL_BUTTON_LEFT) {
                        display_entity(grid, virus_array, pRenderer, window_width, window_heigth,display_gradient);
                        SDL_RenderPresent(pRenderer);
                        return TRUE;
                    }
            }
        }
    }
}

void quit(SDL_Window *pWindow, SDL_Renderer *pRenderer) {
    SDL_DestroyRenderer(pRenderer); /*détruit rendu*/
    SDL_DestroyWindow(pWindow); /*ferme fenêtre*/
    IMG_Quit(); /*libère mémoire allouée au support*/
    SDL_Quit(); /*efface sous-systèmes appelés*/

}