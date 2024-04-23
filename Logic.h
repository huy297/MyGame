#ifndef LOGIC_H
#define LOGIC_H

#include "map.h"
#include "defs.h"
#include "iostream"
#include "math.h"
using namespace std;

class Logic
{
public:
    static bool canMove(int x, int y, int dir)
    {
//        x += SPRITE_X;
//        y += 2*SPRITE_Y+24;
        int deltaX = 0, deltaY = 0;
        switch (dir)
        {
        case Down:
            deltaX = 24;
            deltaY = 48;
            break;
        case Left:
            deltaY = 24;
            break;
        case Right:
            deltaX = 24;
            deltaY = 24;
            break;
        case Up:
            deltaX = 24;
        }
        int v = (x+deltaX)/SPRITE_X;
        int u = (y+deltaY)/SPRITE_Y;
        return Map::isWalkable[u][v];
    }
    static bool canGetThrough(int x, int y)
    {
        int v = (x)/SPRITE_X;
        int u = (y)/SPRITE_Y;
        return Map::isBlocked[u][v];
    }
};


#endif // LOGIC_H
