#ifndef GUN_H
#define GUN_H

#include <QList>
#include <QObject>

#include "game/entities/bullet.h"
#include "player.h"

class Gun : public QObject
{
    Q_OBJECT

    QList<Bullet*> bullets;

    void handleCollision();
    bool handleCollisionHelp(Bullet *bullet, int index);
    bool handleCollisionHelp(Tile tile, int index);
signals:
    void mobHit(Mob* mob);
public:
    Gun(QObject* parent = nullptr);
    void update();
    void addBullet(Bullet *bullet);
};

#endif // GUN_H
