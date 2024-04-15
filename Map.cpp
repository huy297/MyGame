#include "map.h"
#include "Graphics.h"
#include "Game.h"

Map::Map(){
    texture = Graphics::LoadTexture("MyMap/VeryBigMap.png");
    srcRect = new SDL_Rect();
    srcRect->x = 0;
    srcRect->y = 0;
    srcRect->w = SCREEN_WIDTH;
    srcRect->h = SCREEN_HEIGHT;

    destRect = new SDL_Rect();
    destRect->x = 0;
    destRect->y = 0;
    destRect->w = SCREEN_WIDTH;
    destRect->h = SCREEN_HEIGHT;
    Map::LoadMap();
}

Map::~Map(){}

void Map::LoadMap()
{
    for (int i=0; i<15; i++)
        for (int j=0; j<15; j++) MyMap[i][j] = i <= 7;
}

void Map::DrawMap()
{

//    Game::addTexture(MAP,0,0);
//    for (int i=0; i<15; i++)
//    {
//        for (int j=0; j<15; j++)
//        {
//            switch (MyMap[i][j])
//            {
//            case 0:
//                Game::addTexture(grass,i*48,j*48);
//                break;
//            case 1:
//                Game::addTexture(ground,i*48,j*48);
//                break;
//            default:
//                break;
//            }
//        }
//    }
}
