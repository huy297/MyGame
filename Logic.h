#ifndef LOGIC_H
#define LOGIC_H

#include "map.h"
#include "defs.h"
#include "iostream"
#include "math.h"
#include <bits/stdc++.h>
#include <SDL.h>
mt19937 ran(time(0));
using namespace std;

class Logic
{
public:
    static pair<int,int> coordToBlock(int x, int y)
    {

    }
    static bool intersect(SDL_Rect *a, SDL_Rect *b)
    {
        if (a->x == b->x && a->y == b->y && a->w == b->w && a->h == b->h) return false;
        int x = max(a->x,b->x);
        int y = max(a->y,b->y);
        int u = min(a->x+a->w,b->x+b->w);
        int v = min(a->y+b->h,b->y+b->h);
        return x < u && y < v;
    }
    static int Rand(int l, int r)
    {
        return l + ran()%(r-l+1);
    }
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
     //   cout << x << ' ' << y << ' ' << u << ' ' << v << " why still moving\n";
//        if (x == 840 && y == 840)
//        {
//            cout << Map::isWalkable[u][v] << " why \n";
//        }
        return Map::isWalkable[u][v];
    }
    static bool canGetThrough(int x, int y)
    {
        int v = (x)/SPRITE_X;
        int u = (y)/SPRITE_Y;
     //   cout << x << ' ' << y << ' ' << u << ' ' << v << ' ' << Map::isBlocked[u][v] << " ???\n";
        return Map::isBlocked[u][v] == 0;
    }
    static bool canGo(SDL_Rect *dest, list<SDL_Rect*> &MovingThings)
    {
        for (auto u : MovingThings)
        {
           // cout << u->player->destRect->x << ' ' << u->player->destRect->y << ' ' << player->destRect->x << ' ' << player->destRect->y << " dmm\n";
            if ((u->x == dest->x) && (u->y == dest->y) && (u->w == dest->w) && (u->h == dest->h)) return true;
            if (Logic::intersect(u,dest)) return false;
        }
        return true;
    }


};


#endif // LOGIC_H
