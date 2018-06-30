#ifndef MOB_H
#define MOB_H

#include <QPointF>
#include <QRectF>

#include "game/tile.h"
#include "game/entities/entity.h"

#define MOB_WIDTH 100.0f * 0.574468f
#define MOB_HEIGHT 30.0f

enum class MobType
{
    PLAYER,
    ZOMBIE,
    SCIENTIST
};

class Mob : public Entity
{
    QRectF collisionBox;
    MobType type;
    bool collidingWithTile;
public:
    Mob(QPointF position, float width, float height, MobType type);
    virtual ~Mob();

    bool isColliding(Entity* entity);
    bool isColliding(Tile* tile);
    virtual void mobCollision();
    virtual void tileCollision();
    void setPosition(QPointF position);
    void setPosition(float x, float y);
    QPointF getCenterPosition();
    virtual void update();

    QRectF getCollisionBox() { return collisionBox; }
    MobType getMobType() { return type; }
    bool isCollidingWithTile() {  return collidingWithTile; }
};

#endif // MOB_H
