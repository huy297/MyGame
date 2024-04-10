#ifndef Graphics_H
#define Graphics_H

#include "Game.h"
#include "structs.h"

class Graphics
{
public:
    static SDL_Texture* LoadTexture(const char* filename);
    static void blitRect(SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dest);
    static void drawCharacter (Character *character);
    static void drawBullet(Bullet *bullet);
    static void drawWeapon(Weapon *weapon);
    static void drawSwordCut(Sword *sword);
};
#endif // Graphics_H



