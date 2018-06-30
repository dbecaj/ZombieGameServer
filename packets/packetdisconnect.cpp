#include "packetdisconnect.h"

PacketDisconnect::PacketDisconnect(QByteArray data) : Packet(data)
{
    valid = validate();
}

PacketDisconnect::PacketDisconnect(QString username) : Packet(PacketType::DISCONNECT)
{
    writeDisconnect(username);
}

PacketDisconnect::PacketDisconnect() : Packet(PacketType::DISCONNECT)
{
    valid = false;
}

void PacketDisconnect::writeDisconnect(QString username)
{
    this->writeData(username);
}

bool PacketDisconnect::validate()
{
    if(data.contains(','))
    {
        errorMsg = "DISCONNECT packet error: The packet contains ','!";
        return false;
    }

    return true;
}

QString PacketDisconnect::getUsername()
{
    if(!valid) return "";

    return getData();
}
