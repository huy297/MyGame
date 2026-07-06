#include "entities/Items.h"
#include "core/World.h"
#include "core/Defs.h"
#include "graphics/Graphics.h"
#include "map/Logic.h"
using namespace std;

Death::Death(const char *filename, int x, int y)
{
    texture = Graphics::loadTexture(filename);
    srcRect = std::make_unique<SDL_Rect>();
    destRect = std::make_unique<SDL_Rect>();
    MakeRect(srcRect.get(), 0, 0, 48, 48);
    MakeRect(destRect.get(), x, y, 48, 48);
    framesLeft = 4;
}

void Death::update()
{
    framesLeft--;
    srcRect->x += 48;
}

Item::Item(const char *filename, int x, int y, ItemName nameItem)
{
    texture = Graphics::loadTexture(filename);
    srcRect = std::make_unique<SDL_Rect>();
    destRect = std::make_unique<SDL_Rect>();
    MakeRect(srcRect.get(), 0, 0, 32, 32);
    MakeRect(destRect.get(), x, y, 32, 32);
    spawnedAtMs = Logic::currentTime();
    this->nameItem = nameItem;
}

bool Item::update(World &world)
{
    if (Logic::currentTime() - spawnedAtMs >= 30000)
    {
        return false;
    }
    for (auto other : world.collidables())
    {
        if (Logic::intersect(this->destRect.get(), other->destRect.get()) > 10)
        {
            if (nameItem == Health)
            {
                other->hp += 100;
                other->hp = min(other->hp, other->maxHp);
            }
            else if (nameItem == Magazine)
            {
                other->magazine += 40;
                other->magazine = min(other->magazine, other->maxMagazine);
            }
            else
            {
                other->numGren++;
                other->numGren = min(other->numGren, other->maxGren);
            }
            return false;
        }
    }
    return true;
}
