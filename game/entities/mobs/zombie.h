#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "mob.h"
#include "utils/vector2d.h"

class Zombie : public Mob
{
    static long idIncrement;

    long id;
    Mob* target;
    float speed;
    int lives;
    bool lockTarget;
public:
    Zombie(QPointF position, long id = -1);
    ~Zombie();
    void update();
    Mob* scanForTarget();
    void sendChangeTarget();
    void mobCollision();
    void checkIfHitTarget();
    void damage(int ammount);

    long getId() { return id; }
    QPointF getTargetLocation();
    void setTarget(Mob* mob);
    Mob* getTarget() { return target; }
    bool hasTarget();
    int getLives() { return lives; }
    void setLives(int lives) { this->lives = lives; }
};

#endif // ZOMBIE_H
