#ifndef MAP_H
#define MAP_H

#include "Game.h"
#include "Defs.h"
#include <string>
#include<SDL.h>
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
    static int isBlocked[MAPSIZE][MAPSIZE];
    static int numBlocked[MAPSIZE][MAPSIZE];
    static int blockedSize (int x, int y, int u, int v)
    {
        int res = numBlocked[u][v];
        if (y) res -= numBlocked[u][y-1];
        if (x) res -= numBlocked[x-1][v];
        if (x && y) res += numBlocked[x-1][y-1];
        return res;
    }
};

#endif // MAP_H
