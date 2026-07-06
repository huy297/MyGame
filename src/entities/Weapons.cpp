#include "entities/Weapons.h"
#include "core/World.h"
#include "core/Defs.h"
#include "graphics/Graphics.h"
#include "map/Logic.h"
using namespace std;

Weapon::Weapon(const char *filename, Entity *owner)
{
    Weapon::loadTexture(filename);
    Weapon::setOwner(owner);
    srcRect = std::make_unique<SDL_Rect>();
    destRect = std::make_unique<SDL_Rect>();
    lastUsedAtMs = 0;
}
void Weapon::loadTexture(const char *filename)
{
    texture = Graphics::loadTexture(filename);
}
void Weapon::setOwner(Entity *owner)
{
    this->owner = owner;
}

WeaponEffect::WeaponEffect(const char *filename, Weapon *owner)
{
    texture = Graphics::loadTexture(filename);
    srcRect = std::make_unique<SDL_Rect>();
    destRect = std::make_unique<SDL_Rect>();
    this->owner = owner;
    framesLeft = 0;
}

Gun::Gun(const char *filename, Entity *owner)
    : Weapon(filename, owner), fireSound(Graphics::loadSound("sound/fire.mp3"), Mix_FreeChunk)
{
    Gun::update();
}

void Gun::shot(World &world)
{
    if (owner->magazine <= 0)
    {
        return;
    }
    Graphics::play(fireSound.get());
    owner->magazine--;
    world.bullets.push_back(std::make_unique<Bullet>("Img/bullet.png", destRect->x, destRect->y, owner->currentDir, owner));
}

void Gun::update()
{
    switch (owner->currentDir)
    {
    case Right:
        MakeRect(srcRect.get(), 0, 0, 32, 16);
        MakeRect(destRect.get(), owner->x + 10, owner->y + 18, 32, 16);
        break;
    case Left:
        MakeRect(srcRect.get(), 0, 16, 32, 16);
        MakeRect(destRect.get(), owner->x, owner->y + 15, 32, 16);
        break;
    case Down:
        MakeRect(srcRect.get(), 0, 32, 16, 32);
        MakeRect(destRect.get(), owner->x + 25, owner->y + 10, 16, 32);
        break;
    case Up:
        MakeRect(srcRect.get(), 16, 32, 16, 32);
        MakeRect(destRect.get(), owner->x + 25, owner->y - 5, 16, 32);
        break;
    }
}

Bullet::Bullet(const char *filename, int x, int y, int dir, Entity *owner)
{
    this->dir = dir;
    this->framesLeft = 15;
    speed = 30;
    Bullet::loadTexture(filename);
    srcRect = std::make_unique<SDL_Rect>();
    destRect = std::make_unique<SDL_Rect>();
    this->owner = owner;
    setUp(x, y);
}
void Bullet::loadTexture(const char *filename)
{
    texture = Graphics::loadTexture(filename);
}

void Bullet::setUp(int x, int y)
{
    switch (dir)
    {
    case Up:
        MakeRect(srcRect.get(), 16, 48, 16, 16);
        MakeRect(destRect.get(), x + GUN_X / 2 - 12, y + GUN_Y / 2 - 20, 16, 16);
        break;
    case Down:
        MakeRect(srcRect.get(), 0, 0, 16, 16);
        MakeRect(destRect.get(), x + GUN_X / 2 - 14, y + GUN_Y / 2 + 2, 16, 16);
        break;
    case Right:
        MakeRect(srcRect.get(), 0, 80, 16, 16);
        MakeRect(destRect.get(), x + GUN_X / 2, y + GUN_Y / 2 - 10, 16, 16);
        break;
    case Left:
        MakeRect(srcRect.get(), 48, 64, 16, 16);
        MakeRect(destRect.get(), x - 16, y + GUN_Y / 2 - 10, 16, 16);
        break;
    }
}

