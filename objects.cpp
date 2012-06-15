#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <stdlib.h>
#include <math.h>
#include <list>
#include "score.h"
#include "objects.h"
#include "defines.h"
#define PI 3.14159265

using namespace std;

/*Object::Object(){
}*/

Button::Button(int x1,int y1,int x2,int y2){
    rect.x = x1;
    rect.y = y1;
    rect.w = x2-x1;
    rect.h = y2-y1;
}

bool Button::check_hit(Sint16 x, Sint16 y){
    if(x>rect.x && y>rect.y && x<rect.x+rect.w && y<rect.y+rect.h) return true;
    else return false;
}

Hero::Hero(SDL_Surface* screen){
    x=WIDTH/2; y=HEIGHT/2;
    vx=vy=0.;
    level = 1;
    radius = RTOL*level;
    acc = 0.96;
    scr = screen;
}

void Hero::inc_vel(float xv,float yv){
    vx+=xv; vy+=yv;
}

void Hero::draw(){
    if(x>WIDTH+radius) x=-radius;
    if(x<-radius) x=WIDTH+radius;
    if(y>HEIGHT+radius) y=-radius;
    if(y<-radius) y=HEIGHT+radius;
    x+=vx; y+=vy;
    vx*=acc; vy*=acc;
    aacircleColor(scr, x, y, radius, 0xFF0000FF);
}

void Hero::inc_level(float enemy_lvl){
    level+=0.1*enemy_lvl/level;
    radius = RTOL*level;
}

Enemy::Enemy(float lv, SDL_Surface* screen){
    level = lv;
    radius = RTOL*level;
    scr = screen;
    int angle = rand()%360-45;
    if(-45<=angle && angle<45){ x=-radius; y=rand()%HEIGHT;}
    if(45<=angle && angle<135){ x=rand()%WIDTH; y=-radius;}
    if(135<=angle && angle<225){ x=WIDTH+radius; y=rand()%HEIGHT;}
    if(225<=angle && angle<315){ x=rand()%WIDTH; y=HEIGHT+radius;}
    float v = rand()%3+2;
    vx = v*cos(angle*PI/180);
    vy = v*sin(angle*PI/180);
}

void Enemy::process(){
    x+=vx; y+=vy;
}

void Enemy::draw(){
    aacircleColor(scr, x, y, radius, 0x0000FFFF);
}

bool Enemy::del(){
    if(x<-radius || x>WIDTH+radius || y<-radius || y>HEIGHT+radius) return true;
    else return false;
}

Enemies::Enemies(Hero* h,Score* s, SDL_Surface* screen){
    hero = h;
    scr = screen;
    score = s;
    tbe_min = 300; tbe_max = 500;
    current_time = 0;
    next_time = rand()%(tbe_max-tbe_min) + tbe_min;
    game_over = false;
}

Enemies::~Enemies(){
    for(list<Enemy*>::iterator i=l.begin(); i!=l.end();){
        delete (*i);
        i=l.erase(i);
    }
}

void Enemies::draw(){
    current_time+=DELAY;
    if(current_time>next_time){
        l.push_front(new Enemy((rand()%20)*0.1+hero->level-0.5,scr));
        next_time = current_time + rand()%(tbe_max-tbe_min) + tbe_min;
    }
    for(list<Enemy*>::iterator i=l.begin(); i!=l.end();){
        if((hero->x - (*i)->x)*(hero->x - (*i)->x) + (hero->y - (*i)->y)*(hero->y - (*i)->y) < (hero->radius + (*i)->radius)*(hero->radius + (*i)->radius))//if enemy hits hero
            if (hero->level >= (*i)->level){ hero->inc_level((*i)->level); score->add(hero->level,(*i)->level); delete (*i); i=l.erase(i); continue;}
            else {game_over = true; break;}
        else if((*i)->del()){
            delete (*i);
            i=l.erase(i);
        }
        else {
            (*i)->process();
            (*i)->draw();
            ++i;
        }
    }
}
