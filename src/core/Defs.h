#ifndef DEFS_H
#define DEFS_H

#define SCREEN_WIDTH 760
#define SCREEN_HEIGHT 760
#define WINDOW_TITLE "MyGame"
#define SPRITE_X 48
#define SPRITE_Y 48
#define GUN_X 32
#define GUN_Y 16
#define BLOCK 16

// Movement direction bitmask; diagonals are the bitwise OR of two directions.
#define Up 1
#define Down 2
#define Left 4
#define Right 8
#define RightAndUp 9
#define RightAndDown 10
#define LeftAndUp 5
#define LeftAndDown 6

#define MAPSIZE 80

// Movement/hit collision checks are O(live bot count) per bot per frame, so
// an unbounded bot count makes each frame progressively slower the longer a
// playthrough runs. Capping concurrent bots keeps per-frame cost bounded;
// killing bots frees up room for new spawns as before.
#define MAX_BOTS 60

#include <SDL.h>
inline void MakeRect(SDL_Rect *r, int x, int y, int w, int h)
{
    r->x = x;
    r->y = y;
    r->w = w;
    r->h = h;
}

#endif // DEFS_H
