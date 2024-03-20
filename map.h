#ifndef MAP_H
#define MAP_H

#include "Game.h"

class Map
{
public:
    Map();
    ~Map();

    void LoadMap();
    void DrawMap();
    SDL_Rect src,dest;
    SDL_Texture *ground;
    SDL_Texture *grass;
    SDL_Texture *tree;
    int MyMap[64][64];
};

#endif // MAP_H
