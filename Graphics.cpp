#include "Graphics.h"
#include<bits/stdc++.h>
using namespace std;


SDL_Texture* Graphics::LoadTexture(const char* filename)
{
    SDL_Texture *texture;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,"Loading %s", filename);
    texture = IMG_LoadTexture(Game::renderer,filename);
    return texture;
}


void Graphics::blitRect(SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dest)
{
	SDL_RenderCopy(Game::renderer, texture, src, dest);
}

void Graphics::drawBullet(Bullet *bullet)
{
    Graphics::blitRect(bullet->texture,bullet->srcRect,bullet->destRect);
}

void Graphics::drawWeapon(Weapon *weapon)
{

    Graphics::blitRect(weapon->texture,weapon->srcRect,weapon->destRect);
}

void Graphics::drawSwordCut(Sword *sword)
{
   // cerr << sword->toSlash->srcRect->x << ' ' << sword->toSlash->srcRect->y << endl;
    Graphics::blitRect(sword->toSlash->texture,sword->toSlash->srcRect,sword->toSlash->destRect);
}

void Graphics::drawCharacter(Character *character) {
    Graphics::blitRect(character->player->texture, character->player->srcRect, character->player->destRect);
  //  Graphics::blitRect(character->gun->texture, character->gun->srcRect, character->gun->destRect);

    if (character->sword->isSlashed)
    {
        Graphics::drawSwordCut(character->sword);
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
