#ifndef ITEMS_H
#define ITEMS_H

#include <SDL.h>
#include <memory>

class World;

class Death
{
public:
    SDL_Texture *texture;
    std::unique_ptr<SDL_Rect> srcRect;
    std::unique_ptr<SDL_Rect> destRect;
    int framesLeft;
    Death(const char *filename, int x, int y);
    void update();
};

enum ItemName
{
    Health = 1,
    Magazine = 2,
    Gren = 3
};

class Item
{
public:
    SDL_Texture *texture;
    std::unique_ptr<SDL_Rect> srcRect;
    std::unique_ptr<SDL_Rect> destRect;
    long long spawnedAtMs;
    Item(const char *filename, int x, int y, ItemName nameItem);
    bool update(World &world);
    ItemName nameItem;
};

#endif // ITEMS_H
