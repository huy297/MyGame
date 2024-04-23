#ifndef MAP_H
#define MAP_H

#include "Game.h"
#include "Defs.h"
#include <string>
using namespace std;
class Map
{
public:
    Map();
    ~Map();
    void ProcessData(int i, string s, int layer);
    void LoadMap();
    void DrawMap();
    SDL_Rect *destRect;
    SDL_Texture *texture;
    SDL_Rect *srcRect;
public:
    static bool isWalkable[MAPSIZE][MAPSIZE];
    static bool isBlocked[MAPSIZE][MAPSIZE];
};

#endif // MAP_H
