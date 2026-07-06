#include "graphics/Graphics.h"
#include "core/World.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

namespace
{
int offsetX, offsetY;
int lastOffsetX, lastOffsetY;
unordered_map<string, SDL_Texture *> s_textureCache;
} // namespace

SDL_Texture *Graphics::loadTexture(const char *filename)
{
    auto it = s_textureCache.find(filename);
    if (it != s_textureCache.end())
        return it->second;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
    SDL_Texture *texture = IMG_LoadTexture(Game::renderer, filename);
    s_textureCache[filename] = texture;
    return texture;
}

void Graphics::clearCache()
{
    for (auto &kv : s_textureCache)
        if (kv.second)
            SDL_DestroyTexture(kv.second);
    s_textureCache.clear();
}

void Graphics::blitRect(SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dest)
{
    dest->x -= offsetX;
    dest->y -= offsetY;
    SDL_RenderCopy(Game::renderer, texture, src, dest);
    dest->x += offsetX;
    dest->y += offsetY;
}

void Graphics::background(SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dest)
{
    SDL_RenderCopy(Game::renderer, texture, src, dest);
}

void Graphics::drawBullet(Bullet *bullet)
{
    Graphics::blitRect(bullet->texture, bullet->srcRect.get(), bullet->destRect.get());
}

void Graphics::drawWeapon(Weapon *weapon)
{
    Graphics::blitRect(weapon->texture, weapon->srcRect.get(), weapon->destRect.get());
}

void Graphics::drawDeath(Death *death)
{
    Graphics::blitRect(death->texture, death->srcRect.get(), death->destRect.get());
}

void Graphics::drawItem(Item *item)
{
    Graphics::blitRect(item->texture, item->srcRect.get(), item->destRect.get());
}

void Graphics::drawWeaponEffect(WeaponEffect *effect)
{
    Graphics::blitRect(effect->texture, effect->srcRect.get(), effect->destRect.get());
}

void Graphics::updateOffset(int newOffsetX, int newOffsetY)
{
    lastOffsetX = newOffsetX - offsetX;
    lastOffsetY = newOffsetY - offsetY;
    offsetX = newOffsetX;
    offsetY = newOffsetY;
}

void Graphics::drawCharacter(Character *character)
{
    Graphics::blitRect(character->player->texture, character->player->srcRect.get(), character->player->destRect.get());

    if (character->sword->isSlashed)
    {
        Graphics::drawWeaponEffect(character->sword->toSlash.get());
    }
    else if (character->grenade->isExploded)
    {
        Graphics::drawWeaponEffect(character->grenade->explosion.get());
    }
    else
    {
        Graphics::drawWeapon(character->currentWeapon);
    }
}

void Graphics::drawAll(const World &world)
{
    for (auto &bot : world.bots)
    {
        drawCharacter(bot.get());
    }

    for (auto &death : world.deaths)
    {
        drawDeath(death.get());
    }

    for (auto &item : world.items)
    {
        drawItem(item.get());
    }

    for (auto &bullet : world.bullets)
    {
        drawBullet(bullet.get());
    }
}

void Graphics::drawMap(Map *map)
{
    map->srcRect->x += lastOffsetX;
    map->srcRect->y += lastOffsetY;

    // The camera viewport (srcRect) has to stay within the map texture's
    // actual bounds. Nothing else constrains it, so walking far enough in
    // any direction (the player spawns close to the map's north edge, so
    // "too far north" is the easiest way to hit this) pushes srcRect
    // negative/past the far edge; SDL then samples clamped/undefined
    // texture data there, which looks like the map suddenly zooming or
    // losing its aspect ratio rather than just scrolling.
    int textureWidth, textureHeight;
    SDL_QueryTexture(map->texture, NULL, NULL, &textureWidth, &textureHeight);
    map->srcRect->x = std::clamp(map->srcRect->x, 0, textureWidth - map->srcRect->w);
    map->srcRect->y = std::clamp(map->srcRect->y, 0, textureHeight - map->srcRect->h);

    Graphics::background(map->texture, map->srcRect.get(), map->destRect.get());
}
