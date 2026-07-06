#include "core/World.h"
#include "map/Logic.h"
#include "map/Algo.h"
#include <algorithm>
#include <cstdlib>

namespace
{
void cellOf(const SDL_Rect *rect, int &row, int &col)
{
    col = std::clamp(rect->x / SPRITE_X, 0, MAPSIZE - 1);
    row = std::clamp(rect->y / SPRITE_Y, 0, MAPSIZE - 1);
}
} // namespace

void World::rebuildCollidables()
{
    collidables_.clear();
    collidables_.push_back(player.player.get());
    for (auto &bot : bots)
        collidables_.push_back(bot->player.get());

    for (auto &cell : touchedCells_)
        grid_[cell.first][cell.second].clear();
    touchedCells_.clear();

    for (Entity *entity : collidables_)
    {
        int row, col;
        cellOf(entity->destRect.get(), row, col);
        if (grid_[row][col].empty())
            touchedCells_.push_back({row, col});
        grid_[row][col].push_back(entity);
    }
}

std::vector<Entity *> World::nearbyEntities(SDL_Rect *rect) const
{
    std::vector<Entity *> result;
    int row, col;
    cellOf(rect, row, col);
    for (int r = std::max(0, row - 1); r <= std::min(MAPSIZE - 1, row + 1); r++)
        for (int c = std::max(0, col - 1); c <= std::min(MAPSIZE - 1, col + 1); c++)
            for (Entity *entity : grid_[r][c])
                result.push_back(entity);
    return result;
}

bool World::isPathClear(SDL_Rect *dest) const
{
    for (auto entity : nearbyEntities(dest))
    {
        SDL_Rect *entityRect = entity->destRect.get();
        if ((entityRect->x == dest->x) && (entityRect->y == dest->y) && (entityRect->w == dest->w) && (entityRect->h == dest->h))
            continue;
        if (Logic::intersect(entityRect, dest) > 50)
            return false;
    }
    return true;
}

bool World::bulletCanAdvance(Bullet &bullet) const
{
    for (auto entity : nearbyEntities(bullet.destRect.get()))
    {
        if (entity == bullet.owner)
            continue;
        SDL_Rect *entityRect = entity->destRect.get();
        if (Logic::intersect(entityRect, bullet.destRect.get()))
        {
            entity->hp -= 15;
            return false;
        }
    }
    return true;
}

bool World::grenadeCanAdvance(const Grenade &grenade) const
{
    for (auto entity : nearbyEntities(grenade.destRect.get()))
    {
        if (abs(entity->x - grenade.owner->x) <= 10 && abs(entity->y - grenade.owner->y) <= 10)
            continue;
        if (Logic::intersect(grenade.destRect.get(), entity->destRect.get()) > 10)
        {
            return false;
        }
    }
    return true;
}

void World::damageAreaAroundExplosion(SDL_Rect *rect)
{
    for (auto &bot : bots)
    {
        int damage = 0;

        if (bot->player->currentDir == Up)
        {
            rect->x += 64;
            rect->y += 80;
        }

        int overlapArea = Logic::intersect(rect, bot->player->destRect.get());
        if (overlapArea >= 700)
            damage = 100;
        else if (overlapArea >= 400)
            damage = 70;
        else if (overlapArea >= 200)
            damage = 50;
        else if (overlapArea >= 10)
            damage = 30;
        bot->player->hp -= damage;
        if (bot->player->currentDir == Up)
        {
            rect->x -= 64;
            rect->y -= 80;
        }
    }
}

void World::spawnDeath(SDL_Rect *rect)
{
    deaths.push_back(std::make_unique<Death>("Img/Death.png", rect->x, rect->y));
}

void World::spawnBots()
{
    if ((int)bots.size() >= MAX_BOTS)
        return;

    long long now = Logic::currentTime();
    if (now - lastBotSpawn_ >= 3300)
    {
        int x = 0, y = 0;
        while (Logic::canMove(x, y, Up) == 0)
        {
            x = Logic::randomInt(900, 3300);
            y = Logic::randomInt(900, 3300);
        }
        int botKind = Logic::randomInt(1, 3);

        std::unique_ptr<Bot> newBot;
        switch (botKind)
        {
        case 1:
            newBot = std::make_unique<Bot>(x, y, "Img/Bot_1.png");
            break;
        case 2:
            newBot = std::make_unique<Bot>(x, y, "Img/Bot_2.png");
            break;
        case 3:
            newBot = std::make_unique<Bot>(x, y, "Img/Bot_3.png");
            break;
        }
        bots.push_back(std::move(newBot));
        lastBotSpawn_ = now;
    }
}

