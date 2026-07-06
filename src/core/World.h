#ifndef WORLD_H
#define WORLD_H

#include "core/Defs.h"
#include "entities/Character.h"
#include "entities/Items.h"
#include <list>
#include <memory>
#include <utility>
#include <vector>

// Owns every entity that lives for the length of a playthrough (the player,
// bots, bullets, deaths, items) plus the per-frame collidable-entity list
// used for movement/hit collision. Replaces what used to be static
// std::list<T*> members scattered across Bot/Gun/Death/Item and the loose
// globals (MovingThings, lastGen, lastGenItem) defined in Game.cpp.
class World
{
public:
    Character player;
    std::list<std::unique_ptr<Bot>> bots;
    std::list<std::unique_ptr<Bullet>> bullets;
    std::list<std::unique_ptr<Death>> deaths;
    std::list<std::unique_ptr<Item>> items;

    void update();
    int takeScoreDelta() const
    {
        return scoreDelta_;
    }

    bool isPathClear(SDL_Rect *dest) const;
    bool bulletCanAdvance(Bullet &bullet) const;
    bool grenadeCanAdvance(const Grenade &grenade) const;
    void damageAreaAroundExplosion(SDL_Rect *rect);
    void spawnDeath(SDL_Rect *rect);
    const std::vector<Entity *> &collidables() const
    {
        return collidables_;
    }

private:
    void spawnBots();
    void spawnItems();
    void updateBots();
    void updateItems();
    void updateBullets();
    void updateDeaths();
    void rebuildCollidables();
    std::vector<Entity *> nearbyEntities(SDL_Rect *rect) const;

    std::vector<Entity *> collidables_;
    // Uniform spatial grid bucketing collidables_ by tile (SPRITE_X/Y,
    // matching Logic::canMove/Map::isWalkable), rebuilt each frame in
    // rebuildCollidables(). isPathClear/bulletCanAdvance/grenadeCanAdvance
    // used to linearly scan all of collidables_ (O(n) per query, O(n^2) per
    // frame overall) -- every entity/bullet/grenade rect involved is at most
    // one tile in size, so a 3x3 neighborhood of buckets around a query
    // rect's own tile is always enough to catch every possible overlap.
    std::vector<Entity *> grid_[MAPSIZE][MAPSIZE];
    // Cells touched last populate, so rebuildCollidables() only clears the
    // handful of cells actually used instead of resetting all MAPSIZE^2
    // buckets every frame (same lazy-reset idea as Algo.cpp's visitedGen).
    std::vector<std::pair<int, int>> touchedCells_;
    long long lastBotSpawn_ = 0;
    long long lastItemSpawn_ = 0;
    int scoreDelta_ = 0;
    // computeBotDirections() floods the whole reachable map (~163k cells,
    // ~16ms) every call. The player moves only a few pixels between frames,
    // so recomputing at full 20Hz frequency is wasted work: recompute every
    // 3rd frame instead, letting traceDirection() (already O(1)) keep
    // answering every frame off whatever flood is currently cached. Bot path
    // targets can lag up to ~2 frames (~100ms) behind the player as a result.
    int botPathTick_ = 0;
};

#endif // WORLD_H
