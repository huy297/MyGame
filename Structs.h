#ifndef STRUCTS_H
#define STRUCTS_H

//#include "Game.h"
#include<SDL.h>
#include<list>
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
  bool checkValidMove();
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
  int hp,maxHp;
  int magazine,maxMagazine;
  int numGren,maxGren;
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
    long long lastActived;
    int dir;
};

class WeaponEffect
{
public:
    WeaponEffect(const char *filename, Weapon *owner);
    ~WeaponEffect();
public:
    SDL_Texture *texture;
    SDL_Rect *srcRect;
    SDL_Rect *destRect;
    int Time;
    Weapon *owner;
};

class Bullet
{
public:
    Bullet(const char* filename, int x, int y, int dir, Entity *owner);
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
    Entity *owner;
    int Time;
};


class SwordSlash : public WeaponEffect
{
public:
    SwordSlash(const char *filename, Weapon *owner);
    ~SwordSlash();
    void slash();
    void update();
    void setUp(int x, int y);
public:

};

class Gun : public Weapon{
public:
    Gun(const char* filename, Entity *owner);
    ~Gun();
    void update();
    void shot();
public:
    static void updateAllBullet();
    static std::list<Bullet*> bullet;
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

class Explosion : public WeaponEffect
{
public:
    Explosion(const char *filename, Weapon *owner);
    ~Explosion();
    void update();
    void setUp(int x, int y);
    static list<Explosion*> allExplosion;
    void updateAllExplosion();
};

class Death
{
    public:
        SDL_Texture *texture;
        SDL_Rect *srcRect;
        SDL_Rect *destRect;
        int Time;
        Death(const char *filename, int x, int y);
        ~Death();
        void update();
        static list<Death*> dead;
        static void updateAllDeath();
};
enum ItemName
{
    Health = 1, Magazine = 2, Gren = 3
};
class Item
{
    public:
        SDL_Texture *texture;
        SDL_Rect *srcRect;
        SDL_Rect *destRect;
        long long Time;
        Item(const char *filename, int x, int y, ItemName nameItem);
        ~Item();
        bool update();
        ItemName nameItem;
        static list<Item*> allItem;
        static void updateAllItem();
        static void genItem();
};

class Grenade : public Weapon
{
public:
    Grenade(const char* filename, Entity *owner);
    ~Grenade();
    void update();
    void active();
    void release();
    void setUp();
    void mapSpeed();
    void explose();
    void backToPos();
public:
    int Time;
    int dir;
    bool isActived;
    bool isReleased;
    bool isExplosed;
    int pressTime,releaseTime;
    int speed;
    Explosion *explosion;
    static list<Grenade*> onGoing;
};




class Character {
public:
    Character();
    ~Character();
    Character(const char *filename);
    void update();
public:
    Entity *player;
    Gun *gun;
    Sword *sword;
    Grenade *grenade;
    Weapon *currentWeapon;
    WeaponState weaponState = NONE;
};

class Bot : public Character
{
public:
    Bot();
    ~Bot();
    Bot(int x, int y, const char *filename);
    void updateInput(int dir,Character *MainCharacter);
    static list<Bot*> bot;
    static void updateAllBot(Character *MainCharacter);
    static void genBot();
};

#endif // STRUCTS_H