void World::spawnItems()
{
    long long now = Logic::currentTime();
    if (now - lastItemSpawn_ >= 500)
    {
        int x = 0, y = 0;
        while (Logic::canMove(x, y, Up) == 0)
        {
            x = Logic::randomInt(400, 3300);
            y = Logic::randomInt(400, 3300);
        }
        int itemKind = Logic::randomInt(1, 3);
        ItemName nameItem = Health;
        if (itemKind == 2)
            nameItem = Magazine;
        else if (itemKind == 3)
            nameItem = Gren;
        std::unique_ptr<Item> newItem;
        switch (nameItem)
        {
        case Health:
            newItem = std::make_unique<Item>("Img/health.png", x, y, nameItem);
            break;
        case Magazine:
            newItem = std::make_unique<Item>("Img/magazine.png", x, y, nameItem);
            break;
        case Gren:
            newItem = std::make_unique<Item>("Img/gren.png", x, y, nameItem);
            break;
        }
        items.push_back(std::move(newItem));
        lastItemSpawn_ = now;
    }
}

void World::updateBots()
{
    SDL_Rect *targetRect = player.player->destRect.get();
    if (botPathTick_ % 3 == 0)
    {
        Point targetGridPos = {(targetRect->x) / 10, (targetRect->y) / 10};
        computeBotDirections(targetGridPos);
    }
    botPathTick_++;
    rebuildCollidables();

    // Every bot gets its turn before any are reaped: collidables_ holds raw
    // pointers into each bot's Entity, and killing a bot mid-loop would free
    // that Entity while a later bot's collision check still points at it.
    for (auto &bot : bots)
    {
        Bot *b = bot.get();
        int dir = traceDirection(int(b->player->x) / 10, int(b->player->y) / 10);
        b->updateInput(dir, &player, *this);
    }

    bool anyReaped = false;
    auto it = bots.begin();
    while (it != bots.end())
    {
        if ((*it)->player->hp <= 0)
        {
            spawnDeath((*it)->player->destRect.get());
            it = bots.erase(it);
            anyReaped = true;
        }
        else
        {
            ++it;
        }
    }
    // updateItems()/updateBullets() (and next frame's player update) read
    // collidables_ too; if any bot was just destroyed, the pointer taken
    // for it above is now dangling, so refresh before anything else uses it.
    if (anyReaped)
        rebuildCollidables();
}

void World::updateItems()
{
    auto it = items.begin();
    while (it != items.end())
    {
        auto tmp = it++;
        if ((*tmp)->update(*this) == false)
        {
            items.erase(tmp);
        }
    }
}

void World::updateBullets()
{
    auto it = bullets.begin();
    while (it != bullets.end())
    {
        auto tmp = it++;
        if ((*tmp)->framesLeft <= 0)
        {
            bullets.erase(tmp);
        }
        else
        {
            (*tmp)->update(*this);
        }
    }
}

void World::updateDeaths()
{
    auto it = deaths.begin();
    while (it != deaths.end())
    {
        (*it)->update();
        auto tmp = it++;
        if ((*tmp)->framesLeft <= 0)
        {
            deaths.erase(tmp);
        }
    }
}

void World::update()
{
    // Preserves the original frame order exactly: the player moves against
    // the collidable list left over from the end of the *previous* frame
    // (rebuildCollidables only runs inside updateBots, same as the old
    // Bot::updateAllBot did), while bots/items/bullets see this frame's
    // freshly rebuilt list. Reordering this would change collision timing.
    player.update(*this);
    spawnBots();
    int oldBotCount = (int)bots.size();
    updateBots();
    scoreDelta_ = oldBotCount - (int)bots.size();
    spawnItems();
    updateItems();
    updateBullets();
    updateDeaths();
}
