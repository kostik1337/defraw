#include <SDL/SDL.h> //sys includes
#include <SDL/SDL_gfxPrimitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "score.h"
#include "objects.h"//my includes
#include "locations.h"
#include "main.h"
#include "defines.h"

Game::Game(){
    atexit(SDL_Quit);
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        exit(1);
    }
    screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if ( !screen ){
        printf("Unable to set video mode: %s\n", SDL_GetError());
        exit(1);
    }
    srand(time(NULL));
    SDL_WM_SetCaption("defraw",NULL);
}

void Game::run(){
    bool done=false;
    SDL_Event event;
    l = new Menu(this);
    current = to = menu;
    while (!done){
        if(current!=to){
            delete l;
            if(to==menu) l = new Menu(this);
            else if(to==game) l = new GameLoc(this);
            else if(to==end) l = new EndLoc(this);
            current = to;
        }
        while(SDL_PollEvent(&event) )
            switch(event.type){
            case SDL_QUIT:
                done = 1;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                case 'q': done = 1; break;
                default: l->event(&event); break;
                }
            break;
            default:
                l->event(&event);
                break;
            }
        l->draw();
        SDL_Flip(screen);
        SDL_Delay(DELAY);
    }
}

void Game::change_loc(loc q){
    to = q;
}

int main (){   
    Game game;
    game.run();
    return 0;
}
