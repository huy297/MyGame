#include "Structs.h"
#include "Graphics.h"
#include "Logic.h"
#include "Math.h"
#include "Algo.h"
#include<iostream>
#include<SDL_mixer.h>
using namespace std;

int DirectionY[] = {0,-1,1,0,0};
int DirectionX[] = {0,0,0,-1,1};


list<Entity*> MovingThings;


bool canGo(SDL_Rect *dest){
        for (auto x : MovingThings)
        {
            SDL_Rect *u = x->destRect;
           // cout << u->player->destRect->x << ' ' << u->player->destRect->y << ' ' << player->destRect->x << ' ' << player->destRect->y << " dmm\n";
            if ((u->x == dest->x) && (u->y == dest->y) && (u->w == dest->w) && (u->h == dest->h)) continue;
            if (Logic::intersect(u,dest) > 50) return false;
        }
        return true;
}

bool bulletCanGo(Bullet *bullet){
        for (auto x : MovingThings)
        {
            if (x == bullet->owner) continue;
            SDL_Rect *u = x->destRect;
            if (Logic::intersect(u,bullet->destRect))
            {
                x->hp -= 15;
                return false;
            }
        }
        return true;
}
void MakeRect (SDL_Rect *rect, int x, int y, int w, int h) {
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}


Entity::Entity(){
    dx = 0;
    dy = 0;
    speedx = 10;
    speedy = 10;
    x = SCREEN_HEIGHT/2+50;
    y = SCREEN_WIDTH/2+50;
    hp = 100;
    magazine = 40;
    numGren = 2;

    maxHp = 200;
    maxMagazine = 80;
    maxGren = 3;
   // x = 0; y = 0;

    srcRect = new SDL_Rect();

    srcRect->x = 0;
    srcRect->y = 0;
    srcRect->w = SPRITE_X;
    srcRect->h = SPRITE_Y;


    destRect = new SDL_Rect();
    destRect->x = x; // Vị trí x trên màn hình
    destRect->y = y; // Vị trí y trên màn hình
    destRect->w = SPRITE_X; // Width of displayed frame on screen
    destRect->h = SPRITE_Y; // Height of displayed frame on screen

    Space = false;

    dir = 0;
    currentDir = Up;
}

Entity::~Entity(){}

void Entity::loadTexture (const char* filename)
{
    texture = IMG_LoadTexture(Game::renderer,filename);
}

bool Entity::checkValidMove()
{
    if (Logic::canMove(destRect->x+dx,destRect->y+dy,this->dir) == 0)
    {
        dx = 0;
        dy = 0;
        return false;
    }
    destRect->x += dx;
    destRect->y += dy;
    if (canGo(destRect) == false)
    {
        destRect->x -=dx;
        destRect->y -= dy;
        return false;
    }
    destRect->x -=dx;
    destRect->y -= dy;
    return true;
}
void Entity::update()
{
    bool isMove = dir;
    Map::isBlocked[int(x)/48][int(y)/48]--;
    if (Entity::checkValidMove() == true)
    {
        destRect->x += dx;
        destRect->y += dy;
    }

    dx = 0; dy = 0;
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
       // srcRect->x = 0;
        srcRect->y = SPRITE_Y * 2;
    }
    if (dir == Left)
    {
        //srcRect->x = 0;
        srcRect->y = SPRITE_Y * 3;
    }
    if (isMove) currentDir = dir;
    dir = 0;
    if (isMove) {

    srcRect->x += SPRITE_X;
    srcRect->x %= (SPRITE_X*3);
    }
    x = destRect->x;
    y = destRect->y;
    Map::isBlocked[int(x)/48][int(y)/48]++;
    //srcRect->x %= 128*16;
}

