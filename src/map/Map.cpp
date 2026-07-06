#include "map/Map.h"
#include "graphics/Graphics.h"
#include <cstring>
#include <string>
#include <iostream>
using namespace std;

int Map::isBlocked[MAPSIZE][MAPSIZE] = {};
bool Map::isWalkable[MAPSIZE][MAPSIZE] = {};
int Map::numBlocked[MAPSIZE][MAPSIZE] = {};

void Map::ProcessData(int row, string line, int layer)
{
    int currentValue = 0;
    int col = 0;
    for (auto ch : line)
    {
        if (ch == ',')
        {
            if (currentValue == 0)
            {
                col++;
                continue;
            }
            switch (layer)
            {
            case 0: // walkable, layer 1
                Map::isWalkable[row][col] = true;
                break;
            case 1: // unwalkable, layer 1
                Map::isWalkable[row][col] = false;
                break;
            case 2: // walkable, layer 2
                Map::isWalkable[row][col] = true;
                break;
            case 3: // unwalkable, layer 2 (also blocks line of sight)
                Map::isWalkable[row][col] = false;
                Map::isBlocked[row][col] = true;
                break;
            }
            Map::numBlocked[row][col] = Map::isBlocked[row][col];
            col++;
            currentValue = 0;
        }
        else
            currentValue = currentValue * 10 + ch - '0';
    }
}

Map::Map()
{
    texture = Graphics::loadTexture("MyMap/VeryBigMap.png");
    srcRect = std::make_unique<SDL_Rect>();
    srcRect->x = 0;
    srcRect->y = 0;
    srcRect->w = SCREEN_WIDTH;
    srcRect->h = SCREEN_HEIGHT;

    destRect = std::make_unique<SDL_Rect>();
    destRect->x = 0;
    destRect->y = 0;
    destRect->w = SCREEN_WIDTH;
    destRect->h = SCREEN_HEIGHT;
    Map::LoadMap();
}

void Map::LoadMap()
{
    memset(Map::numBlocked, 0, sizeof(Map::numBlocked));
    for (int layerIndex = 0; layerIndex <= 3; layerIndex++)
    {
        string filename = "MyMap/mymap" + to_string(layerIndex) + ".txt";
        freopen(filename.c_str(), "r", stdin);
        string headerLine;
        cin >> headerLine;
        for (int row = 0; row < 80; row++)
        {
            string line;
            cin >> line;
            Map::ProcessData(row, line, layerIndex);
        }
    }

    for (int i = 0; i < 80; i++)
    {
        for (int j = 0; j < 80; j++)
        {
            if (i < 9 || j < 9)
                isWalkable[i][j] = false;
        }
    }
    for (int i = 1; i < 80; i++)
    {
        for (int j = 1; j < 80; j++)
        {
            Map::numBlocked[i][j] += Map::numBlocked[i - 1][j] + Map::numBlocked[i][j - 1] - Map::numBlocked[i - 1][j - 1];
        }
    }
}
