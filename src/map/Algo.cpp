#include "map/Algo.h"
using namespace std;

// The 4 cardinal grid steps tried from each cell during the flood, and the
// direction value associated with each step (recorded so bots know which
// way to move, and used to check the source tile can be exited that way).
// kOppositeDir mirrors kStepDir so the destination tile's walkability can
// be checked from its own side too.
static const int kStepDx[4] = {0, 1, -1, 0};
static const int kStepDy[4] = {1, 0, 0, -1};
static const int kStepDir[4] = {Up, Left, Right, Down};
static const int kOppositeDir[4] = {Down, Right, Left, Up};

// All 24 permutations of {0,1,2,3}: picking one via a single randomInt call
// gives the same "random direction-try order per cell" as std::shuffle,
// without its per-call distribution-object overhead — this runs once per
// cell in a flood that can touch well over 100k cells in a single call.
// clang-format off
static const int kPermutations[24][4] = {
    {0, 1, 2, 3}, {0, 1, 3, 2}, {0, 2, 1, 3}, {0, 2, 3, 1}, {0, 3, 1, 2}, {0, 3, 2, 1},
    {1, 0, 2, 3}, {1, 0, 3, 2}, {1, 2, 0, 3}, {1, 2, 3, 0}, {1, 3, 0, 2}, {1, 3, 2, 0},
    {2, 0, 1, 3}, {2, 0, 3, 1}, {2, 1, 0, 3}, {2, 1, 3, 0}, {2, 3, 0, 1}, {2, 3, 1, 0},
    {3, 0, 1, 2}, {3, 0, 2, 1}, {3, 1, 0, 2}, {3, 1, 2, 0}, {3, 2, 0, 1}, {3, 2, 1, 0},
};
// clang-format on

static int trace[N][N];

// Generational visited-tracking instead of memset-ing the whole N*N grid
// every call (most of which goes untouched by any single flood): bump a
// counter once per call and lazily treat a cell as unvisited until its
// stored generation matches the current one. Avoids an O(N^2) reset on
// every call for free — same result, far less wasted work.
static int visitedGen[N][N];
static int currentGen = 0;

int traceDirection(int gx, int gy)
{
    if (gx < 0 || gx >= N || gy < 0 || gy >= N)
        return -1;
    return visitedGen[gx][gy] == currentGen ? trace[gx][gy] : -1;
}

void computeBotDirections(Point target)
{
    currentGen++;
    queue<Point> frontier;
    frontier.push(target);
    visitedGen[target.x][target.y] = currentGen;
    trace[target.x][target.y] = 0;

    while (!frontier.empty())
    {
        Point cur = frontier.front();
        frontier.pop();
        const int *order = kPermutations[Logic::randomInt(0, 23)];
        for (int j = 0; j < 4; j++)
        {
            int dirIndex = order[j];
            int nx = cur.x + kStepDx[dirIndex];
            int ny = cur.y + kStepDy[dirIndex];
            if (nx < 0 || nx >= N || ny < 0 || ny >= N)
                continue;
            if (visitedGen[nx][ny] == currentGen)
                continue;
            if (!Logic::canMove(nx * 10, ny * 10, kOppositeDir[dirIndex]) || !Logic::canMove(cur.x * 10, cur.y * 10, kStepDir[dirIndex]))
                continue;
            visitedGen[nx][ny] = currentGen;
            trace[nx][ny] = kStepDir[dirIndex];
            frontier.push({nx, ny});
        }
    }
}
