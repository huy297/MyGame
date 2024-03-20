#ifndef STRUCTS_H
#define STRUCTS_H

#include "Game.h"

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

#endif // STRUCTS_H