void Bullet::update(World &world)
{
    if (Logic::canGetThrough(destRect->x, destRect->y) == false || world.bulletCanAdvance(*this) == 0)
    {
        framesLeft = 0;
        return;
    }
    framesLeft--;
    switch (dir)
    {
    case Up:
        if (framesLeft % 4 == 0)
            srcRect->y -= 16;
        destRect->y -= speed;
        break;
    case Down:
        if (framesLeft % 4 == 0)
            srcRect->y += 16;
        destRect->y += speed;
        break;
    case Left:
        if (framesLeft % 4 == 0)
            srcRect->x -= 16;
        destRect->x -= speed;
        break;
    case Right:
        if (framesLeft % 4 == 0)
            srcRect->x += 16;
        destRect->x += speed;
        break;
    }
}

Sword::Sword(const char *filename, Entity *owner) : Weapon(filename, owner)
{
    isSlashed = false;
    Sword::update();
    toSlash = std::make_unique<SwordSlash>("Img/SlashSword.png", this);
}

void Sword::update()
{
    switch (owner->currentDir)
    {
    case Right:
        MakeRect(srcRect.get(), 0, 0, 32, 16);
        MakeRect(destRect.get(), owner->x + 15, owner->y + 20, 32, 16);
        break;
    case Left:
        MakeRect(srcRect.get(), 0, 16, 32, 16);
        MakeRect(destRect.get(), owner->x - 5, owner->y + 20, 32, 16);
        break;
    case Down:
        MakeRect(srcRect.get(), 0, 32, 16, 32);
        MakeRect(destRect.get(), owner->x + 5, owner->y + 15, 16, 32);
        break;
    case Up:
        MakeRect(srcRect.get(), 16, 32, 16, 32);
        MakeRect(destRect.get(), owner->x + 25, owner->y - 8, 16, 32);
        break;
    }
    if (isSlashed)
    {
        toSlash->update();
        if (toSlash->framesLeft == 0)
        {
            isSlashed = false;
            toSlash->framesLeft = 4;
        }
    }

    this->dir = owner->currentDir;
}

void Sword::cut(World &world)
{
    long long now = Logic::currentTime();
    if (now - lastUsedAtMs <= 500)
    {
        return;
    }
    lastUsedAtMs = now;
    isSlashed = true;
    toSlash->setUp(destRect->x, destRect->y);
    for (auto other : world.collidables())
    {
        if (other == this->owner)
            continue;
        if (Logic::intersect(toSlash->destRect.get(), other->destRect.get()) > 10)
        {
            other->hp -= 30;
        }
    }
}

SwordSlash::SwordSlash(const char *filename, Weapon *owner) : WeaponEffect(filename, owner)
{
    SwordSlash::setUp(owner->destRect->x, owner->destRect->y);
}

void SwordSlash::setUp(int x, int y)
{
    framesLeft = 4;
    switch (owner->dir)
    {
    case Up:
        MakeRect(srcRect.get(), -2 * BLOCK, 4 * BLOCK, 2 * BLOCK, 3 * BLOCK);
        MakeRect(destRect.get(), x - 10, y - 35, 2 * BLOCK, 3 * BLOCK);
        break;
    case Down:
        MakeRect(srcRect.get(), -2 * BLOCK, 7 * BLOCK, 2 * BLOCK, 3 * BLOCK);
        MakeRect(destRect.get(), x - 5, y + 15, 2 * BLOCK, 3 * BLOCK);
        break;
    case Right:
        MakeRect(srcRect.get(), -3 * BLOCK, 0 * BLOCK, 3 * BLOCK, 2 * BLOCK);
        MakeRect(destRect.get(), x + 20, y - 10, 48, 32);
        break;
    case Left:
        MakeRect(srcRect.get(), -3 * BLOCK, 2 * BLOCK, 3 * BLOCK, 2 * BLOCK);
        MakeRect(destRect.get(), x - 27, y - 10, 3 * BLOCK, 2 * BLOCK);
        break;
    }
}

void SwordSlash::update()
{
    switch (owner->dir)
    {
    case Up:
        srcRect->x += 32;
        break;
    case Down:
        srcRect->x += 32;
        break;
    case Left:
        srcRect->x += 48;
        break;
    case Right:
        srcRect->x += 48;
        break;
    }
    framesLeft--;
}

Grenade::Grenade(const char *filename, Entity *owner) : Weapon(filename, owner)
{
    isActive = false;
    pressTime = 0;
    releaseTime = 0;
    speed = 0;
    framesLeft = 0;
    isReleased = false;
    isExploded = false;
    Grenade::setUp();
}

