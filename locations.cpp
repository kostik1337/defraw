#include <iostream>
#include <SDL/SDL.h> //sys includes
#include <SDL/SDL_gfxPrimitives.h>
#include "score.h"//my includes
#include "objects.h"
#include "locations.h"
#include "main.h"

Location::Location(Game* p){
    parent = p;
}

Location::~Location(){
}

Menu::Menu(Game* p):Location(p){
    background = SDL_LoadBMP("background.bmp");
    playbtn = new Button(30,225,112,258);
    exitbtn = new Button(30,305,112,336);
}

Menu::~Menu(){
    SDL_FreeSurface(background);
    delete playbtn, exitbtn;
}

void Menu::draw(){
    SDL_BlitSurface(background, NULL, parent->screen, NULL);
}

void Menu::event(SDL_Event* e){
    switch(e->type){
    case SDL_MOUSEBUTTONUP:
        if(playbtn->check_hit(e->button.x, e->button.y)) parent->change_loc(game);
        if(exitbtn->check_hit(e->button.x, e->button.y)) exit(0);
        break;
    case SDL_KEYDOWN:
        if(e->key.keysym.sym==SDLK_RETURN) parent->change_loc(game);
        break;
    }
}

GameLoc::GameLoc(Game* p):Location(p){
    hero = new Hero(parent->screen);
    parent->score = new Score(parent->screen);
    enemies = new Enemies(hero,parent->score,parent->screen);
    acc = .3;
    for(int i=0; i<4;i++)keypress[i]=0;
}

GameLoc::~GameLoc(){
    delete hero, enemies;
}

void GameLoc::draw(){
    boxColor(parent->screen,0,0,WIDTH, HEIGHT, 0x000000FF);
    enemies->draw();
    if(enemies->game_over) parent->change_loc(end);
    hero->inc_vel(acc*(keypress[3]-keypress[2]),acc*(keypress[1]-keypress[0]));
    hero->draw();
    parent->score->print(2,2);
}

void GameLoc::event(SDL_Event* e){
    switch(e->type){
    case SDL_KEYUP:
        if(e->key.keysym.sym==SDLK_UP) keypress[0]=0;
        else if(e->key.keysym.sym==SDLK_DOWN) keypress[1]=0;
        else if(e->key.keysym.sym==SDLK_LEFT) keypress[2]=0;
        else if(e->key.keysym.sym==SDLK_RIGHT) keypress[3]=0;
        break;
    case SDL_KEYDOWN:
        if(e->key.keysym.sym==SDLK_UP) keypress[0]=1;
        else if(e->key.keysym.sym==SDLK_DOWN) keypress[1]=1;
        else if(e->key.keysym.sym==SDLK_LEFT) keypress[2]=1;
        else if(e->key.keysym.sym==SDLK_RIGHT) keypress[3]=1;
        else if(e->key.keysym.sym==SDLK_ESCAPE) parent->change_loc(end);
        break;
    }
}

EndLoc::EndLoc(Game* p):Location(p){
    background = SDL_LoadBMP("end.bmp");
    playbtn = new Button(30,231,133,266);
    exitbtn = new Button(30,305,112,336);
}

EndLoc::~EndLoc(){
    SDL_FreeSurface(background);
    delete playbtn, exitbtn, parent->score;
}

void EndLoc::draw(){
    SDL_BlitSurface(background, NULL, parent->screen, NULL);
    parent->score->print_only_score(181,131);
}

void EndLoc::event(SDL_Event* e){
    switch(e->type){
    case SDL_MOUSEBUTTONUP:
        if(playbtn->check_hit(e->button.x, e->button.y)) parent->change_loc(game);
        if(exitbtn->check_hit(e->button.x, e->button.y)) exit(0);
        break;
    case SDL_KEYDOWN:
        if(e->key.keysym.sym==SDLK_RETURN) parent->change_loc(game);
        break;
    }
}
