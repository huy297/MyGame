#include "Game.h"
#include "Defs.h"
//#include "Structs.h"
#include "map.h"
#include "Graphics.h"
#include<iostream>
#include <SDL_mixer.h>
using namespace std;


    SDL_Renderer* Game::renderer = nullptr;
    Character *character;
    int First_X,First_Y;
    int OffSet_X, OffSet_Y;
    Map *mp;
    list<Bot*> Bot::bot;
    list<Bullet*> Gun::bullet;
    list<Death*> Death::dead;
    list<Grenade*> Grenade::onGoing;
    list<Explosion*> Explosion::allExplosion;
    list<Item*> Item::allItem;
    int totalScore = 0;
    SDL_Texture *scoreTexture,*hpTexture,*magTexture,*greTexture, *nameOfGame, *playButton,*exitButton, *replayButton, *youlose;
    SDL_Texture *allMap;
    SDL_Rect *srcMap,*destMap;
    TTF_Font* dFont = NULL;
    SDL_Color blue = {0, 0, 255, 255};
    SDL_Color red = {255, 0, 0, 255};
    SDL_Color green = {0, 255, 0, 255};
    SDL_Color orange = {255, 255, 255, 255};

void setRect (SDL_Rect *rect, int x, int y, int w, int h) {
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}


Game::Game(){
    Game::init();
}

Game::~Game(){}

void Game::LogErrorAndExit(const char *msg, const char *error)
{
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_CRITICAL, "%s: %s", msg, error);
}
void LogErrorAndExit(const char* msg, const char* error) {
    printf("%s Error: %s\n", msg, error);
    SDL_Quit();
    exit(1);
}
void Game::init()
{


    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        LogErrorAndExit("SDL_INIT", SDL_GetError());
    }

    // Initialize SDL_mixe
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("TTF_Init failed: %s\n", TTF_GetError());
        SDL_Quit();
    }

    TTF_Quit();

    SDL_Quit();

    if (TTF_Init() == -1) {
        printf("TTF_Init failed: %s\n", TTF_GetError());
        SDL_Quit();
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


    // for only main player
    character->player->hp = 400;
    character->player->maxHp = 400;
    character->player->magazine = 80;
    character->player->maxMagazine = 80;
    character->player->numGren = 3;
    totalScore = 0;
    //
    string scoreText = "Score: " + to_string(totalScore);
    dFont = Graphics::loadFont("img/dak.ttf",32);
    scoreTexture = Graphics::loadText(scoreText.c_str(),dFont,red);
    //
    string hpText = "HP: " + to_string(character->player->hp) + "/400";
    hpTexture = Graphics::loadText(hpText.c_str(),dFont,orange);
    //
    string magText = "Bullets: " + to_string(character->player->magazine) + "/80";
    magTexture = Graphics::loadText(magText.c_str(),dFont,orange);
    //
    string greText = "Grenades: " + to_string(character->player->numGren) + "/3";
    greTexture = Graphics::loadText(greText.c_str(),dFont,orange);

    //
    allMap = Graphics::LoadTexture("mymap/minimap.png");
    srcMap = new SDL_Rect(); destMap = new SDL_Rect();
    setRect(srcMap,0,0,SCREEN_HEIGHT,SCREEN_HEIGHT); setRect(destMap,0,0,SCREEN_HEIGHT,SCREEN_WIDTH);
    //
    nameOfGame = Graphics::LoadTexture("img/survivalgames.png");
    //
    playButton = Graphics::LoadTexture("img/play.png");
    //
    exitButton = Graphics::LoadTexture("img/exit.png");
    //
    replayButton = Graphics::LoadTexture("img/replay.png");
    //
    youlose = Graphics::LoadTexture("img/endgame.png");
}

void Game::reset()
{
    First_X = character->player->x;
    First_Y = character->player->y;

    setRect(character->player->srcRect,0,0,SCREEN_HEIGHT/2,SCREEN_WIDTH/2);
    // for only main player
    character->player->hp = 400;
    character->player->maxHp = 400;
    character->player->magazine = 80;
    character->player->maxMagazine = 80;
    character->player->numGren = 3;
    totalScore = 0;

    Bot::bot.clear();
    Gun::bullet.clear();
    Death::dead.clear();
    Grenade::onGoing.clear();
    Explosion::allExplosion.clear();
    Item::allItem.clear();

}

