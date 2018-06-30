#include "packetzombiehit.h"

#include <QStringList>

#include "game/entities/mobs/player.h"
#include "game/entities/mobs/scientist.h"
#include "core/server.h"
#include "game/gamemp.h"

extern Server* server;
PacketZombieHit::PacketZombieHit(QByteArray data) : Packet(data)
{
    valid = validate();
}

PacketZombieHit::PacketZombieHit(Zombie *zombie, Mob *target, long newZombieId) : Packet(PacketType::ZOMBIE_HIT)
{
    writeZombieHit(zombie, target, newZombieId);
}

PacketZombieHit::PacketZombieHit() : Packet(PacketType::ZOMBIE_HIT)
{
    valid = false;
}

void PacketZombieHit::writeZombieHit(Zombie *zombie, Mob *target, long newZombieId)
{
    QString data = "";
    data += QString::number(zombie->getId()) + "," + QString::number(zombie->getPosition().x()) + "," + QString::number(zombie->getPosition().y());
    data += "," + QString::number(target->getPosition().x()) + "," + QString::number(target->getPosition().y());
    if(target->getMobType() == MobType::PLAYER)
    {
        Player* player = (Player*)target;
        data += "," + QString::fromStdString("PLAYER") + "," + player->getUsername();
    }
    else if(target->getMobType() == MobType::SCIENTIST)
    {
        Scientist* scientist = (Scientist*)target;
        data += "," + QString::fromStdString("SCIENTIST") + "," + QString::number(scientist->getId());
    }
    data += "," + QString::number(newZombieId);

    this->writeData(data);
}

bool PacketZombieHit::validate()
{
    valid = false;

    QString data = this->getData();
    QStringList splitData = data.split(",");

    // [idZombie],[zombieX],[zombieY],[targetX],[targetY],[mobType],[idTarget/playerName]
    if(splitData.length() != 8)
    {
        errorMsg = "ZOMBIE_HIT packet error: The packet format is wrong!";
        return false;
    }

    valid = true;
    return true;
}

long PacketZombieHit::getZombieId()
{
    long id = -1;
    if(!valid) return id;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    id = splitData[0].toLong();

    return id;
}

long PacketZombieHit::getNewZombieId()
{
    long id = -1;
    if(!valid) return id;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    id = splitData[7].toLong();

    return id;
}

QPointF PacketZombieHit::getZombiePosition()
{
    QPointF pos(0.0f, 0.0f);
    if(!valid) return pos;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    float x = splitData[1].toFloat();
    float y = splitData[2].toFloat();

    return QPointF(x, y);
}

Mob* PacketZombieHit::getHitTarget()
{
    if(!valid) return nullptr;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    if(splitData[5] == "PLAYER")
    {
        Player* player = server->getGame()->getPlayer(splitData[6]);
        return player;
    }
    else if(splitData[5] == "SCIENTIST")
    {
        Scientist* scientist = server->getGame()->getScientist(splitData[6].toLong());
        return scientist;
    }

    return nullptr;
}

QPointF PacketZombieHit::getHitTargetPosition()
{
    QPointF pos(0.0f, 0.0f);
    if(!valid) return pos;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    float x = splitData[3].toFloat();
    float y = splitData[4].toFloat();

    return QPointF(x, y);
}
