#include "gun.h"

#include "core/server.h"

extern Server* server;
Gun::Gun(QObject *parent) : QObject(parent)
{
}

void Gun::update()
{
    for(int i = 0; i < bullets.size(); i++)
        bullets[i]->update();
    handleCollision();
}

void Gun::handleCollision()
{
    for(int i = 0; i < bullets.size(); i++)
    {
        if(handleCollisionHelp(bullets[i], i)) continue; // we do this so if the bullet is deleted that it skips the second check becouse it causes our of bounds index obs :3
        handleCollisionHelp(Tile(0, 0, TileType::BOX), i); // how can something so wrong feel so good PS: pls fix senpai
    }
}

bool Gun::handleCollisionHelp(Bullet* bullet, int index)
{
    QRectF bulletBox(bullet->getPosition().x(), bullet->getPosition().y(), BULLET_SIZE, BULLET_SIZE);
    QList<Mob*> mobs = server->getGame()->getMobs();
    for(Mob* mob : mobs)
    {
        if(mob->getMobType() == MobType::PLAYER) continue;
        QRectF mobBox(mob->getPosition().x(), mob->getPosition().y() - 70.0f, MOB_WIDTH, MOB_HEIGHT + 70.0f); // We need the mob boundingBox (see ZombieGame)
        if(bulletBox.intersects(mobBox))
        {
            if(mob->getMobType() != MobType::PLAYER) emit mobHit(mob);

            delete bullets[index];
            bullets.removeAt(index);

            return true;
        }
    }

    return false;
}

bool Gun::handleCollisionHelp(Tile tile, int index)
{
    QList<Tile> tiles = server->getGame()->getLevel()->getTiles();
    for(int j = 0; j < tiles.size(); j++)
    {
        if(bullets[index]->isColliding(&tiles[j]))
        {
            delete bullets[index];
            bullets.removeAt(index);
            return true;
        }
    }

    return false;
}

void Gun::addBullet(Bullet* bullet)
{
    bullets.push_back(bullet);
}
