# include <stdio.h>
# include <stdlib.h>
# include <SDL.h>
# include <SDL_Image.h>
# include <SDL_ttf.h>
# include "menu.h"


int main(int argc, char **argv)
{
    SDL_Surface *ecran = NULL;
    SDL_Init(SDL_INIT_VIDEO);
    ecran = SDL_SetVideoMode(35/2*20+(35/2+1)*10,35/2*20+(35/2+1)*10,32,SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption(" Jeux de Labyrinthe ",NULL);

    int choix=1; // ici on a mit le 1 pour entrer la 1er fois à la boucle sinon on peut la remplacer avec do{....}while();
    while(choix)
    {
        choix = menu (ecran); // Le retourne du la fonction menu qui va controller quel menu  sera chargé à l'écran
        switch (choix)
        {
            case 1:
            labyrinthe(ecran);
            break;
            case 2 : labyrinthe(ecran,choix);
            break;


        }
    }

    SDL_Quit();
    exit(EXIT_SUCCESS);
}
