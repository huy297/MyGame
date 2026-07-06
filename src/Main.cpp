#include <SDL.h>
#include "core/Game.h"
#include "core/Defs.h"
#include "graphics/Graphics.h"
#include "map/Logic.h"

// Single definition of the RNG declared extern in Logic.h.
std::mt19937 ran(time(0));

const Uint32 FRAME_MS = 50;

// SDL_Delay(50) alone would make each loop's true period
// (work_time + 50ms) instead of a steady ~50ms/20fps, since it doesn't
// account for time already spent this frame in update/render. Sleep only
// the remainder of the frame budget instead.
void paceFrame(Uint32 frameStart)
{
    Uint32 elapsed = SDL_GetTicks() - frameStart;
    if (elapsed < FRAME_MS)
        SDL_Delay(FRAME_MS - elapsed);
}

GameState currentGameState = Menu;
int main(int argc, char *argv[])
{
    Game game;
    while (!game.quitRequested)
    {
        if (currentGameState == Playing)
        {
            while (!game.quitRequested)
            {
                Uint32 frameStart = SDL_GetTicks();
                game.handleEvents();
                game.updateEvents();
                game.render();
                if (game.changeState(currentGameState))
                    break;
                paceFrame(frameStart);
            }
        }
        else if (currentGameState == Menu)
        {
            while (!game.quitRequested)
            {
                Uint32 frameStart = SDL_GetTicks();
                game.renderMenu();
                game.handleMouse();
                if (game.changeState(currentGameState))
                {
                    break;
                }
                paceFrame(frameStart);
            }
        }
        else if (currentGameState == EndGame)
        {
            while (!game.quitRequested)
            {
                Uint32 frameStart = SDL_GetTicks();
                game.renderEndGame();
                game.handleMouse();
                if (game.changeState(currentGameState))
                {
                    if (!game.quitRequested)
                        game.reset();
                    break;
                }
                paceFrame(frameStart);
            }
        }
    }
    return 0;
}
