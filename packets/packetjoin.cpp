#include "packetjoin.h"

PacketJoin::PacketJoin(QByteArray data) : Packet(data)
{
    valid = validate();
}

PacketJoin::PacketJoin(QString username) : Packet(PacketType::JOIN)
{
    writeJoin(username);
}

PacketJoin::PacketJoin() : Packet(PacketType::JOIN)
{
    valid = false;
}

void PacketJoin::writeJoin(QString username)
{
    this->writeData(username);
}

bool PacketJoin::validate()
{
    valid = false;

    if(getData().contains(','))
    {
        errorMsg = "JOIN packet error: The packet contains ','!";
        return false;
    }

    valid = true;
    return true;
}

QString PacketJoin::getUsername()
{
    if(!valid) return "";

    return getData();
}
