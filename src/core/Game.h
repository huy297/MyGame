#ifndef GAME_H
#define GAME_H

#include "core/Defs.h"
#include <SDL.h>
#include <SDL_image.h>

enum GameState
{
    Menu = 1,
    Playing = 2,
    EndGame = 3
};
class Game
{
public:
    Game();
    ~Game();
    void init();
    void handleEvents();
    void render();
    static void addTexture(SDL_Texture *texture, int x, int y);
    void updateEvents();
    void renderMenu();
    void handleMouse();
    void renderEndGame();
    bool changeState(GameState &currentState);
    void reset();

public:
    SDL_Window *window;
    static SDL_Renderer *renderer;
    // Set instead of calling exit() directly, so the destructor (textures,
    // font, renderer/window, SDL subsystems) actually runs before the
    // process ends. Main.cpp's loops check this and unwind to main().
    bool quitRequested = false;
};

#endif // GAME_H
