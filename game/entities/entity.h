#ifndef ENTITY_H
#define ENTITY_H

#include <QPointF>

#include "game/tile.h"

enum class EntityType
{
    MOB,
    BULLET
};

class Entity
{
    QPointF position;
    EntityType type;
public:
    Entity(QPointF position, EntityType type);

    virtual bool isColliding(Entity* entity) = 0;
    virtual bool isColliding(Tile* tile) = 0;
    virtual QPointF getCenterPosition() = 0;
    virtual void update() = 0;

    QPointF getPosition() { return position; }
    virtual void setPosition(float x, float y) { position = QPointF(x, y); }
    virtual void setPosition(QPointF position) { this->position = position; }
    EntityType getType() { return type; }
};

#endif // ENTITY_H
