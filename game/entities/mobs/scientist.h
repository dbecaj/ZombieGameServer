#ifndef SCIENTIST_H
#define SCIENTIST_H

#include "mob.h"

enum class ScientistGender
{
    MALE,
    FEMALE
};

class Scientist : public Mob
{
    static long idIncrement;

    long id;
    float speed;
    float direction;
    int frameTimer;
    ScientistGender gender;
public:
    Scientist(ScientistGender gender, QPointF position, long id = -1);
    Scientist(QPointF position, long id = -1);
    void update();
    void sendChangeDirection();
    void randomizeDirection();
    void changeAngle(float angle);

    long getId() { return id; }
    float getDirection() { return direction; }
    ScientistGender getGender() { return gender; }
};

#endif // SCIENTIST_H
