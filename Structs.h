#ifndef STRUCTS_H
#define STRUCTS_H

#include "Game.h"
#include<vector>
using namespace std;

enum WeaponState
{
    SWORD,GUN,GRENADE,NONE
};

class Entity {
public:
  Entity();
  ~Entity();
  void loadTexture(const char* filename);
  void update();
public:
  float x,y;
  float dx,dy;
  float speedx,speedy;
  int dir;
  SDL_Texture *texture;
  SDL_Rect *srcRect;
  SDL_Rect *destRect;
  bool Space;
  int currentDir;
};

class Bullet
{
public:
    Bullet(const char* filename, int x, int y, int dir);
    ~Bullet();
    void loadTexture(const char *filename);
    void update();
    void setUp(int x, int y);
public:
    SDL_Texture *texture;
    SDL_Rect *srcRect;
    SDL_Rect *destRect;
    int dx,dy,speed;
    int dir;
    int Time;
};

class Weapon
{
public:
    Weapon(const char* filename, Entity *owner);
    ~Weapon();
    void loadTexture(const char *filename);
    void setOwner(Entity *owner);
public:
    Entity *owner;
    SDL_Texture *texture;
    SDL_Rect *srcRect;
    SDL_Rect *destRect;
    int dir;
};

class SwordSlash
{
public:
    SwordSlash(const char *filename, Weapon *owner);
    ~SwordSlash();
    void slash();
    void update();
    void setUp(int x, int y);
public:
    SDL_Texture *texture;
    SDL_Rect *srcRect;
    SDL_Rect *destRect;
    int Time;
    Weapon *owner;
};

class Gun : public Weapon{
public:
    Gun(const char* filename, Entity *owner);
    ~Gun();
    void update();
    void shot();
public:
    std::vector<Bullet*> bullet;
};
class Sword : public Weapon
{
public:
    Sword(const char* filename, Entity *owner);
    ~Sword();
    void update();
    void cut();
public:
    SwordSlash *toSlash;
    bool isSlashed;
};

class Grenade : public Weapon
{
public:
    Grenade(const char* filename, Entity *owner);
    ~Grenade();
    void update();
    void shot();
    void setUp(int x, int y);
public:
    int Time;
};


class Character {
public:
    Character();
    ~Character();
    void update();
public:
    Entity *player;
    Gun *gun;
    Sword *sword;
    Grenade *grenade;
    Weapon *currentWeapon;
    WeaponState weaponState = NONE;
};
#endif // STRUCTS_H