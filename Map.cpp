#include "map.h"
#include "Graphics.h"
#include "Game.h"
#include "iostream"
#include<string.h>
using namespace std;
bool Map::isBlocked[MAPSIZE][MAPSIZE] = {};
bool Map::isWalkable[MAPSIZE][MAPSIZE] = {};
void Map::ProcessData(int i, string s, int layer) {
	int cur = 0;
	int j = 0;
	for (auto u : s) {
		if (u == ',') {
            if (cur == 0)
            {
                j++;
                continue;
            }
		    switch (layer)
		    {
            case 0: // walkable 1
                Map::isWalkable[i][j] = true;
                break;
            case 1: // unwalkable 1
                Map::isWalkable[i][j] = false;
                break;
            case 2: //walkable 2
                Map::isWalkable[i][j] = true;
                break;
            case 3: // unwalkable 2
                if (i == 9 && j == 13) cout << " wtf" << endl;
                Map::isWalkable[i][j] = false;
                Map::isBlocked[i][j] = true;
		    }
		    j++;
			cur = 0;
		}
		else cur = cur*10 + u - '0';
	}
}

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
    //freopen("MyMap/outmap.txt", "w", stdout);
    for (int a=0; a<=3; a++)
    {
        string filename = "MyMap/mymap0.txt";
        if (a == 1) filename = "MyMap/mymap1.txt";
        else if (a== 2) filename = "MyMap/mymap2.txt";
        else if (a == 3) filename = "MyMap/mymap3.txt";
        freopen(filename.c_str(), "r", stdin);
        string layer;
        cin >> layer;
        for (int i=0; i<80; i++) {
            string s; cin >> s;
            Map::ProcessData(i,s,a);
        }
    }

    for (int i=0; i<80; i++) {
        for (int j=0; j<80; j++)
        {
            if (i < 9 || j < 9) isWalkable[i][j] = false;
        }
    }
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
