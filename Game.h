#ifndef Game_h
#define Game_h

#include "defs.h"
#include <SDL.h>
#include<SDL_Image.h>

class Game
{
public:
    Game();
    ~Game();
    void LogErrorAndExit(const char *msg, const char *error);
    void init();
    void handleEvents();
    void render();
    static void addTexture(SDL_Texture *texture, int x, int y);
    void updateEvents();
public:
    SDL_Window *window;
    static SDL_Renderer *renderer;
};

#endif // Game_h
