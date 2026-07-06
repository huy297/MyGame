#ifndef ALGO_H
#define ALGO_H
#include "core/Defs.h"
#include "map/Map.h"
#include "map/Logic.h"
#include <queue>

struct Point
{
    int x, y;
};

// Grid is indexed by world coordinate / 10; the map spans MAPSIZE*SPRITE_X
// (80*48 = 3840) in each axis, so indices can reach ~384. N=350 previously
// left this out of bounds near the far edges of the map.
const int N = MAPSIZE * SPRITE_X / 10 + 20;

// Breadth-first floods outward from `target` (in the /10-scaled grid above)
// over walkable tiles, so traceDirection() can answer "which way should a
// bot standing at this cell move to head toward `target`" in O(1) per bot
// afterward. Call once per frame (or however often bot AI should refresh)
// from the target's current position.
void computeBotDirections(Point target);

// Direction a bot standing at grid cell (gx, gy) should move to head toward
// the last computeBotDirections() target, or -1 if that cell wasn't reached
// (out of bounds, disconnected from the target, or not visited this call —
// e.g. the target's own tile sits inside the map's forced-unwalkable
// border until it moves off of it).
int traceDirection(int gx, int gy);

#endif // ALGO_H
