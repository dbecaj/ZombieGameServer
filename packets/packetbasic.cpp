#include "packetbasic.h"

PacketBasic::PacketBasic(QString message) : Packet(PacketType::BASIC)
{
    valid = true;

    this->writeBasic(message);
}

bool PacketBasic::validate()
{
    valid = true;
    return true;
}

void PacketBasic::writeBasic(QString message)
{
    this->writeData(message);
}
