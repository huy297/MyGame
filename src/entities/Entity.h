#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>
#include <memory>

class World;

class Entity
{
public:
    Entity();
    void loadTexture(const char *filename);
    void update(World &world);
    bool checkValidMove(World &world);

public:
    float x, y;
    float dx, dy;
    float speedx, speedy;
    int dir;
    SDL_Texture *texture;
    std::unique_ptr<SDL_Rect> srcRect;
    std::unique_ptr<SDL_Rect> destRect;
    int currentDir;
    int hp, maxHp;
    int magazine, maxMagazine;
    int numGren, maxGren;
};

#endif // ENTITY_H
