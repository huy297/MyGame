#include "Graphics.h"
#include<bits/stdc++.h>
using namespace std;

int __OffSet_X; int __OffSet_Y;
int lastOffSet_X, lastOffSet_Y;

SDL_Texture* Graphics::LoadTexture(const char* filename)
{
    SDL_Texture *texture;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,"Loading %s", filename);
    texture = IMG_LoadTexture(Game::renderer,filename);
    return texture;
}


void Graphics::blitRect(SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dest)
{
   // cout << dest->x << ' ' << dest->y << " current is " << endl;
    dest->x -= __OffSet_X;
    dest->y -= __OffSet_Y;
	SDL_RenderCopy(Game::renderer, texture, src, dest);
    dest->x += __OffSet_X;
    dest->y += __OffSet_Y;
}

void Graphics::backGround(SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dest)
{
   // cout << dest->x << ' ' << dest->y << " current is " << endl;
    //dest->x -= __OffSet_X;
    //dest->y -= __OffSet_Y;
	SDL_RenderCopy(Game::renderer, texture, src, dest);
    //dest->x += __OffSet_X;
    //dest->y += __OffSet_Y;
}

void Graphics::drawBullet(Bullet *bullet)
{
    Graphics::blitRect(bullet->texture,bullet->srcRect,bullet->destRect);
}

void Graphics::drawWeapon(Weapon *weapon)
{

    Graphics::blitRect(weapon->texture,weapon->srcRect,weapon->destRect);
}

void Graphics::drawWeaponEffect(WeaponEffect *eff)
{
   // cerr << sword->toSlash->srcRect->x << ' ' << sword->toSlash->srcRect->y << endl;
    Graphics::blitRect(eff->texture,eff->srcRect,eff->destRect);
}
void Graphics::updateOffSet(int OffSet_X, int OffSet_Y)
{
    lastOffSet_X = OffSet_X - __OffSet_X;
    lastOffSet_Y = OffSet_Y  - __OffSet_Y;
    __OffSet_X = OffSet_X; __OffSet_Y = OffSet_Y;
  //  cout << "OffSet " << OffSet_X << ' ' << OffSet_Y << endl;
}
void Graphics::drawCharacter(Character *character) {

    Graphics::blitRect(character->player->texture, character->player->srcRect, character->player->destRect);

  //  Graphics::blitRect(character->gun->texture, character->gun->srcRect, character->gun->destRect);

    if (character->sword->isSlashed)
    {
        Graphics::drawWeaponEffect(character->sword->toSlash);
    }
    else  if (character->grenade->isExplosed)
    {
        Graphics::drawWeaponEffect(character->grenade->explosion);
    }
    else
    {
      //  cout << " not Slashed " << endl;
        Graphics::drawWeapon(character->currentWeapon);
    }

     for (auto currentBullet : character->gun->bullet)
    {
       // cerr << currentBullet->srcRect->x << ' ' << currentBullet->srcRect->y << endl;
        Graphics::drawBullet(currentBullet);
    }
}


void Graphics::drawAll()
{
    //cout << "being called" << endl;
    for (Bot* x : Bot::bot)
    {
        drawCharacter(x);
    }

}

void Graphics::drawMap(Map *MAP)
{

    MAP->srcRect->x += lastOffSet_X;
    MAP->srcRect->y += lastOffSet_Y;
   // cout << MAP->srcRect->x << ' ' << MAP->srcRect->y << "MAP position" << endl;
    Graphics::backGround(MAP->texture,MAP->srcRect,MAP->destRect);

}
