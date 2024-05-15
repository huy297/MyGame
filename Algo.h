#ifndef Algo_h
#define Algo_h
#include "defs.h"
#include "map.h"
#include "logic.h"
#include<algorithm>
#include<queue>
using namespace std;
struct Point
{
    int x,y;
};
struct Tron
{
    int x,y,w;
    bool operator < (const Tron &other) const
    {
        return w > other.w;
    }
};
const int N = 350;
int mx[] = {0,1,-1,0,1,1,-1,-1};
int my[] = {1,0,0,-1,1,-1,1,-1};
int Dir[] = {Up,Left,Right,Down,Left,Left,Left,Right,Right}; //remember this is going opposite
int RightDir[] = {Down,Right,Left,Up,Right,Right,Left,Left};
int TraceDir[] = {Up,Left,Right,Down,Left,LeftAndUp,LeftAndDown,RightAndUp,RightAndDown}; //remember this is going opposite
int TraceRightDir[] = {Down,Right,Left,Up,RightAndDown,RightAndUp,LeftAndDown,LeftAndUp};
int Trace[N][N];
int RightTrace[N][N];
int dist[N][N];
Point m[N][N];
bool mini (int &a, int b)
{
    if (a > b)
    {
        a = b;
        return true;
    }
    return false;
}
bool valid (int u, int v)
{
    return u >= 0 && u < N  && v >= 0 && v < N;
}

void bfs (Point st)
{
    queue<Point> q;
    q.push(st);
    for (int i=0; i<N; i++) for (int j=0; j<N; j++) Trace[i][j] = -1;
    Trace[st.x][st.y] = 0;
    int p[4]; p[0] = 0; p[1] = 1; p[2] = 2; p[3] = 3;
    while (q.size())
    {
        Point u = q.front();
        //cout << u.x << ' ' << u.y << " loang\n";
        q.pop();
        random_shuffle(p,p+4);
        for (int j=0; j<4; j++)
        {
            int i = p[j];
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
            Trace[x][y] = TraceDir[i];
            RightTrace[x][y] = TraceRightDir[i];
            m[x][y] = {u.x,u.y};
            q.push({x,y});
        }
    }
}

void dijkstra(Point st)
{
    priority_queue<Tron> pq;
    pq.push({st.x,st.y,0});
    for (int i=0; i<N; i++) for (int j=0; j<N; j++)
    {
        Trace[i][j] = -1;
        dist[i][j] = 1e9;
    }
    Trace[st.x][st.y] = 0;
    dist[st.x][st.y] = 0;
    int p[4]; p[0] = 0; p[1] = 1; p[2] = 2; p[3] = 3;
    while (pq.size())
    {
        Tron dak = pq.top();
        Point u = {dak.x,dak.y};
        int d = dak.w;
        pq.pop();
        if (d != dist[u.x][u.y]) continue;
        random_shuffle(p,p+4);
        for (int j=0; j<4; j++)
        {
            int i = p[j];
            int x = u.x + mx[i];
            int y = u.y + my[i];
            int randomWeight = Logic::Rand(1,25);
            if (!Logic::canMove(x*10,y*10,RightDir[i]) || !Logic::canMove(u.x*10,u.y*10,Dir[i]))
            {
                continue;
            }
            int a = x*10/48;
            int b = y*10/48;
            if (!mini(dist[x][y],d + randomWeight)) continue;
            Trace[x][y] = TraceDir[i];
            RightTrace[x][y] = TraceRightDir[i];
            m[x][y] = {u.x,u.y};
            pq.push({x,y,dist[x][y]});
        }
    }
}

#endif // Algo_h

