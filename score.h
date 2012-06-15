class Score{
protected:
    unsigned int score;
    SDL_Surface *scr;
public:
    Score(SDL_Surface*);
    void add(float,float);
    void print(int,int);
    void print_only_score(int x, int y);
};
