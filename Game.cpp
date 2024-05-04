#include "Game.h"
#include "Defs.h"
//#include "Structs.h"
#include "map.h"
#include "Graphics.h"
#include<iostream>
using namespace std;


Character *character;
int First_X,First_Y;
int OffSet_X, OffSet_Y;
Map *mp;
SDL_Renderer* Game::renderer = nullptr;
vector<Bot*> Bot::bot;
vector<Bullet*> Gun::bullet;
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

    character = new Character();
    mp = new Map();

    First_X = character->player->x;
    First_Y = character->player->y;

    //knight = new Knight();
    //knight->loadTexture("img/Knight/Attacks.png");
    //std::cout << " ???";

    Bot *dak = new Bot();
    Bot::bot.push_back(dak);
}

void Game::updateEvents()
{
    character->update();
    for (auto u : Bot::bot) u->update();
    OffSet_X = character->player->x - First_X;
    OffSet_Y = character->player->y - First_Y;
}
void Game::handleEvents ()
{
    SDL_Event event;
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
                        else if (character->weaponState == SWORD)
                        {
                            character->sword->cut();
                        }
                        else if (character->weaponState == GRENADE)
                        {
                            character->grenade->active();
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
                   case SDLK_4:
                        character->currentWeapon = character->grenade;
                        character->weaponState = GRENADE;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                    case SDLK_SPACE:
                        if (character->weaponState == GRENADE)
                        {
                            character->grenade->release();
                        }
                   //     character->currentWeapon = character->sword;
                     //   character->weaponState = SWORD;
                    default:
                        break;
                }
        }

    }

    // Check for continuous key states (movement keys)
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_W])
    {
        character->player->dy -= character->player->speedy;
        character->player->dir = Up;
    }
    if (keystate[SDL_SCANCODE_S])
    {
        character->player->dy += character->player->speedy;
        character->player->dir = Down;
    }
    if (keystate[SDL_SCANCODE_A])
    {
        character->player->dx -= character->player->speedx;
        character->player->dir = Left;
    }
    if (keystate[SDL_SCANCODE_D])
    {
        character->player->dx += character->player->speedx;
        character->player->dir = Right;
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
    Graphics::updateOffSet(OffSet_X,OffSet_Y);
    Graphics::drawMap(mp);
    Graphics::drawCharacter(character);
    Graphics::drawAll();
    SDL_RenderPresent(renderer);
}

