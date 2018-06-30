#ifndef PACKETBASIC_H
#define PACKETBASIC_H

#include "packet.h"

class PacketBasic : public Packet
{
public:
    PacketBasic(QString message);

    bool validate();
    void writeBasic(QString message);
};

#endif // PACKETBASIC_H