Weapon::Weapon(const char* filename, Entity *owner)
{
    Weapon::loadTexture(filename);
    Weapon::setOwner(owner);
    srcRect = new SDL_Rect();
    destRect = new SDL_Rect();
    lastActived = 0;
}
Weapon::~Weapon()
{

}
void Weapon::loadTexture(const char* filename) {
    texture = IMG_LoadTexture(Game::renderer,filename);
}
void Weapon::setOwner(Entity *owner) {
    this->owner = owner;
}

WeaponEffect::WeaponEffect(const char* filename, Weapon *owner)
{
    texture = IMG_LoadTexture(Game::renderer,filename);
    srcRect = new SDL_Rect();
    destRect = new SDL_Rect();
    this->owner = owner;
    Time = 0;
}
WeaponEffect::~WeaponEffect()
{

}


Gun::Gun(const char *filename, Entity *owner) : Weapon(filename,owner) {
    Gun::update();
}
Gun::~Gun() {}



void Gun::shot()
{
    if (owner->magazine <= 0)
    {
        return;
    }
    cout << "bonk\n";
    Mix_Chunk *gJump = Graphics::loadSound("sound/fire.mp3");
    Graphics::play(gJump);
    if (gJump != nullptr) Mix_FreeChunk(gJump);
    owner->magazine--;
    Bullet *newBullet = new Bullet("img/bullet.png",destRect->x,destRect->y,owner->currentDir,owner);
    bullet.push_back(newBullet);
    //cerr << owner->x << ' ' << owner->y << ' ' << newBullet->destRect->x << ' ' << newBullet->destRect->y << "owner bullet" << endl;
}

void Gun::update() {
    switch (owner->currentDir)
    {
    case Right:
        MakeRect(srcRect,0,0,32,16);
        MakeRect(destRect,owner->x+10,owner->y+18,32,16);
        break;
    case Left:
        MakeRect(srcRect,0,16,32,16);
        MakeRect(destRect,owner->x,owner->y+15,32,16);
        break;
    case Down:
        MakeRect(srcRect,0,32,16,32);
        MakeRect(destRect,owner->x+25,owner->y+10,16,32);
        break;
    case Up:
        MakeRect(srcRect,16,32,16,32);
        MakeRect(destRect,owner->x+25,owner->y-5,16,32);
        break;
    }
}

void Gun::updateAllBullet()
{
    auto x = bullet.begin();
    while (x != bullet.end())
    {
        auto u = *x;
        auto tmp = x++;
        if (u->Time <= 0)
        {
            delete(u);
            bullet.erase(tmp);
        }
        else
        {
            u->update();
        }
    }
}

Bullet::Bullet(const char* filename, int x, int y, int dir, Entity *owner)
{
    this->dir = dir;
    this->Time = 15;
    speed = 30;
    Bullet::loadTexture(filename);
    srcRect = new SDL_Rect();
    destRect = new SDL_Rect();
    this->owner = owner;
    setUp(x,y);
}
Bullet::~Bullet()
{

}
void Bullet::loadTexture(const char* filename) {
    texture = IMG_LoadTexture(Game::renderer,filename);
}
void PrintRect (SDL_Rect *a)
{
    cout << "my dfk " << a->x << ' ' << a->y << ' ' << a->w << ' ' << a->h << endl;
}
void Bullet::setUp(int x, int y)
{
    switch (dir)
    {
    case Up:
        MakeRect(srcRect,16,48,16,16);
        MakeRect(destRect,x+GUN_X/2-12,y+GUN_Y/2-20,16,16);
        break;
    case Down:
        MakeRect(srcRect,0,0,16,16);
        MakeRect(destRect,x+GUN_X/2-14,y+GUN_Y/2+2,16,16);
        break;
    case Right:
        MakeRect(srcRect,0,80,16,16);
        MakeRect(destRect,x+GUN_X/2,y+GUN_Y/2-10,16,16);
        break;
    case Left:
        MakeRect(srcRect,48,64,16,16);
        MakeRect(destRect,x-16,y+GUN_Y/2-10,16,16);
        break;
    }
}

