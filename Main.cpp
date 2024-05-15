#include <SDL.h>
#include "Game.h"
#include "Defs.h"
#include "Graphics.h"
#include "Structs.h"

Game *game = nullptr;

GameState currentGameState = Menu;
int main(int argc, char* argv[])
{
    game = new Game();
    while (true)
    {
        if (currentGameState == isPlaying)
        {
            while (true)
            {
              game->handleEvents();
              game->updateEvents();
                game->render();
                if (game->changeState(currentGameState)) break;
                SDL_Delay(50);
            }
        }
        else if (currentGameState == Menu)
        {
            while (true)
            {
                game->renderMenu();
                game->handleMouse();
                if (game->changeState(currentGameState))
                {
                    break;
                }
                SDL_Delay(50);
            }
        }
        else if (currentGameState == endGame)
        {
            while (true)
            {
                game->renderEndGame();
                game->handleMouse();
                if (game->changeState(currentGameState))
                {
                    game->reset();
                    break;
                }
                SDL_Delay(50);
            }
        }
    }
   return 0;
}
