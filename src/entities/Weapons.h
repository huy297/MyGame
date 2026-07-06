#ifndef WEAPONS_H
#define WEAPONS_H

#include "entities/Entity.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <memory>

class World;

enum WeaponState
{
    SWORD,
    GUN,
    GRENADE,
    NONE
};

class Weapon
{
public:
    Weapon(const char *filename, Entity *owner);
    void loadTexture(const char *filename);
    void setOwner(Entity *owner);

public:
    Entity *owner;
    SDL_Texture *texture;
    std::unique_ptr<SDL_Rect> srcRect;
    std::unique_ptr<SDL_Rect> destRect;
    long long lastUsedAtMs;
    int dir;
};

class WeaponEffect
{
public:
    WeaponEffect(const char *filename, Weapon *owner);

public:
    SDL_Texture *texture;
    std::unique_ptr<SDL_Rect> srcRect;
    std::unique_ptr<SDL_Rect> destRect;
    int framesLeft;
    Weapon *owner;
};

class Bullet
{
public:
    Bullet(const char *filename, int x, int y, int dir, Entity *owner);
    void loadTexture(const char *filename);
    void update(World &world);
    void setUp(int x, int y);

public:
    SDL_Texture *texture;
    std::unique_ptr<SDL_Rect> srcRect;
    std::unique_ptr<SDL_Rect> destRect;
    int dx, dy, speed;
    int dir;
    Entity *owner;
    int framesLeft;
};

class SwordSlash : public WeaponEffect
{
public:
    SwordSlash(const char *filename, Weapon *owner);
    void slash();
    void update();
    void setUp(int x, int y);
};

class Gun : public Weapon
{
public:
    Gun(const char *filename, Entity *owner);
    void update();
    void shot(World &world);

public:
    std::unique_ptr<Mix_Chunk, void (*)(Mix_Chunk *)> fireSound;
};

class Sword : public Weapon
{
public:
    Sword(const char *filename, Entity *owner);
    void update();
    void cut(World &world);

public:
    std::unique_ptr<SwordSlash> toSlash;
    bool isSlashed;
};

class Explosion : public WeaponEffect
{
public:
    Explosion(const char *filename, Weapon *owner, World &world);
    void update();
    void setUp(int x, int y);
};

class Grenade : public Weapon
{
public:
    Grenade(const char *filename, Entity *owner);
    void update(World &world);
    void active();
    void release();
    void setUp();
    void mapSpeed();
    void explode(World &world);
    void backToPos();

public:
    int framesLeft;
    int dir;
    bool isActive;
    bool isReleased;
    bool isExploded;
    int pressTime, releaseTime;
    int speed;
    std::unique_ptr<Explosion> explosion;
};

#endif // WEAPONS_H
