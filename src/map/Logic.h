#ifndef LOGIC_H
#define LOGIC_H

#include "map/Map.h"
#include "core/Defs.h"
#include <algorithm>
#include <chrono>
#include <random>
#include <ctime>
#include <cmath>
#include <SDL.h>

extern std::mt19937 ran;

class Logic
{
public:
    static long long currentTime()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    static int intersect(SDL_Rect *rectA, SDL_Rect *rectB)
    {
        if (rectA->x == rectB->x && rectA->y == rectB->y && rectA->w == rectB->w && rectA->h == rectB->h)
            return 0;
        int left = std::max(rectA->x, rectB->x);
        int top = std::max(rectA->y, rectB->y);
        int right = std::min(rectA->x + rectA->w, rectB->x + rectB->w);
        int bottom = std::min(rectA->y + rectA->h, rectB->y + rectB->h);
        if (left < right && top < bottom)
        {
            return (right - left) * (bottom - top);
        }
        return 0;
    }

    static int randomInt(int lo, int hi)
    {
        return lo + ran() % (hi - lo + 1);
    }

    static bool canMove(int x, int y, int dir)
    {
        int deltaX = 0, deltaY = 0;
        switch (dir)
        {
        case Down:
            deltaX = SPRITE_X / 2;
            deltaY = SPRITE_Y;
            break;
        case Left:
            deltaY = SPRITE_Y / 2;
            break;
        case Right:
            deltaX = SPRITE_X / 2;
            deltaY = SPRITE_Y / 2;
            break;
        case Up:
            deltaX = SPRITE_X / 2;
            break;
        }
        int tileCol = (x + deltaX) / SPRITE_X;
        int tileRow = (y + deltaY) / SPRITE_Y;
        if (tileRow < 0 || tileRow >= MAPSIZE || tileCol < 0 || tileCol >= MAPSIZE)
            return false;
        return Map::isWalkable[tileRow][tileCol];
    }

    static bool canGetThrough(int x, int y)
    {
        int tileCol = x / SPRITE_X;
        int tileRow = y / SPRITE_Y;
        if (tileRow < 0 || tileRow >= MAPSIZE || tileCol < 0 || tileCol >= MAPSIZE)
            return false;
        return Map::isBlocked[tileRow][tileCol] == 0;
    }

    // Can the attacker at attackerRect hit the target at targetRect with a
    // ranged shot? Returns the facing direction to shoot in, or 0 if not
    // aligned/in range/blocked by a wall.
    static int canShoot(SDL_Rect *attackerRect, SDL_Rect *targetRect)
    {
        if (std::abs(attackerRect->x - targetRect->x) + std::abs(attackerRect->y - targetRect->y) <= 100)
            return 0;

        int attackerCol = (attackerRect->x + SPRITE_X - 1) / SPRITE_X;
        int attackerRow = attackerRect->y / SPRITE_Y;
        int targetCol = (targetRect->x + SPRITE_X - 1) / SPRITE_X;
        int targetRow = targetRect->y / SPRITE_Y;

        if (std::abs(attackerRect->x - targetRect->x) <= 10)
        {
            if (attackerRect->y >= targetRect->y && attackerRect->y - targetRect->y <= 400)
            {
                if (Map::blockedSize(targetCol, targetRow, attackerCol, attackerRow))
                    return 0;
                return Up;
            }
            else if (attackerRect->y <= targetRect->y && targetRect->y - attackerRect->y <= 400)
            {
                if (Map::blockedSize(attackerCol, attackerRow, targetCol, targetRow))
                    return 0;
                return Down;
            }
        }
        if (std::abs(attackerRect->y - targetRect->y) <= 10)
        {
            if (attackerRect->x <= targetRect->x && targetRect->x - attackerRect->x <= 400)
            {
                if (Map::blockedSize(attackerCol, attackerRow, targetCol, targetRow))
                    return 0;
                return Right;
            }
            else if (attackerRect->x >= targetRect->x && attackerRect->x - targetRect->x <= 400)
            {
                if (Map::blockedSize(targetCol, targetRow, attackerCol, attackerRow))
                    return 0;
                return Left;
            }
        }
        return 0;
    }

    // Can the attacker at attackerRect melee the target at targetRect?
    // Returns the facing direction to swing in, or 0 if out of range.
    static int canSlash(SDL_Rect *attackerRect, SDL_Rect *targetRect)
    {
        if (std::abs(attackerRect->x - targetRect->x) <= 20)
        {
            if (attackerRect->y >= targetRect->y && attackerRect->y - targetRect->y <= 60)
                return Up;
            else if (attackerRect->y <= targetRect->y && attackerRect->y - targetRect->y >= -60)
                return Down;
        }
        if (std::abs(attackerRect->y - targetRect->y) <= 20)
        {
            if (attackerRect->x >= targetRect->x && attackerRect->x - targetRect->x <= 60)
                return Left;
            else if (attackerRect->x <= targetRect->x && attackerRect->x - targetRect->x >= -60)
                return Right;
        }
        return 0;
    }
};

#endif // LOGIC_H
