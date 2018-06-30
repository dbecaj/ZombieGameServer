#ifndef PACKETZOMBIEHIT_H
#define PACKETZOMBIEHIT_H

#include "packet.h"
#include "game/entities/mobs/zombie.h"

class PacketZombieHit : public Packet
{
public:
    PacketZombieHit(QByteArray data);
    PacketZombieHit(Zombie* zombie, Mob* target, long newZombieId);
    PacketZombieHit();

    void writeZombieHit(Zombie* zombie, Mob* target, long newZombieId);
    bool validate();

    long getZombieId();
    long getNewZombieId();
    QPointF getZombiePosition();
    Mob* getHitTarget();
    QPointF getHitTargetPosition();
};

#endif // PACKETZOMBIEHIT_H
