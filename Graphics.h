#ifndef Graphics_H
#define Graphics_H

#include "Game.h"
#include "structs.h"
#include "map.h"
#include <SDL_ttf.h>
#include <bits/stdc++.h>
#include<SDL_mixer.h>

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
    static void drawDeath(Death *dead);
    static void drawItem(Item *item);
    static void drawAll();


    static TTF_Font* loadFont(const char* filename, int fontSize) {
        TTF_Font* gFont = TTF_OpenFont(filename, fontSize);
        if (!gFont)
        {
            if (!gFont) {
                std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
            }
        }
        return gFont;
    }
    static SDL_Texture* loadText(const char *text, TTF_Font* font, SDL_Color color) {
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(Game::renderer, textSurface);
        SDL_FreeSurface(textSurface);
        return textTexture;
    }
    static void updateText(SDL_Texture*& textTexture, const char* newText, TTF_Font* font, SDL_Color textColor) {
        if (textTexture) {
            SDL_DestroyTexture(textTexture);
            textTexture = nullptr;
        }
        textTexture = loadText(newText, font, textColor);
    }
    static void drawText(SDL_Texture* texture, int x, int y, int w, int h) {
        SDL_Rect dstRect = {x, y, w, h};
        SDL_QueryTexture(texture, NULL, NULL, &dstRect.w, &dstRect.h);
        SDL_RenderCopy(Game::renderer, texture, NULL, &dstRect);
    }

    static Mix_Chunk* loadSound(const char* path) {
        Mix_Chunk* gChunk = Mix_LoadWAV(path);
        if (gChunk == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
               "Could not load sound! SDL_mixer Error: %s", Mix_GetError());
        }
        return gChunk;
    }

    static void play(Mix_Chunk* gChunk) {
        if (gChunk != nullptr) {
            Mix_PlayChannel( -1, gChunk, 0 );
        }
    }
};
#endif // Graphics_H



