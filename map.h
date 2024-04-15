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
    SDL_Rect *destRect;
    SDL_Texture *texture;
    SDL_Rect *srcRect;
    int MyMap[64][64];
};

#endif // MAP_H
