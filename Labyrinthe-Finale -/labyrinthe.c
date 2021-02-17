
# include <stdio.h>
# include <stdlib.h>
# include <SDL.h>
# include <SDL_Image.h>
# include <SDL_ttf.h>
# include <time.h>
int** visiter;
const int nombreCase=35;
const int tailleGrosBloc=20; // 20 px  pour les cases du Vide
const int taillePetitBloc=10; // 10 px pour les cases  les bordures
int fin=0;

enum {Vide, MurHorizontal, MurVertical, Coin, Fin,chemin,Mur,Entrer};
//cette énumération va nous faciliter  la lecture et la maintenance d'un programme.

typedef struct
{
    int ID;
    int typeCase;
    SDL_Surface *texture;
} CASE;

/******************************************************************************************************/
void alloc_visiter()
// Pour créer un tableau des cases déja vister dans le labyrinthe , il va nous aidé pour appliquer l'algorithme de DFS
{
    visiter = malloc(nombreCase * sizeof(int*));
    int i;
    for (i = 0; i < nombreCase; ++i)
    {
        visiter[i] = malloc(nombreCase * sizeof(int*));
    }
}
int dfs(int ligne, int colonne)
// l'application de l'aglorithme de DFS recursive
{
    int* actuel = &visiter[ligne][colonne];

    if (*actuel == Fin)
    {
        return 1;
    }

    if (*actuel == Vide)
    {
        *actuel = chemin;

        if (dfs(ligne, colonne - 1)!=2)
        {
            *actuel = chemin;
            return 1;
        }
        if (dfs(ligne + 1, colonne)!=2)
        {
            *actuel = chemin;
            return 1;
        }
        if (dfs(ligne, colonne + 1)!=2)
        {
            *actuel = chemin;
            return 1;
        }
        if (dfs(ligne - 1, colonne)!=2)
        {
            *actuel = chemin;
            return 1;
        }
    }
    return 2;

}
/******************************************************************************************************/

