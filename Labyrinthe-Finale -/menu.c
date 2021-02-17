# include <stdio.h>
# include <stdlib.h>
# include <SDL.h>
# include <SDL_Image.h>
# include <SDL_ttf.h>

typedef struct
{
    SDL_Rect _position;
    SDL_Rect _positiontexte;
    SDL_Surface *_texte;
    SDL_Surface *_texture;
    int _clic;
} BOUTON;

BOUTON AjoutBouton(int positionX, int positionY, char *texte)
{
    // La fonction qui permet d'ajouter un Button à la surface créer

    BOUTON bouton;
    bouton._position.x=positionX;  // l'emplacement des buttons suivant un axe (ox) et (oy)
    bouton._position.y=positionY;

    bouton._texture=IMG_Load("Bouton_jouer1.png");

    TTF_Font *police = TTF_OpenFont("CaslonRoman.ttf", 32); // la police et sa taille
    SDL_Color couleurNoire = {0, 0, 0}; // la couleur de la police

    bouton._texte=TTF_RenderText_Blended(police, texte, couleurNoire);
    bouton._positiontexte.x=bouton._position.x+bouton._texture->w/2-bouton._texte->w/2;
    bouton._positiontexte.y=bouton._position.y+bouton._texture->h/2-bouton._texte->h/2;

    bouton._clic=0; // on a utilsé 1 ou 0 puisqu' on a pas des boolean en C , pour savoir l'état du button en un temps donné

    return bouton;
}

BOUTON modificationBouton(BOUTON bouton, int sourisX, int sourisY, SDL_Event event)
{
    // l'acouteur des évenenemts pour savoir si on a cliquer sur le button ou pas

    if(sourisX>bouton._position.x&&sourisX<(bouton._position.x+bouton._texture->w) // cette conditon c'est que qu'on la souris est sur le button on charge un style focus puis on atteind l'event du clique
        &&sourisY>bouton._position.y&&sourisY<(bouton._position.y+bouton._texture->h))
    {
        bouton._texture=IMG_Load("Bouton_jouer2.png");
        if (event.button.button == SDL_BUTTON_LEFT) bouton._clic=1; // Quand on clique sur le button , on donne à l' état la valuer 1
    }
    else  bouton._texture=IMG_Load("Bouton_jouer1.png");

    return bouton; // puisque on  a un passage par valeur on retourn le button
}

int menu (SDL_Surface *ecran)
{
    TTF_Init();
    SDL_Event event;

    TTF_Font *police = NULL,*policeRealisation=NULL;
    SDL_Surface *texteTitle = NULL;
    SDL_Color couleurNoire = {0, 0, 0};
    SDL_Rect positionTitle;
    positionTitle.x = 25; // titre
    positionTitle.y = 70; // titre

    SDL_Surface *realisation = NULL;
    SDL_Rect positionRealisation;
    positionRealisation.x = 11; // Realiser par : ....
    positionRealisation.y = 455;

    police = TTF_OpenFont("defused.ttf", 32);
    policeRealisation = TTF_OpenFont("MARYJANE.ttf", 20);
    texteTitle = TTF_RenderText_Blended(police,"ENSIAS     Labyrinthe", couleurNoire);

    realisation = TTF_RenderText_Blended(policeRealisation,"Realise par : Adnane Drief et Khaoula Bouzraf", couleurNoire);

    BOUTON jouer = AjoutBouton(ecran->w/2-150,ecran->h/3," Générer le Labyrinthe "); // le 1 er button

    BOUTON jouer2 = AjoutBouton(ecran->w/2-150,ecran->h*2/3-80,"Générer et résoudre ");  // 2eme button

    BOUTON quiter = AjoutBouton(ecran->w/2-150,ecran->h*2/3+10,"Quitter "); // 3 eme button



    int continuer=1;
    // continuer prend 1 pour entrer la prermier fois à la boucle on peut la remplacer par do{}while();
    while(continuer)
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                // le listner du button Quitter
                case SDL_QUIT:
                exit(EXIT_SUCCESS);
                break;
            }
        }
        jouer = modificationBouton(jouer, event.button.x, event.button.y, event);
        jouer2 = modificationBouton(jouer2, event.button.x, event.button.y, event);

        quiter = modificationBouton(quiter, event.button.x, event.button.y, event);


        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format,10, 179, 233)); // la couleur du background du Menu

        SDL_BlitSurface(texteTitle, NULL, ecran, &positionTitle); // position du titre

        SDL_BlitSurface(realisation, NULL, ecran, &positionRealisation); // position du Réalisation

        SDL_BlitSurface(jouer._texture, NULL, ecran, &jouer._position); // position du 1er button

        SDL_BlitSurface(jouer._texte, NULL, ecran, &jouer._positiontexte); // position du texte du 1er button

        SDL_BlitSurface(jouer2._texture, NULL, ecran, &jouer2._position); // position du 2eme button
        SDL_BlitSurface(jouer2._texte, NULL, ecran, &jouer2._positiontexte);  // position du texte du 2eme button

        SDL_BlitSurface(quiter._texture, NULL, ecran, &quiter._position);
        SDL_BlitSurface(quiter._texte, NULL, ecran, &quiter._positiontexte); //  position du texte du 3eme  button


        if(jouer._clic) // tester l état du 1er button jouer
            return 1;
        if(jouer2._clic) // tester l état du 2eme button jouer

            return 2; // pour choisir le 2 eme menu

        if(quiter._clic)

            return 0;  // return 0 ; pour quitter le programme qu'on on clique sur ce button

        SDL_Flip(ecran);
        SDL_Delay(20);
    }

    SDL_Quit();
    TTF_Quit();
}

