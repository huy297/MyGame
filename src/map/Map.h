#ifndef MAP_H
#define MAP_H

#include "core/Defs.h"
#include <algorithm>
#include <string>
#include <SDL.h>
#include <memory>

class Map
{
public:
    Map();
    void ProcessData(int row, std::string line, int layer);
    void LoadMap();
    std::unique_ptr<SDL_Rect> destRect;
    SDL_Texture *texture;
    std::unique_ptr<SDL_Rect> srcRect;

public:
    static bool isWalkable[MAPSIZE][MAPSIZE];
    static int isBlocked[MAPSIZE][MAPSIZE];
    static int numBlocked[MAPSIZE][MAPSIZE];

    // Rectangle corners are clamped to the map's actual extent first: a
    // query that reaches past the edge (e.g. from a shot/path check near
    // the map boundary) is answered against the on-map portion instead of
    // reading out of bounds.
    static int blockedSize(int x1, int y1, int x2, int y2)
    {
        x1 = std::clamp(x1, 0, MAPSIZE - 1);
        y1 = std::clamp(y1, 0, MAPSIZE - 1);
        x2 = std::clamp(x2, 0, MAPSIZE - 1);
        y2 = std::clamp(y2, 0, MAPSIZE - 1);
        int total = numBlocked[x2][y2];
        if (y1)
            total -= numBlocked[x2][y1 - 1];
        if (x1)
            total -= numBlocked[x1 - 1][y2];
        if (x1 && y1)
            total += numBlocked[x1 - 1][y1 - 1];
        return total;
    }
};

#endif // MAP_H
