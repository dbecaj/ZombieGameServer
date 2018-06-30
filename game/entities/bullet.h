#ifndef BULLET_H
#define BULLET_H

#include <QPointF>
#include <QObject>

#include "entity.h"
#include "mobs/mob.h"

#define BULLET_SIZE 10.0f

enum Direction
{
    UP = 0,
    UP_LEFT = 1,
    UP_RIGHT = 2,
    DOWN = 3,
    DOWN_LEFT = 4,
    DOWN_RIGHT = 5,
    LEFT = 6,
    RIGHT = 7
};

class Bullet : public QObject, public Entity
{
    Q_OBJECT
    Direction direction;
public:
    explicit Bullet(QPointF position, Direction direction, QObject* parent = nullptr);

    bool isColliding(Entity* entity);
    bool isColliding(Tile* tile);
    QPointF getCenterPosition();
    void update();
};

#endif // BULLET_H
