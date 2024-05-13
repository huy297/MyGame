#ifndef Algo_h
#define Algo_h
#include "defs.h"
#include "map.h"
#include "logic.h"
#include<queue>
using namespace std;
struct Point
{
    int x,y;
};
const int N = 350;
int mx[] = {0,1,-1,0};
int my[] = {1,0,0,-1};
int Dir[] = {Up,Left,Right,Down}; //remember this is going opposite
int RightDir[] = {Down,Right,Left,Up};
int Trace[N][N];
int RightTrace[N][N];
Point m[N][N];
bool valid (int u, int v)
{
    return u >= 0 && u < N  && v >= 0 && v < N;
}

void trace (int u, int v, int x, int y)
{
    while (x != u || y != v)
    {
        Point d = m[x][y];
        cout << x*10 << ' ' << y*10 << ' ' << Trace[x][y] << '\n';
        cout << Logic::canMove(x*10,y*10,RightTrace[x][y]) << " cai deo gi the\n";
      //  cout << d.x*5 << ' ' << d.y*5 << ' ' << RightTrace[x][y] << ' ' << Logic::canMove(d.x*5,d.y*5,RightTrace[x][y]) << " wtf it's can move\n";
        x = d.x,y = d.y;
        if (!x || !y) break;
    }
}

void bfs (Point st)
{
    queue<Point> q;
    q.push(st);
    for (int i=0; i<N; i++) for (int j=0; j<N; j++) Trace[i][j] = -1;
    Trace[st.x][st.y] = 0;
    while (q.size())
    {
        Point u = q.front();
        //cout << u.x << ' ' << u.y << " loang\n";
        q.pop();
        for (int i=0; i<4; i++)
        {
            int x = u.x + mx[i];
            int y = u.y + my[i];
            if (!Logic::canMove(x*10,y*10,RightDir[i]) || !Logic::canMove(u.x*10,u.y*10,Dir[i]))
            {
        //        cout << "can't go to " << x*5 << ' ' << y*5 << " ??\n";
                continue;
            }
            int a = x*10/48;
            int b = y*10/48;
            if (Trace[x][y] != -1) continue;
            Trace[x][y] = Dir[i];
            RightTrace[x][y] = RightDir[i];
            m[x][y] = {u.x,u.y};
            q.push({x,y});
        }
    }
}


#endif // Algo_h

