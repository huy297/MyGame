#include "map.h"
#include "Graphics.h"
#include "Game.h"

Map::Map(){
    ground = Graphics::LoadTexture("img/ground.png");
    tree = Graphics::LoadTexture("img/tree.png");
    grass = Graphics::LoadTexture("img/grass.png");
    Map::LoadMap();
}

Map::~Map(){}

void Map::LoadMap()
{
    for (int i=0; i<60; i++)
        for (int j=0; j<60; j++) MyMap[i][j] = (i+j)%2;
}

void Map::DrawMap()
{
    for (int i=0; i<60; i++)
    {
        for (int j=0; j<60; j++)
        {
            switch (MyMap[i][j])
            {
            case 0:
                Game::addTexture(grass,i*16,j*16);
                break;
            case 1:
                Game::addTexture(ground,i*16,j*16);
                break;
            default:
                break;
            }
        }
    }
}
