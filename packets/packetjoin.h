#ifndef PACKETJOIN_H
#define PACKETJOIN_H

#include "packet.h"

class PacketJoin : public Packet
{
public:
    PacketJoin(QByteArray data);
    PacketJoin(QString username);
    PacketJoin();

    void writeJoin(QString username);
    bool validate();

    QString getUsername();
};

#endif // PACKETJOIN_H
