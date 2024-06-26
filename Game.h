#ifndef Game_h
#define Game_h

#include "defs.h"
#include "Structs.h"
#include<vector>
#include <SDL.h>
#include<SDL_Image.h>
using namespace std;
enum GameState
{
    Menu = 1, isPlaying = 2, endGame = 3
};
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
    void renderMenu();
    void handleMouse();
    void renderEndGame();
    bool changeState(GameState &currentState);
    void reset();
public:
    SDL_Window *window;
    static SDL_Renderer *renderer;

   // static vector<Bot*> bot;
};

#endif // Game_h
