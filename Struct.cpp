#include "Structs.h"
#include "Graphics.h"
#include<iostream>
using namespace std;

int DirectionY[] = {0,-1,1,0,0};
int DirectionX[] = {0,0,0,-1,1};

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
    x = 100;
    y = 100;

    srcRect = new SDL_Rect();

    srcRect->x = 0;
    srcRect->y = 0;
    srcRect->w = SPRITE_X;
    srcRect->h = SPRITE_Y;


    destRect = new SDL_Rect();
    destRect->x = 100; // Vị trí x trên màn hình
    destRect->y = 100; // Vị trí y trên màn hình
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

void Entity::update()
{
    bool isMove = dir;
    destRect->x += dx;
    destRect->y += dy;
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
    //srcRect->x %= 128*16;
}

Weapon::Weapon(const char* filename, Entity *owner)
{
    Weapon::loadTexture(filename);
    Weapon::setOwner(owner);
    srcRect = new SDL_Rect();
    destRect = new SDL_Rect();
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

Gun::Gun(const char *filename, Entity *owner) : Weapon(filename,owner) {
    Gun::update();
}
Gun::~Gun() {}



void Gun::shot()
{
    Bullet *newBullet = new Bullet("img/bullet.png",destRect->x,destRect->y,owner->currentDir);
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
    vector<Bullet*> tmp;
    for (auto currentBullet : bullet)
    {
       if (currentBullet->Time > 0)
       {
           tmp.push_back(currentBullet);
           currentBullet->update();
       }
    }
    swap(bullet,tmp);
}

Bullet::Bullet(const char* filename, int x, int y, int dir)
{
    this->dir = dir;
    this->Time = 15;
    speed = 30;
    Bullet::loadTexture(filename);
    srcRect = new SDL_Rect();
    destRect = new SDL_Rect();
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
        MakeRect(destRect,x+GUN_X/2-12,y+GUN_Y/2-10,16,16);
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
        MakeRect(destRect,x+GUN_X/2-8,y+GUN_Y/2-10,16,16);
        break;
    }
}

void Bullet::update()
{
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
    Sword::update();

    PrintRect(srcRect);
    PrintRect(destRect);

    isSlashed = false;
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
    isSlashed = true;
    toSlash->setUp(destRect->x,destRect->y);
}

SwordSlash::SwordSlash(const char *filename, Weapon *owner)
{
    texture = IMG_LoadTexture(Game::renderer,filename);
    srcRect = new SDL_Rect();
    destRect = new SDL_Rect();
    Time = 4;
    this->owner = owner;
    SwordSlash::setUp(owner->destRect->x,owner->destRect->y);
}

SwordSlash::~SwordSlash()
{

}

void SwordSlash::setUp(int x, int y)
{
    // remember -3 to render
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
    Time--;
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
}



Character::Character(Entity *player, Gun *gun, Sword *sword) {
    this->player = player;
    this->sword = sword;
    this->gun = gun;
    currentWeapon = sword;
}
Character::~Character() {}
void Character::update() {
    player->update();
    gun->update();
    sword->update();
}
