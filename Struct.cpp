#include "Structs.h"
#include "Graphics.h"

int DirectionY[] = {0,-1,1,0,0};
int DirectionX[] = {0,0,0,-1,1};

Entity::Entity(){
    dx = 0;
    dy = 0;
    speedx = 10;
    speedy = 10;

    srcRect = new SDL_Rect();

    srcRect->x = 0;
    srcRect->y = 0;
    srcRect->w = SPRITE_SIZE;
    srcRect->h = SPRITE_SIZE;


    destRect = new SDL_Rect();
    destRect->x = 100; // Vị trí x trên màn hình
    destRect->y = 100; // Vị trí y trên màn hình
    destRect->w = SPRITE_SIZE; // Width of displayed frame on screen
    destRect->h = SPRITE_SIZE; // Height of displayed frame on screen

    Space = false;

    dir = 0;
    currentDir = Down;
}

Entity::~Entity(){}

void Entity::loadTexture (const char* filename)
{
    texture = IMG_LoadTexture(Game::renderer,filename);
}

void Entity::update()
{
    if (Space)
    {
        destRect->x += DirectionX[currentDir]*50;
        destRect->y += DirectionY[currentDir]*50;
        Space = false;
    }
    destRect->x += dx;
    destRect->y += dy;
    dx = 0; dy = 0;
    if (dir == Up)
    {
        srcRect->x = 0;
        srcRect->y = 96;
        currentDir = dir;
    }
    if (dir == Down)
    {
        srcRect->x = 0;
        srcRect->y = 0;
        currentDir = dir;
    }
    if (dir == Left)
    {
        srcRect->x = 0;
        srcRect->y = 32;
        currentDir = dir;
    }
    if (dir == Right)
    {
        srcRect->x = 0;
        srcRect->y = 64;
        currentDir = dir;
    }
    dir = 0;
    srcRect->x += 32;
    srcRect->x %= 96;
}

