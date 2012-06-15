#include <list>
#include "defines.h"

using namespace std;

class Object{ //basic object, like hero and enemy, but not button
protected:
    SDL_Surface *scr;
    float vx, vy;
public:
    float level,radius;
    float x,y;
    //virtual void process() = 0;
    virtual void draw() = 0;
};

class Button{ //invisible rectange button
protected:
    SDL_Rect rect;
public:
    Button(int,int,int,int);
    //~Button();
    bool check_hit(Sint16,Sint16);
};

class Hero:public Object{ //hero of the day
protected:
    float acc;
public:
    Hero(SDL_Surface*);
    void inc_vel(float,float);
    void inc_level(float enemy_lvl);
    void draw();
};

class Enemy:public Object{//bad-ass punk
public:
    Enemy(float,SDL_Surface*);
    void process();
    void draw();
    bool del();
};

class Enemies{//bad-ass punks factory
protected:
    Hero* hero;
    SDL_Surface* scr;
    list<Enemy*> l;
    int tbe_min, tbe_max; //minimal and maximal time between creating enemies (in ms)
    long int current_time; //time from creating object "Enemies" (actually, that is not true, we suppose that only SDL_Delay take any time)
    long int next_time; //time when next enemy will appear
public:
    bool game_over;
    Score* score;
    Enemies(Hero*,Score*,SDL_Surface*);
    ~Enemies();
    void draw();
};
