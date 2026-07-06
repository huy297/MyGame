#ifndef CHARACTER_H
#define CHARACTER_H

#include "entities/Entity.h"
#include "entities/Weapons.h"
#include <memory>

class World;

class Character
{
public:
    Character();
    Character(const char *filename);
    void update(World &world);

public:
    std::unique_ptr<Entity> player;
    std::unique_ptr<Gun> gun;
    std::unique_ptr<Sword> sword;
    std::unique_ptr<Grenade> grenade;
    Weapon *currentWeapon;
    WeaponState weaponState = NONE;
};

class Bot : public Character
{
public:
    Bot();
    Bot(int x, int y, const char *filename);
    void updateInput(int dir, Character *target, World &world);
};

#endif // CHARACTER_H
