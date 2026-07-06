#include "entities/Entity.h"
#include "core/World.h"
#include "core/Defs.h"
#include "graphics/Graphics.h"
#include "map/Logic.h"
#include "map/Map.h"

Entity::Entity()
{
    dx = 0;
    dy = 0;
    speedx = 10;
    speedy = 10;
    x = SCREEN_HEIGHT / 2 + 50;
    y = SCREEN_WIDTH / 2 + 50;
    hp = 100;
    magazine = 40;
    numGren = 2;

    maxHp = 200;
    maxMagazine = 80;
    maxGren = 3;

    srcRect = std::make_unique<SDL_Rect>();
    srcRect->x = 0;
    srcRect->y = 0;
    srcRect->w = SPRITE_X;
    srcRect->h = SPRITE_Y;

    destRect = std::make_unique<SDL_Rect>();
    destRect->x = x;
    destRect->y = y;
    destRect->w = SPRITE_X;
    destRect->h = SPRITE_Y;

    dir = 0;
    currentDir = Up;
}

void Entity::loadTexture(const char *filename)
{
    texture = Graphics::loadTexture(filename);
}

bool Entity::checkValidMove(World &world)
{
    if (Logic::canMove(destRect->x + dx, destRect->y + dy, this->dir) == 0)
    {
        dx = 0;
        dy = 0;
        return false;
    }
    destRect->x += dx;
    destRect->y += dy;
    if (world.isPathClear(destRect.get()) == false)
    {
        destRect->x -= dx;
        destRect->y -= dy;
        return false;
    }
    destRect->x -= dx;
    destRect->y -= dy;
    return true;
}
void Entity::update(World &world)
{
    bool isMove = dir;
    Map::isBlocked[int(x) / SPRITE_X][int(y) / SPRITE_Y]--;
    if (Entity::checkValidMove(world) == true)
    {
        destRect->x += dx;
        destRect->y += dy;
    }

    dx = 0;
    dy = 0;
    if (dir == Up)
    {
        srcRect->y = 0;
    }
    if (dir == Right)
    {
        srcRect->y = SPRITE_Y * 1;
    }
    if (dir == Down)
    {
        srcRect->y = SPRITE_Y * 2;
    }
    if (dir == Left)
    {
        srcRect->y = SPRITE_Y * 3;
    }
    if (isMove)
        currentDir = dir;
    dir = 0;
    if (isMove)
    {
        srcRect->x += SPRITE_X;
        srcRect->x %= (SPRITE_X * 3);
    }
    x = destRect->x;
    y = destRect->y;
    Map::isBlocked[int(x) / SPRITE_X][int(y) / SPRITE_Y]++;
}
