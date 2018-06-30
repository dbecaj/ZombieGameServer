#ifndef PACKETSHUTDOWN_H
#define PACKETSHUTDOWN_H

#include "packet.h"

class PacketShutdown : public Packet
{
public:
    PacketShutdown();

    bool validate();
};

#endif // PACKETSHUTDOWN_H
