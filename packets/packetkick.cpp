#include "packetkick.h"

PacketKick::PacketKick(QByteArray data) : Packet(data)
{
    valid = validate();
}

PacketKick::PacketKick(QString username) : Packet(PacketType::KICK)
{
    writeKick(username);
}

PacketKick::PacketKick() : Packet(PacketType::KICK)
{
    valid = false;
}

void PacketKick::writeKick(QString username)
{
    this->writeData(username);
}

bool PacketKick::validate()
{
    valid = false;

    if(data.contains(','))
    {
        errorMsg = "KICK packet error: The packet contains a ','!";
        return false;
    }

    valid = true;
    return true;
}

QString PacketKick::getUsername()
{
    if(!valid) return "";

    return this->getData();
}
