#ifndef Graphics_H
#define Graphics_H

#include "Game.h"
#include "structs.h"
#include "map.h"

class Graphics
{
public:
    static SDL_Texture* LoadTexture(const char* filename);

    static void drawMap(Map *MAP);
    static void updateOffSet(int _OffSet_X, int _OffSet_Y);
    static void blitRect(SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dest);
    static void drawCharacter (Character *character);
    static void drawBullet(Bullet *bullet);
    static void drawWeapon(Weapon *weapon);
    static void drawWeaponEffect(WeaponEffect *eff);
    static void backGround(SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dest);
    static void drawAll();
};
#endif // Graphics_H



