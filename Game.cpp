#include "Game.h"
#include "Defs.h"
#include "Structs.h"
#include "map.h"
#include "Graphics.h"
#include<iostream>
using namespace std;

Entity *Player;
Map *mp;
Gun *gun;
Sword *sword;
Character *character;

SDL_Renderer* Game::renderer = nullptr;
Game::Game(){
    Game::init();
}

Game::~Game(){}

void Game::LogErrorAndExit(const char *msg, const char *error)
{
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_CRITICAL, "%s: %s", msg, error);
}

void Game::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        LogErrorAndExit("SDL_INIT", SDL_GetError());
    }
    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        LogErrorAndExit("Create Window", SDL_GetError());
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
    if (renderer == nullptr)
    {
        LogErrorAndExit("Create Render", SDL_GetError());
    }
    SDL_RenderSetLogicalSize(renderer,SCREEN_HEIGHT,SCREEN_WIDTH);

    Player = new Entity();
    Player->loadTexture("img/simple.png");
    gun = new Gun("img/SuperGun.png", Player);
    sword = new Sword("img/sword.png", Player);
    character = new Character(Player,gun,sword);
    mp = new Map();

    //knight = new Knight();
    //knight->loadTexture("img/Knight/Attacks.png");
    //std::cout << " ???";
}

void Game::updateEvents()
{
    character->update();
}
void Game::handleEvents ()
{
    SDL_Event event;
//    while (SDL_PollEvent(&event))
//    {
//        switch(event.type)
//        {
//            case SDL_QUIT:
//                exit(0);
//                break;
//            case SDL_KEYDOWN:
//                switch (event.key.keysym.sym)
//                {
//                case SDLK_w:
//                    Player->dy -= Player->speedy;
//                    Player->dir = Up;
//                    break;
//                case SDLK_s:
//                    Player->dy += Player->speedy;
//                    Player->dir = Down;
//                    break;
//                case SDLK_a:
//                    Player->dir = Left;
//                    Player->dx -= Player->speedx;
//                   break;
//                case SDLK_d:
//                    Player->dx += Player->speedx;
//                    Player->dir = Right;
//                    break;
//                case SDLK_SPACE:
//                    character->gun->shot();
//                    break;
//                default:
//                    break;
//                }
//        }
//    }
    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                exit(0);
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_SPACE:
                        if (character->weaponState == GUN)
                        character->gun->shot();
                        else
                        {
                            character->sword->cut();
                        }
                        break;
                    case SDLK_1:
                        character->currentWeapon = character->sword;
                        character->weaponState = SWORD;
                        break;
                    case SDLK_2:
                        character->currentWeapon = character->gun;
                        character->weaponState = GUN;
                        break;
                    default:
                        break;
                }
                break;
        }

    }

    // Check for continuous key states (movement keys)
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_W])
    {
        Player->dy -= Player->speedy;
        Player->dir = Up;
    }
    if (keystate[SDL_SCANCODE_S])
    {
        Player->dy += Player->speedy;
        Player->dir = Down;
    }
    if (keystate[SDL_SCANCODE_A])
    {
        Player->dx -= Player->speedx;
        Player->dir = Left;
    }
    if (keystate[SDL_SCANCODE_D])
    {
        Player->dx += Player->speedx;
        Player->dir = Right;
    }
}


void Game::addTexture(SDL_Texture *texture, int x, int y)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(texture,NULL,NULL,&dest.w,&dest.h);
    SDL_RenderCopy(renderer,texture,NULL,&dest);
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer,0,255,0,255);
    SDL_RenderClear(renderer);
    mp->DrawMap();
//    Graphics::blitRect(Player->texture,Player->srcRect,Player->destRect);
//    Graphics::blitRect(gun->texture,gun->srcRect,gun->destRect);
    Graphics::drawCharacter(character);
    SDL_RenderPresent(renderer);
}


