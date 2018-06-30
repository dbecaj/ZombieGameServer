#ifndef PACKETENTITYDATA_H
#define PACKETENTITYDATA_H

#include <QList>

#include "packet.h"
#include "game/entities/mobs/player.h"
#include "game/entities/mobs/scientist.h"
#include "game/entities/mobs/zombie.h"

class PacketEntityData : public Packet
{
    QString usernames;
    QString zombies;
    QString scientists;
public:
    PacketEntityData(QByteArray data);
    PacketEntityData(QList<Player*> players, QList<Zombie*> zombies, QList<Scientist*> scientists);
    PacketEntityData();

    void writeEntityData(QList<Player*> players, QList<Zombie*> zombies, QList<Scientist*> scientists);
    bool validate();

    QList<Player*> getPlayers();
    QList<Zombie*> getZombies();
    QList<Scientist*> getScientists();
};

#endif // PACKETENTITYDATA_H
