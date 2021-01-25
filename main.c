#include "utils.h"
#include "affichage_graphique.h"
#include "affichageconsole.h"
#include "case.h"
#include "deplacement.h"
#include "initialisation.h"
#include "simulation.h"

// Fonctions pour demander le nombre de tour à faire/ajouter et le type d' affichage
int add_simulation();
/*fonction demandant à l' utilisateur le nombre de simulation à ajouter et retourne ce nombre*/

void ask_user(int *nb_simulation, Display *display_type, char *presence_gradient);
/*procédure demandant à l' utilisateur le type d' affichage voulu et modifie le pointeur associé
 * Si c'est un affichage graphique, il explique comment changer de tour
 * Puis il demande si l'utilisateur veut afficher le gradient */

char ask_gradient(char answer);
/*fonction demandant à l' utilisateur si il veut afficher le gradient, si il ne répond pas par les valeurs proposées,
 * un message d' erreur est affichée et la question est redemandée*/


int main(int argc, char const *argv[]) {
    // Initialisation de la grille et des tableaux d'entités
    Cell grid[R][C];
    Person lambda_array[R * C], doctor_array[R * C];
    int virus_array[R * C] = {0};

    // Constantes diverses
    int nb_lambda = 0, nb_doctor = 0, nb_free_virus = 0; /*nombres d'entités*/
    int round = 0, round_total = 0; /*tour actuel et nombre de tour total*/
    int add_round = 0; /*nombre de tour ajouté par l' utilisateur*/

    // Type d'affichage et affichage ou non du gradient
    Display display_type= CONSOLE;
    char answer_gradient; /*uniquement si affichage en console*/

    // Variables nécessaires pour l' affichage graphique
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    Boolean continue_display = TRUE; /*permettra arrêt de l'affichage graphique si on ferme la fenêtre*/

    // Initialisation de l' aléa
    srand(time(NULL));



    // Demande du nombre de tours à faire et type d'affichage des simulations
    ask_user(&add_round, &display_type, &answer_gradient);
    round_total = add_round;

    // Génération de la fenêtre d'affichage graphique
    if(display_type == GRAPHIC){
        init_graphic(&pWindow, &pRenderer);
    }

    // Simulation des tours
    while (add_round != 0) { /*si l' utilisateur n'a pas ajouté de tour à la fin, arrêt de la simulation*/
        while (round < round_total) {
            // Initialisation de la grille
            if (round == 0) {
                init_grid(grid, lambda_array, doctor_array, virus_array, &nb_lambda, &nb_doctor, &nb_free_virus);

                // Tour de simulation post-initialisation
            } else { /* tour de simulation pour chaque entité*/
                simul_lambda(grid, lambda_array, &nb_lambda, round, virus_array, &nb_free_virus);
                simul_doctor(grid, doctor_array, &nb_doctor, round, virus_array, &nb_free_virus);
                simulation_virus(virus_array, &nb_free_virus);
            }

            // Affichage
            //En console
            if (display_type == CONSOLE) {
                printf(CLEAR); /*efface affichage déja présent*/
                if (round == 0) printf("Initialisation \n");
                else printf("Tour %d\n", round);
                console_display(grid, virus_array, answer_gradient);
                printf("-----\n");

                // Graphique
            } else {
                continue_display = graphic_display(grid, virus_array, pRenderer,round,pWindow, answer_gradient);
                if (!continue_display) break; /*si la fenêtre a été fermée ou l' utilisateur a appuyé sur echap,
                                                * la boucle est arrêtée*/
            }
            round++;
            usleep(500000); /*met en pause programme pour avoir le temps de voir chaque tour */
        }
        if (!continue_display) break; /*si la fenêtre a été fermée ou l' utilisateur a appuyé sur echap,
                                                * la simulation est arrêtée*/

        // Demande à l' utilisateur si il veut voir d'autres tours
        add_round = add_simulation();
        round_total += add_round;
    }
    if (display_type == GRAPHIC) quit(pWindow, pRenderer); /*ferme affichage graphique*/
    return 0;
}

void ask_user(int *nb_simulation, Display *display_type, char *presence_gradient) {
    printf("Combien de simulation(s) aimeriez-vous faire ?");
    scanf("%d", nb_simulation);

    int display;
    printf("Quel affichage préférez-vous ? Pour celui en console, entrez 0, pour le graphique entrez 1 \n");
    scanf("%d", &display);
    *display_type=display;

    if ((*display_type) == 1) {
        //Message pour expliquer comment passer au tour suivant ou arrêter la simulation
        printf("Pour passer au tour suivant, vous devrez faire un clic gauche ou appuyer sur entrée, "
               "flèche droite ou espace\n"
               "Vous pouvez arrêter la simulation en fermant la fenêtre ou en appuyant sur echap. \n");
    }
    (*presence_gradient) = ask_gradient(*presence_gradient);

}

char ask_gradient(char answer) {
    printf("Voulez vous affichez le gradient ? (1= cyan, 2-9 = bleu, >10 = jaune), "
           "répondez par Y (oui) ou N(non) \n");
    scanf(" %c", &answer);
    if (answer != 'Y' && answer != 'y' && answer != 'N' && answer != 'n') {
        printf("S' il vous plait répondez que pas Y ou N\n");
        ask_gradient(answer);
    }
    return answer;
}

int add_simulation() {
    int ajout = 0;
    printf("Voulez vous ajouter des tours ? Si oui entrez, combien vous voulez, sinon entrez 0\n");
    scanf(" %d", &ajout);
    return ajout;
}
