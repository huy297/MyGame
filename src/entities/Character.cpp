#include "entities/Character.h"
#include "core/Defs.h"
#include "map/Logic.h"

Character::Character()
{
    this->player = std::make_unique<Entity>();
    this->player->loadTexture("Img/Simple.png");
    this->sword = std::make_unique<Sword>("Img/Sword.png", this->player.get());
    this->gun = std::make_unique<Gun>("Img/SuperGun.png", this->player.get());
    this->grenade = std::make_unique<Grenade>("Img/Grenade.png", this->player.get());
    currentWeapon = sword.get();
    weaponState = SWORD;
}

Character::Character(const char *filename)
{
    this->player = std::make_unique<Entity>();
    this->player->loadTexture(filename);
    this->sword = std::make_unique<Sword>("Img/Sword.png", this->player.get());
    this->gun = std::make_unique<Gun>("Img/SuperGun.png", this->player.get());
    this->grenade = std::make_unique<Grenade>("Img/Grenade.png", this->player.get());
    currentWeapon = sword.get();
    weaponState = SWORD;
}
void Character::update(World &world)
{
    player->update(world);
    gun->update();
    sword->update();
    grenade->update(world);
}

Bot::Bot() : Character()
{
    this->player->x = SCREEN_HEIGHT;
    this->player->y = SCREEN_WIDTH;
    this->player->destRect->x = SCREEN_HEIGHT + 200;
    this->player->destRect->y = SCREEN_WIDTH + 200;
}

Bot::Bot(int x, int y, const char *filename) : Character(filename)
{
    this->player->x = x;
    this->player->y = y;
    this->player->destRect->x = x;
    this->player->destRect->y = y;
}

void Bot::updateInput(int dir, Character *target, World &world)
{
    int newDir = Logic::canSlash(this->player->destRect.get(), target->player->destRect.get());
    if (newDir)
    {
        this->player->dir = newDir;
        if (this->weaponState != SWORD)
        {
            this->currentWeapon = this->sword.get();
            this->weaponState = SWORD;
        }
        this->sword->cut(world);
        this->update(world);
        return;
    }
    newDir = Logic::canShoot(this->player->destRect.get(), target->player->destRect.get());
    if (newDir && target->player->magazine > 0)
    {
        this->player->dir = newDir;
        if (this->weaponState != GUN)
        {
            this->currentWeapon = this->gun.get();
            this->weaponState = GUN;
        }
        this->gun->shot(world);
        this->update(world);
        return;
    }
    if (dir == -1)
    {
        this->update(world);
        return;
    }
    this->player->dir = dir;
    if (dir & Up)
    {
        this->player->dy += -10;
    }
    if (dir & Down)
    {
        this->player->dy += 10;
    }
    if (dir & Left)
    {
        this->player->dx += -10;
    }
    if (dir & Right)
    {
        this->player->dx += 10;
    }
    if (Logic::canMove(this->player->x + this->player->dx, this->player->y + this->player->dy, this->player->dir) == 0)
    {
        int retryDir = Logic::randomInt(1, 4);
        Bot::updateInput(retryDir, target, world);
        return;
    }
    this->update(world);
}
