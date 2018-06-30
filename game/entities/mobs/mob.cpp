#include "mob.h"

#include "game/entities/bullet.h"
#include "core/server.h"
#include "game/gamemp.h"
#include "utils/vector2d.h"
#include "qmath.h"
#include "math.h"

extern Server* server;
Mob::Mob(QPointF position, float width, float height, MobType type) : Entity(position, EntityType::MOB)
{
    collisionBox.setRect(position.x(), position.y(), width, height);
    this->type  = type;
    collidingWithTile = false;
}

Mob::~Mob()
{
}

void Mob::update()
{
}

bool Mob::isColliding(Entity* entity)
{
    if(entity->getType() == EntityType::MOB)
    {
        Mob *mob = (Mob*)entity;
        if(collisionBox.intersects(mob->getCollisionBox())) return true;
    }

    return false;
}

bool Mob::isColliding(Tile* tile)
{
    if(collisionBox.intersects(tile->getCollisionBox())) return true;

    return false;
}

void Mob::tileCollision()
{
    const static float MIN_DIST_X = MOB_WIDTH/2.0f + TILE_SIZE/2.0f; // buffer otherwise spaz attack
    const static float MIN_DIST_Y = MOB_HEIGHT/2.0f + TILE_SIZE/2.0f;
    collidingWithTile = false;
    for(Tile t : server->getGame()->getLevel()->getTiles())
    {
        if(isColliding(&t))
        {
            collidingWithTile = true;
            float x = this->getPosition().x();
            float y = this->getPosition().y();
            QPointF center(getCenterPosition());
            QPointF tileCenter(t.getCenterPosition());
            QPointF distance = center - tileCenter;

            float xDepth = MIN_DIST_X - qAbs(distance.x());
            float yDepth = MIN_DIST_Y - qAbs(distance.y());

            if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f))
            {
                 if (distance.x() < 0.0f) setPosition(x-xDepth, y);
                 else setPosition(x+xDepth, y);
            }
            else
            {
                if (distance.y() < 0.0f) setPosition(x, y-yDepth);
                else setPosition(x, y+yDepth);
            }
        }
    }
}

void Mob::mobCollision()
{
    static const float MIN_DIST_X = MOB_WIDTH;
    static const float MIN_DIST_Y = MOB_HEIGHT;
    for(Entity* e : *server->getGame()->getEntities())
    {
        if(e->getType() == EntityType::MOB)
        {
            Mob* mob  =(Mob*)e;
            if(isColliding(e) && mob != this)
            {
                float x = this->getPosition().x();
                float y = this->getPosition().y();

                QPointF center(getCenterPosition());
                QPointF mobCenter(mob->getCenterPosition());
                QPointF distance = center - mobCenter;

                float xDepth = MIN_DIST_X - qAbs(distance.x());
                float yDepth = MIN_DIST_Y - qAbs(distance.y());

                xDepth /= 2.0f;
                yDepth /= 2.0f;

                if(qMax(xDepth, 0.0f) < qMax(yDepth, 0.0f))
                {
                    if(distance.x() < 0.0f)
                    {
                        setPosition(x-xDepth, y);

                        x = mob->getPosition().x();
                        y = mob->getPosition().y();
                        mob->setPosition(x+xDepth, y);
                    }
                    else
                    {
                        setPosition(x+xDepth, y);

                        x = mob->getPosition().x();
                        y = mob->getPosition().y();
                        mob->setPosition(x-xDepth, y);
                    }
                }
                else
                {
                    if(distance.y() < 0.0f)
                    {
                        setPosition(x, y-yDepth);

                        x = mob->getPosition().x();
                        y = mob->getPosition().y();
                        mob->setPosition(x, y+yDepth);
                    }
                    else
                    {
                        setPosition(x, y+yDepth);

                        x = mob->getPosition().x();
                        y = mob->getPosition().y();
                        mob->setPosition(x, y-yDepth);
                    }
                }
            }
        }
    }
}

void Mob::setPosition(QPointF position)
{
    setPosition(position.x(), position.y());
}

void Mob::setPosition(float x, float y)
{
    Entity::setPosition(x, y);
    collisionBox.setX(x);
    collisionBox.setY(y);
    collisionBox.setWidth(MOB_WIDTH);
    collisionBox.setHeight(MOB_HEIGHT);
}

QPointF Mob::getCenterPosition()
{
    float x = this->getPosition().x() + MOB_WIDTH/2.0f;
    float y = this->getPosition().y() + MOB_HEIGHT/2.0f;

    return QPointF(x, y);
}
