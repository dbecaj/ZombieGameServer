#include "packetinfected.h"

#include <QStringList>
#include <QDebug>

PacketInfected::PacketInfected(QByteArray data) : Packet(data)
{
    valid = validate();
}

PacketInfected::PacketInfected(QString playerName, long zombieId) : Packet(PacketType::INFECTED)
{
    writeKilled(playerName, zombieId);
}

PacketInfected::PacketInfected() : Packet(PacketType::INFECTED)
{
    valid = false;
}

void PacketInfected::writeKilled(QString playerName, long zombieId)
{
    this->writeData(playerName + "," + QString::number(zombieId));
}

bool PacketInfected::validate()
{
    valid = false;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    if(splitData.length() != 2)
    {
        errorMsg = "KILLED packet error: The packet format is wrong!";
        return false;
    }

    valid = true;
    return true;
}

QString PacketInfected::getUsername()
{
    if(!valid) return "";

    QString data = this->getData();
    QStringList splitData = data.split(",");
    return splitData[0];
}

long PacketInfected::getZombieId()
{
    if(!valid) return -1;

    QString data = this->getData();
    QStringList splitData = data.split(",");

    return splitData[1].toLong();
}
