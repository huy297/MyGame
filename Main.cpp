#include <SDL.h>
#include "Game.h"
#include "Defs.h"
#include "Graphics.h"
#include "Structs.h"

Game *game = nullptr;

int main(int argc, char* argv[])
{
    game = new Game();
    while (true)
    {
      game->handleEvents();
      game->updateEvents();
        game->render();
        SDL_Delay(100);
    }
   return 0;
}
