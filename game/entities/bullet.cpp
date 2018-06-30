#include "bullet.h"

#include <qmath.h>

#include "core/server.h"

extern Server* server;
Bullet::Bullet(QPointF position, Direction direction, QObject *parent) : QObject(parent),
                                                    Entity(position, EntityType::BULLET)
{
    this->direction = direction;
}

bool Bullet::isColliding(Entity *entity)
{
    if(entity->getType() == EntityType::BULLET)
    {
        Bullet* bullet = (Bullet*)entity;
        float a = qAbs(getCenterPosition().y() - bullet->getCenterPosition().y());
        float b = qAbs(getCenterPosition().x() - bullet->getCenterPosition().x());
        float c = qSqrt((qPow(a, 2) + qPow(b, 2))); // pitagorov izrek da dobimo razdaljo

        if(c < BULLET_SIZE) return true; // TODO get efficiant way of getting entity width height... UPDATE: it will have to do
    }
    else if(entity->getType() == EntityType::MOB)
    {
        // here we will treat the bullet like a square for convenience
        Mob* mob = (Mob*)entity;
        if(mob->getMobType() == MobType::PLAYER) return false;
        QPointF bulletPos = this->getPosition(); // for some reason I get "does not have a class type if I call getPosition() directly
        QRectF bulletBox(bulletPos.x(), bulletPos.y(), BULLET_SIZE, BULLET_SIZE);
        QRectF mobBox = mob->getCollisionBox();

        if(bulletBox.intersects(mobBox)) return true; // yey QT has collision :3
    }

    return false;
}

bool Bullet::isColliding(Tile *tile)
{
    // again better if we just treat bullet like a square + we don't need precise collision
    QPointF bulletPos = this->getPosition();
    QRectF bulletBox(bulletPos.x(), bulletPos.y(), BULLET_SIZE, BULLET_SIZE);

    if(bulletBox.intersects(tile->getCollisionBox())) return true;

    return false;
}

QPointF Bullet::getCenterPosition()
{
    QPointF bulletPos = this->getPosition();
    float x = bulletPos.x() + BULLET_SIZE/2.0f;
    float y = bulletPos.y() + BULLET_SIZE/2.0f;

    return QPointF(x, y);
}

void Bullet::update()
{
    float x = this->getPosition().x();
    float y = this->getPosition().y();
    switch(direction)
    {
        case Direction::UP:
        {
            this->setPosition(x, y - 15.0f);
            break;
        }
        case Direction::UP_LEFT:
        {
            this->setPosition(x - 15.0f, y - 15.0f);
            break;
        }
        case Direction::UP_RIGHT:
        {
            this->setPosition(x + 15.0f, y - 15.0f);
            break;
        }
        case Direction::DOWN:
        {
            this->setPosition(x, y + 15.0f);
            break;
        }
        case Direction::DOWN_LEFT:
        {
            this->setPosition(x - 15.0f, y + 15.0f);
            break;
        }
        case Direction::DOWN_RIGHT:
        {
            this->setPosition(x + 15.0f, y + 15.0f);
            break;
        }
        case Direction::LEFT:
        {
            this->setPosition(x - 15.0f, y);
            break;
        }
        case Direction::RIGHT:
        {
            this->setPosition(x + 15.0f, y);
            break;
        }
    }
}