void Bullet::update()
{
    if (Logic::canGetThrough(destRect->x,destRect->y) == false || bulletCanGo(this) == 0)
    {
        Time = 0;
        return;
    }
    Time--;
    switch (dir)
    {
    case Up:
        if (Time%4==0) srcRect->y -= 16;
        destRect->y -= speed;
        break;
    case Down:
        if (Time%4==0) srcRect->y += 16;
        destRect->y += speed;
        break;
    case Left:
        if (Time%4==0) srcRect->x -= 16;
        destRect->x -= speed;
        break;
    case Right:
        if (Time%4==0) srcRect->x += 16;
        destRect->x += speed;
        break;
    }
}

Sword::Sword(const char* filename, Entity *owner) : Weapon(filename, owner)
{

    isSlashed = false;
    Sword::update();
    PrintRect(srcRect);
    PrintRect(destRect);

    toSlash = new SwordSlash("img/SlashSword.png",this);
}

void Sword::update()
{
    switch (owner->currentDir)
    {
    case Right:
        MakeRect(srcRect,0,0,32,16);
        MakeRect(destRect,owner->x+15,owner->y+20,32,16);
        break;
    case Left:
        MakeRect(srcRect,0,16,32,16);
        MakeRect(destRect,owner->x-5,owner->y+20,32,16);
        break;
    case Down:
        MakeRect(srcRect,0,32,16,32);
        MakeRect(destRect,owner->x+5,owner->y+15,16,32);
        break;
    case Up:
        MakeRect(srcRect,16,32,16,32);
        MakeRect(destRect,owner->x+25,owner->y-8,16,32);
        break;
    }
    if (isSlashed)
    {
        toSlash->update();
        if (toSlash->Time == 0)
        {
            isSlashed = false;
            toSlash->Time = 4;
        }
    }

    this->dir = owner->currentDir;
}

void Sword::cut()
{
    long long Now = Logic::currentTime();
    if (Now - lastActived <= 500)
    {
        return;
    }
    lastActived = Now;
    isSlashed = true;
    toSlash->setUp(destRect->x,destRect->y);
    for (auto u : MovingThings)
    {
        if (u == this->owner) continue;
        if (Logic::intersect(toSlash->destRect,u->destRect) > 10)
        {
            u->hp -= 30;
        }
    }
}

SwordSlash::SwordSlash(const char *filename, Weapon *owner) : WeaponEffect(filename,owner)
{
    SwordSlash::setUp(owner->destRect->x,owner->destRect->y);
}

SwordSlash::~SwordSlash()
{

}