void Grenade::setUp()
{
    switch (owner->currentDir)
    {
    case Up:
        MakeRect(srcRect.get(), 0, 0, BLOCK, BLOCK);
        break;
    case Down:
        MakeRect(srcRect.get(), 0, 0, BLOCK, BLOCK);
        break;
    case Right:
        MakeRect(srcRect.get(), 0, 0, BLOCK, BLOCK);
        break;
    case Left:
        MakeRect(srcRect.get(), 0, 0, BLOCK, BLOCK);
        break;
    }
    // setUp/update mutual recursion below is the same shape as before the
    // rewrite (setUp always primes the sprite then reuses update() to
    // position destRect); update() no longer runs the flight/collision
    // branches here since isReleased/isExploded are both false at this point.
}

void Grenade::backToPos()
{
    int x = owner->x;
    int y = owner->y;
    dir = owner->currentDir;
    switch (owner->currentDir)
    {
    case Up:
        MakeRect(destRect.get(), x + 30, y + 10, BLOCK, BLOCK);
        break;
    case Down:
        MakeRect(destRect.get(), x, y + 15, BLOCK, BLOCK);
        break;
    case Right:
        MakeRect(destRect.get(), x + 20, y + 20, BLOCK, BLOCK);
        break;
    case Left:
        MakeRect(destRect.get(), x, y + 10, BLOCK, BLOCK);
        break;
    }
}

void Grenade::update(World &world)
{
    if (isExploded)
    {
        if (explosion->framesLeft == 0)
        {
            isExploded = false;
            explosion.reset();
            Grenade::setUp();
            Grenade::backToPos();
        }
        else
            explosion->update();
        return;
    }
    if (isReleased)
    {
        framesLeft--;
        if (Logic::canGetThrough(destRect->x, destRect->y) == false || world.grenadeCanAdvance(*this) == 0)
        {
            framesLeft = 0;
        }
        if (framesLeft == 0)
        {
            isReleased = false;
            isExploded = true;
            Grenade::explode(world);
            return;
        }
        if (framesLeft == 4)
        {
            speed = 0;
        }
        switch (dir)
        {
        case Up:
            destRect->y -= speed;
            break;
        case Down:
            destRect->y += speed;
            break;
        case Right:
            destRect->x += speed;
            break;
        case Left:
            destRect->x -= speed;
            break;
        }
        return;
    }
    Grenade::backToPos();
}

void Grenade::active()
{
    if (isActive)
    {
        return;
    }
    isActive = true;
    pressTime = SDL_GetTicks();
}

void Grenade::mapSpeed()
{
    int delta = releaseTime - pressTime;
    delta = min(delta, 600);
    speed = delta / 20 + 3;
    delta /= 150;
    switch (delta)
    {
    case 0:
        framesLeft = 10;
        break;
    case 1:
    case 2:
        framesLeft = 14;
        break;
    case 3:
        framesLeft = 16;
        break;
    default:
        framesLeft = 18;
        break;
    }
}

void Grenade::release()
{
    if (isReleased)
        return;
    releaseTime = SDL_GetTicks();
    Grenade::mapSpeed();
    isActive = false;
    isReleased = true;
    MakeRect(srcRect.get(), 0, 2 * BLOCK, BLOCK, BLOCK);
}

void Grenade::explode(World &world)
{
    this->explosion = std::make_unique<Explosion>("Img/Explosion.png", this, world);
}

Explosion::Explosion(const char *filename, Weapon *owner, World &world) : WeaponEffect(filename, owner)
{
    Explosion::setUp(owner->destRect->x - 50, owner->destRect->y - 50);
    world.damageAreaAroundExplosion(destRect.get());
}

void Explosion::setUp(int x, int y)
{
    framesLeft = 4;
    MakeRect(srcRect.get(), 0, 0, 8 * BLOCK, 8 * BLOCK);
    MakeRect(destRect.get(), x, y, 8 * BLOCK, 8 * BLOCK);
}

void Explosion::update()
{
    framesLeft--;
    srcRect->y += 8 * BLOCK;
}
