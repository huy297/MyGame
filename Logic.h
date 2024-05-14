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
    static long long currentTime()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
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
    static int canShoot (SDL_Rect *a, SDL_Rect *b)
    {
        if (abs(a->x-b->x)+abs(a->y-b->y) <= 100) return 0;
        int x = (a->x+47) / 48;
        int y = a->y / 48;
        int u = (b->x+47) / 48;
        int v = b->y / 48;
        if (abs(a->x-b->x) <= 20)
        {
            if (a->y >= b->y)
            {
                if (Map::blockedSize(u,v,x,y)) return 0;
                return Up;
            }
            else
            {
                if (Map::blockedSize(x,y,u,v)) return 0;
                return Down;
            }
        }
        if (abs(a->y-b->y) <= 20)
        {
            if (a->x <= b->x)
            {
                cout << x << ' ' << y << ' ' << u << ' ' << v <<  ' ' << Map::blockedSize(x,y,u,v) << " ??\n";
                if (Map::blockedSize(x,y,u,v)) return 0;
                return Right;
            }
            else
            {
                if (Map::blockedSize(u,v,x,y)) return 0;
                return Left;
            }
        }
        return 0;
    }
    static int canSlash (SDL_Rect *a, SDL_Rect *b)
    {
        if (abs(a->x-b->x) <= 20)
        {
            if (a->y >= b->y && a->y-b->y<=60) return Up;
            else if (a->y <= b->y && a->y-b->y>=-60) return Down;
        }
        if (abs(a->y-b->y) <= 20)
        {
            if (a->x >= b->x && a->x-b->x<=60) return Left;
            else if (a->x <= b->x && a->x-b->x>=-60) return Right;
        }
        return 0;
    }
};


#endif // LOGIC_H