void SwordSlash::setUp(int x, int y)
{
    // remember -3 to render
    Time = 4;
    switch (owner->dir)
    {
    case Up:
        MakeRect(srcRect,-2*BLOCK,4*BLOCK,2*BLOCK,3*BLOCK);
        MakeRect(destRect,x-10,y-35,2*BLOCK,3*BLOCK);
        break;
    case Down:
        MakeRect(srcRect,-2*BLOCK,7*BLOCK,2*BLOCK,3*BLOCK);
        MakeRect(destRect,x-5,y+15,2*BLOCK,3*BLOCK);
        break;
    case Right:
        MakeRect(srcRect,-3*BLOCK,0*BLOCK,3*BLOCK,2*BLOCK);
        MakeRect(destRect,x+20,y-10,48,32);
        break;
    case Left:
        MakeRect(srcRect,-3*BLOCK,2*BLOCK,3*BLOCK,2*BLOCK);
        MakeRect(destRect,x-27,y-10,3*BLOCK,2*BLOCK);
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
    Time--;
}

Grenade::Grenade(const char *filename, Entity *owner) : Weapon(filename, owner)
{
    isActived = false;
    pressTime = 0;
    releaseTime = 0;
    speed = 0;
    Time = 0;
    isReleased = false;
    isExplosed = false;
    Grenade::setUp();

}

void Grenade::setUp()
{
    switch (owner->currentDir)
    {
    case Up:
        MakeRect(srcRect,0,0,BLOCK,BLOCK);
        break;
    case Down:
        MakeRect(srcRect,0,0,BLOCK,BLOCK);
        break;
    case Right:
        MakeRect(srcRect,0,0,BLOCK,BLOCK);
        break;
    case Left:
        MakeRect(srcRect,0,0,BLOCK,BLOCK);
        break;
    }
    Grenade::update();
}

void Grenade::backToPos ()
{
    int x = owner->x;
    int y = owner->y;
    dir = owner->currentDir;
    switch (owner->currentDir)
    {
    case Up:
        MakeRect(destRect,x+30,y+10,BLOCK,BLOCK);
        break;
    case Down:
        MakeRect(destRect,x,y+15,BLOCK,BLOCK);
        break;
    case Right:
        MakeRect(destRect,x+20,y+20,BLOCK,BLOCK);
        break;
    case Left:
        MakeRect(destRect,x,y+10,BLOCK,BLOCK);
        break;
    }
}
bool grenadeCanGo(Grenade *gre)
{
    for (auto u : MovingThings)
    {
        if (abs(u->x - gre->owner->x) <= 10 && abs(u->y - gre->owner->y) <= 10) continue;
        if (Logic::intersect(gre->destRect,u->destRect) > 10)
        {
            return false;
        }
    }
    return true;
}
void Grenade::update()
{
    if (isExplosed)
    {
        if (explosion->Time == 0)
        {
            isExplosed = false;
             Grenade::setUp();
            Grenade::backToPos();
        }
        else explosion->update();
        return;
    }
    if (isReleased)
    {
        Time--;
        if (Logic::canGetThrough(destRect->x,destRect->y) == false || grenadeCanGo(this) == 0)
        {
            Time = 0;
        }
        if (Time == 0) {
            // explosion;
            // but now for testing purpose, let it rollback to old position
            isReleased = false;
            isExplosed = true;
            Grenade::explose();
            return;
         }
        if (Time == 4)
        {
            speed = 0;
        }
        switch (dir)
        {
        case Up:
            destRect->y -= speed;
            break;
        case Down:
            destRect->y+= speed;
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
    if (isActived)
    {
        return;
    }
    isActived = true;
    pressTime =SDL_GetTicks();
}

void Grenade::mapSpeed()
{
    int delta = releaseTime - pressTime;
    delta = min(delta,600);
    speed = delta / 20 + 3;
    delta /= 150;
    switch (delta)
    {
    case 0:
        Time = 10;
    case 1:
        Time = 14;
    case 2:
        Time = 14;
    case 3:
        Time = 16;
    case 4:
        Time = 18;
    }
}


void Grenade::release()
{
    if (isReleased) return;
    releaseTime = SDL_GetTicks();
    Grenade::mapSpeed();
    isActived = false;
    isReleased = true;
    MakeRect(srcRect,0,2*BLOCK,BLOCK,BLOCK);
}

void Grenade::explose()
{
    this->explosion = new Explosion("img/explosion.png",this);

}

void damageExplosion(SDL_Rect *rect)
{
    for (auto u : Bot::bot)
    {
        int dam = 0;

        if (u->player->currentDir == Up)
        {
            rect->x += 64;
            rect->y += 80;
        }

        int dak = Logic::intersect(rect,u->player->destRect);
        if (dak >= 700) dam = 100;
        else if (dak >= 400) dam = 70;
        else if (dak >= 200) dam = 50;
        else if (dak >= 10) dam = 30;
        u->player->hp -= dam;
        if (u->player->currentDir == Up)
        {
            rect->x -= 64;
            rect->y -= 80;
        }
    }
}

Explosion::Explosion(const char* filename, Weapon *owner) : WeaponEffect(filename,owner)
{
    Explosion::setUp(owner->destRect->x-50,owner->destRect->y-50);
    damageExplosion(destRect);
}

Explosion::~Explosion()
{

}

void Explosion::setUp(int x, int y)
{
    Time = 4;
    MakeRect(srcRect,0,0,8*BLOCK,8*BLOCK);
    MakeRect(destRect,x,y,8*BLOCK,8*BLOCK);
}

void Explosion::update()
{
    Time--;
    srcRect->y += 8*BLOCK;
}

Death::Death(const char *filename, int x, int y)
{
    texture = IMG_LoadTexture(Game::renderer,filename);
    srcRect = new SDL_Rect();
    destRect = new SDL_Rect();
    MakeRect(srcRect,0,0,48,48);
    MakeRect(destRect,x,y,48,48);
    Time = 4;
}

Death::~Death(){}

void Death::update()
{
    Time--;
    srcRect->x += 48;
}

void Death::updateAllDeath()
{
    auto x = dead.begin();
    while (x != dead.end())
    {
        auto u = *x;
        u->update();
        auto tmp = x++;
        auto dak = *tmp;
        if (dak->Time <= 0)
        {
            delete(dak);
            dead.erase(tmp);
        }
    }
}

Item::Item(const char* filename, int x, int y, ItemName nameItem)
{
    texture = IMG_LoadTexture(Game::renderer,filename);
    srcRect = new SDL_Rect();
    destRect = new SDL_Rect();
    MakeRect(srcRect,0,0,32,32);
    MakeRect(destRect,x,y,32,32);
    Time = Logic::currentTime();
    this->nameItem = nameItem;
}

Item::~Item()
{

}

bool Item::update()
{
    if (Logic::currentTime() - Time >= 30000)
    {
        return false;
    }
    for (auto u : MovingThings)
    {
        if (Logic::intersect(this->destRect,u->destRect) > 10)
        {
            if (nameItem == Health)
            {
                u->hp += 100;
                u->hp = min(u->hp,u->maxHp);
            }
            else if (nameItem == Magazine)
            {
                u->magazine += 40;
                u->magazine = min (u->magazine,u->maxMagazine);
            }
            else
            {
                u->numGren ++;
                u->numGren = min(u->numGren,u->maxGren);
            }
            return false;
        }
    }
    return true;
}
void Item::updateAllItem()
{
    auto x = allItem.begin();
    while (x != allItem.end())
    {
        auto u = *x;
        auto tmp = x++;
        if (u->update() == false)
        {
            delete(u);
            allItem.erase(tmp);
        }
    }
}
long long lastGenItem = 0;
void Item::genItem()
{
    long long Now = Logic::currentTime();
    if (Now-lastGenItem >= 500)
    {
        int x = 0, y = 0;
        while (Logic::canMove(x,y,Up) == 0)
        {
            x = Logic::Rand(400,3300);
            y = Logic::Rand(400,3300);
        }
        int t = Logic::Rand(1,3);
        ItemName nameItem = Health;
        if (t == 2) nameItem = Magazine;
        else if (t == 3) nameItem = Gren;
        Item *newItem;
        switch (nameItem)
        {
            case Health:
                newItem = new Item("img/health.png",x,y,nameItem);
                break;
            case Magazine:
                newItem = new Item("img/magazine.png", x, y,nameItem);
                break;
            case Gren:
                newItem = new Item("img/Gren.png", x, y,nameItem);
                break;

        }
        allItem.push_back(newItem);
        lastGenItem = Now;
    }
}

Character::Character() {
    this->player = new Entity();
    this->player->loadTexture("img/simple.png");
    this->sword = new Sword("img/sword.png", this->player);
    this->gun = new Gun("img/SuperGun.png", this->player);
    this->grenade = new Grenade("img/grenade.png", this->player);
    currentWeapon = sword;
    weaponState = SWORD;
}

Character::Character(const char *filename)
{
    this->player = new Entity();
    this->player->loadTexture(filename);
    this->sword = new Sword("img/sword.png", this->player);
    this->gun = new Gun("img/SuperGun.png", this->player);
    this->grenade = new Grenade("img/grenade.png", this->player);
    currentWeapon = sword;
    weaponState = SWORD;
}
Character::~Character() {}
void Character::update() {
    player->update();
    gun->update();
    sword->update();
   grenade->update();
}

Bot::Bot() : Character() {
    this->player->x = SCREEN_HEIGHT;
    this->player->y = SCREEN_WIDTH;
    this->player->destRect->x = SCREEN_HEIGHT+200; // Vị trí x trên màn hình
    this->player->destRect->y = SCREEN_WIDTH+200;
}

Bot::Bot(int x, int y, const char *filename) : Character(filename) {
    this->player->x = x;
    this->player->y = y;
    this->player->destRect->x = x; // Vị trí x trên màn hình
    this->player->destRect->y = y;
}


Bot::~Bot() {}
void Bot::updateInput(int dir, Character *MainCharacter)
{
    int newDir = Logic::canSlash(this->player->destRect,MainCharacter->player->destRect);
    if (newDir)
    {
        this->player->dir = newDir;
        if (this->weaponState != SWORD)
        {
            this->currentWeapon = this->sword;
            this->weaponState = SWORD;
        }
        this->sword->cut();
        this->update();
        return;
    }
    newDir = Logic::canShoot(this->player->destRect,MainCharacter->player->destRect);
    if (newDir && MainCharacter->player->magazine > 0)
    {
            this->player->dir = newDir;
            if (this->weaponState != GUN)
            {
                  this->currentWeapon = this->gun;
                  this->weaponState = GUN;
            }
            this->gun->shot();
            this->update();
            return;
    }
    this->player->dir = dir;
    if (dir & 1)
    {
        this->player->dy += -10;
    }
    if (dir & 2)
    {

        this->player->dy += 10;
    }
    if (dir & 4)
    {

        this->player->dx += -10;
    }
    if (dir & 8)
    {
        this->player->dx += 10;
    }
    if (Logic::canMove(this->player->x+this->player->dx,this->player->y+this->player->dy,this->player->dir) == 0)
    {
            int dir = Logic::Rand(1,4);
            Bot::updateInput(dir,MainCharacter);
            return;
    }
    this->update();
}
bool OkK = true;
void addDead(SDL_Rect *rect)
{
    Death *u = new Death("img/Death.png", rect->x, rect->y);
    Death::dead.push_back(u);
}
void Bot::updateAllBot(Character *MainCharacter)
{
    SDL_Rect *player = MainCharacter->player->destRect;
    //cout << x << ' ' << y << " Player\n";
    Point c = {(player->x)/10,(player->y)/10};
    dijkstra(c);
    MovingThings.clear();
    MovingThings.push_back(MainCharacter->player);
    for (auto u : bot) MovingThings.push_back(u->player);
    auto x = bot.begin();
    while (x != bot.end())
    {
        auto u = *x;
        u->updateInput(Trace[int(u->player->x)/10][int(u->player->y)/10],MainCharacter);
        auto tmp = x++;
        if ((*tmp)->player->hp <= 0)
        {
            addDead((*tmp)->player->destRect);
            delete((*tmp)->player);
            bot.erase(tmp);
        }
    }
}
long long lastGen = 0;

void Bot::genBot()
{
    long long Now = Logic::currentTime();
    if (Now - lastGen >= 3300)
    {
        int x = 0, y = 0;
        while (Logic::canMove(x,y,Up) == 0)
        {
            x = Logic::Rand(900,3300);
            y = Logic::Rand(900,3300);
        }
        int t = Logic::Rand(1,3);

        Bot *newBot;
        switch (t)
        {
        case 1:
             newBot = new Bot(x,y,"img/Bot_1.png");
             break;
        case 2:
             newBot = new Bot(x,y,"img/Bot_2.png");
             break;
        case 3:
            newBot = new Bot(x,y,"img/Bot_3.png");
            break;
        }
        Bot::bot.push_back(newBot);
        lastGen = Now;
    }
}
