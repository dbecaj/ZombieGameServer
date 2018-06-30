#include "packetshutdown.h"

PacketShutdown::PacketShutdown() : Packet(PacketType::SERVER_SHUTDOWN)
{
    valid = true;
}

bool PacketShutdown::validate()
{
    valid = true;
    return true;
}
