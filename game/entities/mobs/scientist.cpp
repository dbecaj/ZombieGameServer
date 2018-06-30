#include "scientist.h"

#include <random>
#include <ctime>
#include <qmath.h>

#include "core/server.h"
#include "game/gamemp.h"
#include "packets/packet.h"
#include "packets/packetscientistchangedir.h"

extern Server* server;
Scientist::Scientist(ScientistGender gender, QPointF position, long id) : Mob(position, MOB_WIDTH, MOB_HEIGHT, MobType::SCIENTIST)
{
    speed = 8.0f;
    frameTimer = 0;
    if(id >= 0) this->id = id;
    else this->id = idIncrement++;

    randomizeDirection();

    this->gender = gender;
}

Scientist::Scientist(QPointF position, long id) : Mob(position, MOB_WIDTH, MOB_HEIGHT, MobType::SCIENTIST)
{
    speed = 8.0f;
    frameTimer = 0;
    if(id >= 0) this->id = id;
    else this->id = idIncrement++;

    randomizeDirection();

    std::mt19937 randEngine(time(nullptr));
    std::uniform_int_distribution<int> randGender(0, 1);
    if(randGender(randEngine)) gender = ScientistGender::MALE;
    else gender = ScientistGender::FEMALE;
}

long Scientist::idIncrement = 0;

void Scientist::update()
{
    if(frameTimer >= 50 || this->isCollidingWithTile())
    {
        randomizeDirection();
        sendChangeDirection();
        frameTimer = 0;
    }

    QPointF pos = this->getPosition();
    float rad = qDegreesToRadians(direction);
    this->setPosition(pos.x() + qCos(rad) * speed, pos.y() + qSin(rad) * speed);
    frameTimer++;
}

void Scientist::randomizeDirection()
{
    static std::mt19937 randEngine(time(nullptr));
    std::uniform_real_distribution<float> randDirection(0.0f, 360.0f);
    direction = randDirection(randEngine);
}

void Scientist::changeAngle(float angle)
{
    direction = angle;
}

void Scientist::sendChangeDirection()
{
    PacketScientistChangeDir changeDirectionPacket(id, this->getPosition(), direction);
    server->getGame()->sendUdpPacketToAllPlayers(&changeDirectionPacket);
}
