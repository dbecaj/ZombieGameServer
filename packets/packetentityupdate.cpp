#include "packetentityupdate.h"

#include <QPointF>

#include "core/server.h"
#include "game/gamemp.h"
#include "game/entities/mobs/zombie.h"
#include "game/entities/mobs/scientist.h"

extern Server* server;
PacketEntityUpdate::PacketEntityUpdate(QByteArray data) : Packet(data)
{
    valid = validate();
}

PacketEntityUpdate::PacketEntityUpdate(QList<Zombie *> zombies, QList<Scientist *> scientists) : Packet(PacketType::ENTITY_UPDATE)
{
    writeEntityUpdate(zombies, scientists);
}

PacketEntityUpdate::PacketEntityUpdate() : Packet(PacketType::ENTITY_UPDATE)
{
    valid = false;
}

void PacketEntityUpdate::writeEntityUpdate(QList<Zombie *> zombies, QList<Scientist *> scientists)
{
    QString zombiesData = ""; // [zombieId]/[x]/[y]/[lives]*
    for(Zombie* z : server->getGame()->zombies)
    {
        zombiesData += QString::number(z->getId()) + "/" + QString::number(z->getPosition().x()) + "/" +
                       QString::number(z->getPosition().y()) + "/" + QString::number(z->getLives()) + "*";
    }
    if(zombiesData.length() != 0) zombiesData = zombiesData.mid(0, zombiesData.length() - 1);

    QString scientistsData = ""; // [scientistId]/[x]/[y]/[dir]*
    for(Scientist* s : server->getGame()->scientists)
    {
        scientistsData += QString::number(s->getId()) + "/" + QString::number(s->getPosition().x()) + "/" +
                          QString::number(s->getPosition().y()) + "/" + QString::number(s->getDirection()) + "*";
    }
    if(scientistsData.length() != 0) scientistsData = scientistsData.mid(0, scientistsData.length() - 1);

    QString data = zombiesData + "," + scientistsData;
    this->writeData(data);
}

bool PacketEntityUpdate::validate()
{
    valid = false;

    QString data = getData();
    QStringList splitData = data.split(',');
    if(splitData.length() != 2)
    {
        errorMsg = "ENTITY_UPDATE packet error: The packet format is the wrong format!";
        return false;
    }

    if(splitData[0].split('*').length() != 0 && splitData[0] != "")
    {
        if(splitData[0].split('*')[0].split('/').length() != 4)
        {
            errorMsg = "ENTITY_UPDATE packet error: The packet format for zombies is the wrong format!";
            return false;
        }
    }

    if(splitData[1].split('*').length() != 0 && splitData[1] != "")
    {
        if(splitData[1].split('*')[0].split('/').length() != 4)
        {
            errorMsg = "ENTITY_UPDATE packet error: The packet format for scientists is the wrong format!";
            return false;
        }
    }

    valid = true;
    return true;
}
