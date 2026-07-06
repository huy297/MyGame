#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "core/Game.h"
#include "entities/Character.h"
#include "entities/Weapons.h"
#include "entities/Items.h"
#include "map/Map.h"
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <string>

class World;

class Graphics
{
public:
    static SDL_Texture *loadTexture(const char *filename);
    static void drawMap(Map *map);
    static void updateOffset(int newOffsetX, int newOffsetY);
    static void blitRect(SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dest);
    static void drawCharacter(Character *character);
    static void drawBullet(Bullet *bullet);
    static void drawWeapon(Weapon *weapon);
    static void drawWeaponEffect(WeaponEffect *effect);
    static void background(SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dest);
    static void drawDeath(Death *death);
    static void drawItem(Item *item);
    static void drawAll(const World &world);
    static void clearCache();

    static TTF_Font *loadFont(const char *filename, int fontSize)
    {
        TTF_Font *font = TTF_OpenFont(filename, fontSize);
        if (!font)
        {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        }
        return font;
    }
    static SDL_Texture *loadText(const char *text, TTF_Font *font, SDL_Color color)
    {
        SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, color);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(Game::renderer, textSurface);
        SDL_FreeSurface(textSurface);
        return textTexture;
    }
    static void updateText(SDL_Texture *&textTexture, const char *newText, TTF_Font *font, SDL_Color textColor)
    {
        if (textTexture)
        {
            SDL_DestroyTexture(textTexture);
            textTexture = nullptr;
        }
        textTexture = loadText(newText, font, textColor);
    }
    static void drawText(SDL_Texture *texture, int x, int y, int w, int h)
    {
        SDL_Rect dstRect = {x, y, w, h};
        SDL_QueryTexture(texture, NULL, NULL, &dstRect.w, &dstRect.h);
        SDL_RenderCopy(Game::renderer, texture, NULL, &dstRect);
    }

    static Mix_Chunk *loadSound(const char *path)
    {
        Mix_Chunk *chunk = Mix_LoadWAV(path);
        if (chunk == nullptr)
        {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
                           "Could not load sound! SDL_mixer Error: %s", Mix_GetError());
        }
        return chunk;
    }

    static void play(Mix_Chunk *chunk)
    {
        if (chunk != nullptr)
        {
            Mix_PlayChannel(-1, chunk, 0);
        }
    }
};
#endif // GRAPHICS_H
