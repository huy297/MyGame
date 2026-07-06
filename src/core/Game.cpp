#include "core/Game.h"
#include "core/Defs.h"
#include "map/Map.h"
#include "core/World.h"
#include "graphics/Graphics.h"
#include <iostream>
#include <memory>
#include <SDL_mixer.h>
using namespace std;

SDL_Renderer *Game::renderer = nullptr;

namespace
{
unique_ptr<World> world;
unique_ptr<Map> gameMap;
int startX, startY;
int offsetX, offsetY;
int totalScore = 0;
// Last HUD values actually rendered into their textures. updateEvents()
// only regenerates a stat's texture (TTF render + GPU upload) when its
// value changed since last frame, instead of doing it unconditionally
// every frame. -1 is not a reachable stat value, so it forces a refresh
// on the first frame after init()/reset().
int lastScore = -1, lastHp = -1, lastMagazine = -1, lastNumGren = -1;
SDL_Texture *scoreTexture, *hpTexture, *ammoTexture, *grenadeTexture, *titleTexture, *playButton, *exitButton, *replayButton, *loseTexture;
SDL_Texture *miniMapTexture;
unique_ptr<SDL_Rect> miniMapSrcRect, miniMapDestRect;
TTF_Font *font = nullptr;
SDL_Color red = {255, 0, 0, 255};
SDL_Color orange = {255, 255, 255, 255};
int mouseX, mouseY;

void LogErrorAndExit(const char *msg, const char *error)
{
    printf("%s Error: %s\n", msg, error);
    SDL_Quit();
    exit(1);
}

bool inRect(int x, int y, int rectX, int rectY, int w, int h)
{
    return x >= rectX && x <= (rectX + w) && y >= rectY && y <= (rectY + h);
}
} // namespace

Game::Game()
{
    Game::init();
}

Game::~Game()
{
    // Reverse of acquisition order in init(): game session state first,
    // then rendering resources, then the SDL subsystems themselves.
    world.reset();
    gameMap.reset();

    if (scoreTexture)
        SDL_DestroyTexture(scoreTexture);
    if (hpTexture)
        SDL_DestroyTexture(hpTexture);
    if (ammoTexture)
        SDL_DestroyTexture(ammoTexture);
    if (grenadeTexture)
        SDL_DestroyTexture(grenadeTexture);

    Graphics::clearCache();

    if (font)
        TTF_CloseFont(font);

    miniMapSrcRect.reset();
    miniMapDestRect.reset();

    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);

    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        LogErrorAndExit("SDL_INIT", SDL_GetError());
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        LogErrorAndExit("SDL_mixer could not initialize!", Mix_GetError());
    }
    if (TTF_Init() == -1)
    {
        LogErrorAndExit("TTF_Init failed", TTF_GetError());
    }
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        LogErrorAndExit("IMG_Init failed", IMG_GetError());
    }
    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        LogErrorAndExit("Create Window", SDL_GetError());
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        LogErrorAndExit("Create Render", SDL_GetError());
    }
    SDL_RenderSetLogicalSize(renderer, SCREEN_HEIGHT, SCREEN_WIDTH);

    world = std::make_unique<World>();
    gameMap = std::make_unique<Map>();

    startX = world->player.player->x;
    startY = world->player.player->y;

    // main player has more resources than bots
    world->player.player->hp = 400;
    world->player.player->maxHp = 400;
    world->player.player->magazine = 80;
    world->player.player->maxMagazine = 80;
    world->player.player->numGren = 3;
    totalScore = 0;

    string scoreText = "Score: " + to_string(totalScore);
    font = Graphics::loadFont("Img/dak.ttf", 32);
    scoreTexture = Graphics::loadText(scoreText.c_str(), font, red);

    string hpText = "HP: " + to_string(world->player.player->hp) + "/400";
    hpTexture = Graphics::loadText(hpText.c_str(), font, orange);

    string ammoText = "Bullets: " + to_string(world->player.player->magazine) + "/80";
    ammoTexture = Graphics::loadText(ammoText.c_str(), font, orange);

    string grenadeText = "Grenades: " + to_string(world->player.player->numGren) + "/3";
    grenadeTexture = Graphics::loadText(grenadeText.c_str(), font, orange);

    miniMapTexture = Graphics::loadTexture("MyMap/MiniMap.png");
    miniMapSrcRect = std::make_unique<SDL_Rect>();
    miniMapDestRect = std::make_unique<SDL_Rect>();
    MakeRect(miniMapSrcRect.get(), 0, 0, SCREEN_HEIGHT, SCREEN_HEIGHT);
    MakeRect(miniMapDestRect.get(), 0, 0, SCREEN_HEIGHT, SCREEN_WIDTH);

    titleTexture = Graphics::loadTexture("Img/survivalgames.png");
    playButton = Graphics::loadTexture("Img/Play.png");
    exitButton = Graphics::loadTexture("Img/Exit.png");
    replayButton = Graphics::loadTexture("Img/replay.png");
    loseTexture = Graphics::loadTexture("Img/endgame.png");
}

void Game::reset()
{
    world = std::make_unique<World>();

    world->player.player->hp = 400;
    world->player.player->maxHp = 400;
    world->player.player->magazine = 80;
    world->player.player->maxMagazine = 80;
    world->player.player->numGren = 3;
    totalScore = 0;
    lastScore = lastHp = lastMagazine = lastNumGren = -1;

    startX = world->player.player->x;
    startY = world->player.player->y;
}

