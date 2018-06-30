#ifndef PACKETENTITYUPDATE_H
#define PACKETENTITYUPDATE_H

#include "packet.h"

class Zombie;
class Scientist;

class PacketEntityUpdate : public Packet
{
public:
    PacketEntityUpdate(QByteArray data);
    PacketEntityUpdate(QList<Zombie*> zombies, QList<Scientist*> scientists);
    PacketEntityUpdate();

    void writeEntityUpdate(QList<Zombie *> zombies, QList<Scientist *> scientists);
    bool validate();
};

#endif // PACKETENTITYUPDATE_H
