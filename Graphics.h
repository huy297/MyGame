#ifndef Graphics_H
#define Graphics_H

#include "Game.h"

class Graphics
{
public:
    static SDL_Texture* LoadTexture(const char* filename);
    static void blitRect(SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dest);
};
#endif // Graphics_H