void Game::updateEvents()
{
    character->update();
    Bot::genBot();
    int old = Bot::bot.size();
    Bot::updateAllBot(character);
    totalScore += (old-Bot::bot.size());
    //
    string scoreText = "Score: " + to_string(totalScore);
    Graphics::updateText(scoreTexture,scoreText.c_str(),dFont,red);
    //
    string hpText = "HP: " + to_string(character->player->hp) + "/400";
    Graphics::updateText(hpTexture,hpText.c_str(),dFont,orange);
    //
    string magText = "Bullets: " + to_string(character->player->magazine) + "/80";
    Graphics::updateText(magTexture,magText.c_str(),dFont,orange);
    //
    string greText = "Grenades: " + to_string(character->player->numGren) + "/3";
    Graphics::updateText(greTexture,greText.c_str(),dFont,orange);
    //
    Item::genItem();
    Item::updateAllItem();
    Gun::updateAllBullet();
    Death::updateAllDeath();
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
                            if (character->player->numGren > 0)
                            {
                                character->grenade->active();
                            }
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
                   case SDLK_3:
                        if (character->player->numGren == 0) break;
                        character->currentWeapon = character->grenade;
                        character->weaponState = GRENADE;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                    case SDLK_SPACE:
                        if (character->weaponState == GRENADE && character->player->numGren > 0)
                        {   character->player->numGren--;
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
    // for draw Text
    Graphics::drawText(scoreTexture,720,0,50,50);
    Graphics::drawText(hpTexture,0,0,50,50);
    Graphics::drawText(magTexture,0,50,50,50);
    Graphics::drawText(greTexture,0,100,50,50);
    //
    Graphics::drawCharacter(character);
    Graphics::drawAll();
    SDL_RenderPresent(renderer);
}
int mouseX,mouseY;
void Game::handleMouse()
{
    SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    exit(0);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    SDL_GetMouseState(&mouseX, &mouseY);
                    break;
                default:
                    break;
            }
        }
}
bool inRect (int x, int y, int u, int v, int w, int h)
{
    return x >= u && x <= (u+w) && y >= v && y <= (v+h);
}
bool Game::changeState (GameState &currentState)
{
    if (currentState == Menu)
    {
        if (inRect(mouseX,mouseY,300,400,912,55))
        {
            currentState = isPlaying;
            mouseX = mouseY = 0;
            return true;
        }
        else if (inRect(mouseX,mouseY,300,600,912,55))
        {
            exit(0);
        }
    }
    else if (currentState == isPlaying)
    {
        if (character->player->hp <= 0)
        {
            currentState = endGame;
            return true;
        }
    }
    else if (currentState == endGame)
    {
        if (inRect(mouseX,mouseY,300,500,912,55))
        {
            currentState = isPlaying;
            mouseX = mouseY = 0;
            return true;
        }
        else if (inRect(mouseX,mouseY,300,650,912,55))
        {
            exit(0);
        }
    }
    return false;
}
void Game::renderMenu()
{
    SDL_SetRenderDrawColor(renderer,0,255,0,255);
    SDL_RenderClear(renderer);
    Graphics::backGround(allMap,srcMap,destMap);
    Graphics::drawText(nameOfGame,140,200,200,200);
    Graphics::drawText(playButton,300,400,60,100);
    Graphics::drawText(exitButton,300,600,60,100);
    SDL_RenderPresent(renderer);
}

void Game::renderEndGame()
{
    SDL_SetRenderDrawColor(renderer,0,255,0,255);
    SDL_RenderClear(renderer);
    Graphics::drawMap(mp);
    Graphics::drawText(youlose,140,0,200,200);
    Graphics::drawText(replayButton,300,500,60,100);
    Graphics::drawText(exitButton,300,650,60,100);
    SDL_RenderPresent(renderer);
}

