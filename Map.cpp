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
    for (int i=0; i<15; i++)
        for (int j=0; j<15; j++) MyMap[i][j] = i <= 7;
}

void Map::DrawMap()
{
    for (int i=0; i<15; i++)
    {
        for (int j=0; j<15; j++)
        {
            switch (MyMap[i][j])
            {
            case 0:
                Game::addTexture(grass,i*48,j*48);
                break;
            case 1:
                Game::addTexture(ground,i*48,j*48);
                break;
            default:
                break;
            }
        }
    }
}
