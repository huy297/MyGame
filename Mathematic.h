#ifndef MATH_H
#define MATH_H
#include "defs.h"
#include <SDL.h>
#include <bits/stdc++.h>
mt19937 ran(time(0));
class Math
{
public:
    static pair<int,int> coordToBlock(int x, int y)
    {

    }
    static bool intersect(SDL_Rect *a, SDL_Rect *b)
    {
        int x = max(a->x,b->x);
        int y = max(a->y,b->y);
        int u = min(a->x+a->w,b->x+b->w);
        int v = min(a->y+b->h,b->y+b->h);
        return x < u && y < v;
    }
    static Rand(int l, int r)
    {
        return l + ran()%(r-l+1);
    }
};


#endif // MATH_H