void Game::updateEvents()
{
    world->update();
    totalScore += world->takeScoreDelta();

    int hp = world->player.player->hp;
    int magazine = world->player.player->magazine;
    int numGren = world->player.player->numGren;

    if (totalScore != lastScore)
    {
        string scoreText = "Score: " + to_string(totalScore);
        Graphics::updateText(scoreTexture, scoreText.c_str(), font, red);
        lastScore = totalScore;
    }
    if (hp != lastHp)
    {
        string hpText = "HP: " + to_string(hp) + "/400";
        Graphics::updateText(hpTexture, hpText.c_str(), font, orange);
        lastHp = hp;
    }
    if (magazine != lastMagazine)
    {
        string ammoText = "Bullets: " + to_string(magazine) + "/80";
        Graphics::updateText(ammoTexture, ammoText.c_str(), font, orange);
        lastMagazine = magazine;
    }
    if (numGren != lastNumGren)
    {
        string grenadeText = "Grenades: " + to_string(numGren) + "/3";
        Graphics::updateText(grenadeTexture, grenadeText.c_str(), font, orange);
        lastNumGren = numGren;
    }

    offsetX = world->player.player->x - startX;
    offsetY = world->player.player->y - startY;
}
void Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            quitRequested = true;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_SPACE:
                if (world->player.weaponState == GUN)
                    world->player.gun->shot(*world);
                else if (world->player.weaponState == SWORD)
                {
                    world->player.sword->cut(*world);
                }
                else if (world->player.weaponState == GRENADE)
                {
                    if (world->player.player->numGren > 0)
                    {
                        world->player.grenade->active();
                    }
                }
                break;
            case SDLK_1:
                world->player.currentWeapon = world->player.sword.get();
                world->player.weaponState = SWORD;
                break;
            case SDLK_2:
                world->player.currentWeapon = world->player.gun.get();
                world->player.weaponState = GUN;
                break;
            case SDLK_3:
                if (world->player.player->numGren == 0)
                    break;
                world->player.currentWeapon = world->player.grenade.get();
                world->player.weaponState = GRENADE;
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_SPACE:
                if (world->player.weaponState == GRENADE && world->player.player->numGren > 0)
                {
                    world->player.player->numGren--;
                    world->player.grenade->release();
                }
                break;
            default:
                break;
            }
        }
    }

    // Check for continuous key states (movement keys)
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_W])
    {
        world->player.player->dy -= world->player.player->speedy;
        world->player.player->dir = Up;
    }
    if (keystate[SDL_SCANCODE_S])
    {
        world->player.player->dy += world->player.player->speedy;
        world->player.player->dir = Down;
    }
    if (keystate[SDL_SCANCODE_A])
    {
        world->player.player->dx -= world->player.player->speedx;
        world->player.player->dir = Left;
    }
    if (keystate[SDL_SCANCODE_D])
    {
        world->player.player->dx += world->player.player->speedx;
        world->player.player->dir = Right;
    }
}

void Game::addTexture(SDL_Texture *texture, int x, int y)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderClear(renderer);
    Graphics::updateOffset(offsetX, offsetY);
    Graphics::drawMap(gameMap.get());

    Graphics::drawText(scoreTexture, 620, 0, 50, 50);
    Graphics::drawText(hpTexture, 0, 0, 50, 50);
    Graphics::drawText(ammoTexture, 0, 50, 50, 50);
    Graphics::drawText(grenadeTexture, 0, 100, 50, 50);

    Graphics::drawCharacter(&world->player);
    Graphics::drawAll(*world);
    SDL_RenderPresent(renderer);
}

void Game::handleMouse()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            quitRequested = true;
            break;
        case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState(&mouseX, &mouseY);
            break;
        default:
            break;
        }
    }
}

bool Game::changeState(GameState &currentState)
{
    if (currentState == Menu)
    {
        if (inRect(mouseX, mouseY, 250, 400, 912, 55))
        {
            currentState = Playing;
            mouseX = mouseY = 0;
            return true;
        }
        else if (inRect(mouseX, mouseY, 250, 600, 912, 55))
        {
            quitRequested = true;
            return true;
        }
    }
    else if (currentState == Playing)
    {
        if (world->player.player->hp <= 0)
        {
            currentState = EndGame;
            return true;
        }
    }
    else if (currentState == EndGame)
    {
        if (inRect(mouseX, mouseY, 250, 500, 912, 55))
        {
            currentState = Playing;
            mouseX = mouseY = 0;
            return true;
        }
        else if (inRect(mouseX, mouseY, 250, 650, 912, 55))
        {
            quitRequested = true;
            return true;
        }
    }
    return false;
}
void Game::renderMenu()
{
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderClear(renderer);
    Graphics::background(miniMapTexture, miniMapSrcRect.get(), miniMapDestRect.get());
    Graphics::drawText(titleTexture, 100, 200, 200, 200);
    Graphics::drawText(playButton, 250, 400, 60, 100);
    Graphics::drawText(exitButton, 250, 600, 60, 100);
    SDL_RenderPresent(renderer);
}

void Game::renderEndGame()
{
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderClear(renderer);
    // updateEvents() (and with it Graphics::updateOffset()) stops running
    // once the player dies, so without this the camera delta from the
    // instant of death keeps getting reapplied by drawMap() every frame,
    // scrolling the background forever on the death screen. Re-feeding the
    // same (now frozen) offsetX/offsetY makes the delta zero and keeps it
    // there, matching what render() does every frame during Playing.
    Graphics::updateOffset(offsetX, offsetY);
    Graphics::drawMap(gameMap.get());
    Graphics::drawText(loseTexture, 90, 0, 200, 200);
    Graphics::drawText(replayButton, 250, 500, 60, 100);
    Graphics::drawText(exitButton, 250, 650, 60, 100);
    SDL_RenderPresent(renderer);
}