void labyrinthe(SDL_Surface *ecran,int choix) // la fonction qui va généré le labyrinthe et son résolution
{
    int i,j;
    fin=0;

    SDL_Rect positionCurseur;


    CASE terrain[nombreCase][nombreCase];

    // Initialisation des cases
    for(i=0; i<nombreCase; i++)  // i colonnes , j : lignes
    {
        // on initialise les cases soit par un mur ou case vide ....
        for(j=0; j<nombreCase; j++)
        {
            terrain[i][j].ID=i+j*nombreCase;
            if(((i-1)%2==0)&&((j-1)%2==0)) terrain[i][j].typeCase=Vide;
            else if((i%2==0)&&(j%2==0)) terrain[i][j].typeCase=Coin;
            else if (j%2==0)terrain[i][j].typeCase=MurHorizontal;
            else if (i%2==0)terrain[i][j].typeCase=MurVertical;
        }
    }

    srand(time(NULL));  // On commence par l'initialisation du générateur de nombres pseudo-aléatoires

    // Generation du labyrinthe
    int nombreCaseRelie=0;
    int valeurAleatoire1;
    int valeurAleatoire2;
    int direction;
    int Largeur2;
    int Hauteur2;

    while (nombreCaseRelie!=(nombreCase/2)*(nombreCase/2)-1)
    {
        valeurAleatoire1=(rand()%(nombreCase/2)+1)*2-1; // le % Pour générer un nombre entre 0 et 33
        Largeur2=valeurAleatoire1;
        valeurAleatoire2=(rand()%(nombreCase/2)+1)*2-1; // le % Pour générer un nombre entre 0 et 33
        Hauteur2=valeurAleatoire2;

        direction=rand()%4; // soit haut,bas,gauche,droite
        switch(direction)
        {
        case 0:
            Hauteur2=Hauteur2-2; // bas
            break;
        case 1:
            Largeur2=Largeur2-2; // gauche
            break;
        case 2:
            Hauteur2=Hauteur2+2; // haut
            break;
        case 3:
            Largeur2=Largeur2+2; // droite
            break;
        default:
            break;
        }
        if ((Hauteur2>=1)&&(Hauteur2<=nombreCase-1)
                &&(Largeur2>=1)&&(Largeur2<=nombreCase-1))
        {
            if(terrain[Largeur2][Hauteur2].ID!=terrain[valeurAleatoire1][valeurAleatoire2].ID)
            {
                int IDTemp=terrain[Largeur2][Hauteur2].ID; // on a stocké l'ID de la nouvelle case .

                terrain[Largeur2][Hauteur2].ID=terrain[valeurAleatoire1][valeurAleatoire2].ID; // on a stocké l'ID de l'ancienne case dans l'ID de la nouvelle Case.
                terrain[(valeurAleatoire1+Largeur2)/2][(valeurAleatoire2+Hauteur2)/2].ID
                    = terrain[valeurAleatoire1][valeurAleatoire2].ID;
                terrain[(valeurAleatoire1+Largeur2)/2][(valeurAleatoire2+Hauteur2)/2].typeCase
                    =Vide; // Qui est responsable de la suppression des bordures des cases
                for(i=0; i<nombreCase; i++)
                {
                    for(j=0; j<nombreCase; j++)
                    {
                        if(terrain[i][j].ID==IDTemp) terrain[i][j].ID=terrain[valeurAleatoire1][valeurAleatoire2].ID;
                    }
                }
                nombreCaseRelie++;
            }
        }
    }

    for(i=0; i<nombreCase; i++) // cette boucle c'est juste pour remplir chaque case avec son role associé sous forme d'une image
    {
        for(j=0; j<nombreCase; j++)
        {
            if(terrain[i][j].typeCase==MurVertical) terrain[i][j].texture=IMG_Load("MurVertical.png");
            if(terrain[i][j].typeCase==MurHorizontal) terrain[i][j].texture=IMG_Load("MurHorizontal.png");
            if(terrain[i][j].typeCase==Coin)
            {
                if (terrain[i][j-1].typeCase==Vide)
                {

                    if(terrain[i-1][j].typeCase==Vide&&terrain[i+1][j].typeCase==Vide) terrain[i][j].texture=IMG_Load("Coin4.png");
                    else if (terrain[i-1][j].typeCase==Vide&&terrain[i][j+1].typeCase==Vide)terrain[i][j].texture=IMG_Load("Coin2.png");
                    else if(terrain[i-1][j].typeCase==Vide)terrain[i][j].texture=IMG_Load("Coin5.png");
                    else if(terrain[i+1][j].typeCase==Vide&&terrain[i][j+1].typeCase==Vide)terrain[i][j].texture=IMG_Load("Coin3.png");
                    else if(terrain[i+1][j].typeCase==Vide) terrain[i][j].texture=IMG_Load("Coin6.png");
                    else terrain[i][j].texture=IMG_Load("Coin.png"); // Coin.png --> les coins de labyrinthe ( les bordures )
                }
                else if(terrain[i][j+1].typeCase==Vide&&terrain[i+1][j].typeCase==Vide&&terrain[i-1][j].typeCase==Vide)terrain[i][j].texture=IMG_Load("Coin1.png");
                else if(terrain[i][j+1].typeCase==Vide&&terrain[i+1][j].typeCase==Vide)terrain[i][j].texture=IMG_Load("Coin7.png");
                else if(terrain[i][j+1].typeCase==Vide&&terrain[i-1][j].typeCase==Vide)terrain[i][j].texture=IMG_Load("Coin8.png");
                else terrain[i][j].texture=IMG_Load("Coin.png");
            }
            if(terrain[i][j].typeCase==Vide) terrain[i][j].texture=IMG_Load("Vide.png");

        }
    }
    terrain[33][33].typeCase=Fin;
    terrain[33][33].texture=IMG_Load("Fin.png"); // 1 er sortie

    terrain[1][33].typeCase=Fin;
    terrain[1][33].texture=IMG_Load("Fin.png"); // 2eme sortie
    /******************************************************************************************************/

    int enterAleatoire=(2*rand()%(33/2)+1)*2-1; // pour générer la position de l'entter aléatoire

    terrain[enterAleatoire][enterAleatoire].texture=IMG_Load("Entrer.png"); // L'entreé de débart
    terrain[enterAleatoire][enterAleatoire].typeCase = Entrer;
    /******************************************************************************************************/


        // la fonction de solution doit etre ici après la génération du labyrinthe et l entrée aléatoire .



    /******************************************************************************************************/
    SDL_Event event;

    int continuer=1;
    while(continuer) // le while pour laisser le labyrinthe reste afficher qu'on le génère ( c'est une boucle infinie si continuer resete à 1)
    {
        SDL_Delay(1);
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                exit(EXIT_SUCCESS);
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                case SDLK_ESCAPE: // Cliquer sur ESCAPE du clavier pour revenir au menu précédent
                    continuer=0;
                    choix = 1; // Quand on clique la 1 er fois sur générer et résoudre et on sort on doit remettre choix à 1 sinon quand on va cliquer sur le 1 er button ( càd générer ) on aura la solution mais nous on veut juste la génération
                    break;
                default:
                    break;
                }
                break;
            }
        }
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format,10, 179, 233)); // la couleur du background du labyrinthe

        positionCurseur.x=0;
        positionCurseur.y=0;

        for(i=0; i<nombreCase; i++) // Pour coller les surfaces crées  à l'écran.
        {
            for(j=0; j<nombreCase; j++)
            {
                SDL_BlitSurface(terrain[i][j].texture, NULL, ecran, &positionCurseur);


                if(j%2==0)positionCurseur.y+=taillePetitBloc;
                else positionCurseur.y+=tailleGrosBloc;
            }
            if(i%2==0) positionCurseur.x+=taillePetitBloc;
            else positionCurseur.x+=tailleGrosBloc;
            positionCurseur.y=0;
        }

        /********************************************************************************************************/
  /*  /*Le Début de la résolution de Labyrinthe DFS */

  // /*
    if(choix == 2){ // càd on fait la solution juste qu'on clique sur le button de la solution
            alloc_visiter(); // alouer un tableau pour les cases visiter

                // voir les cases visiter
                for (i = 0; i < nombreCase; ++i)
                {
                    for (j = 0; j < nombreCase; ++j)
                    {

                        if (terrain[i][j].typeCase==MurVertical)
                        {
                            visiter[i][j] = Mur;
                        }

                        else if (terrain[i][j].typeCase==MurHorizontal)
                        {
                            visiter[i][j] = Mur;
                        }

                        else if (terrain[i][j].typeCase==Coin)
                        {
                            visiter[i][j] = Mur;
                        }
                        else if (terrain[i][j].typeCase == Fin)
                        {
                            visiter[i][j] = Fin;
                        }
                        else
                        {
                            visiter[i][j] = Vide;
                        }
                    }
                }
                if (dfs(enterAleatoire,enterAleatoire)!=2) // !=2 càd on a trouvé la FIN  ( dfs() return 1 si il a trouvé Fin )
                {

                    for (i = 1; i < nombreCase; ++i)
                    {
                        for (j = 1; j < nombreCase; ++j)
                        {
        //
                            if (terrain[i][j].typeCase != Fin && visiter[i][j] == chemin)
                            {
                                if (terrain[i][j].typeCase == Entrer) continue ; // pour ne pas remplacer l'icon de L' enter

                                terrain[i][j].texture=IMG_Load("solution.png");
                            }
                        }
                    }

                }
    }
   // */

        /********************************************************************************************************/
        SDL_Flip(ecran);
    }
}
