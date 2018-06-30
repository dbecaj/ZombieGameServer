#include "packetzombiechangetarget.h"

#include "game/entities/mobs/mob.h"
#include "game/entities/mobs/player.h"
#include "game/entities/mobs/scientist.h"
#include "core/server.h"
#include "game/gamemp.h"

extern Server* server;
PacketZombieChangeTarget::PacketZombieChangeTarget(QByteArray data) : Packet(data)
{
    valid = validate();
}

PacketZombieChangeTarget::PacketZombieChangeTarget(long zombieId, QPointF zombiePos, Mob *target) : Packet(PacketType::ZOMBIE_CHANGE_TARGET)
{
    writeChangeTarget(zombieId, zombiePos, target);
}

PacketZombieChangeTarget::PacketZombieChangeTarget() : Packet(PacketType::ZOMBIE_CHANGE_TARGET)
{
    valid = false;
}

void PacketZombieChangeTarget::writeChangeTarget(long zombieId, QPointF zombiePos, Mob *target)
{
    QString data = "";
    data += QString::number(zombieId) + "," + QString::number(zombiePos.x()) + "," + QString::number(zombiePos.y()) + ","; // transform to collision box pos
    if(target->getMobType() == MobType::PLAYER)
    {
        Player* player = (Player*)target;
        data += "PLAYER," + player->getUsername();
    }
    else if(target->getMobType() == MobType::SCIENTIST)
    {
        Scientist* scientist = (Scientist*)target;
        data += "SCIENTIST," + QString::number(scientist->getId());
    }

    this->writeData(data);
}

bool PacketZombieChangeTarget::validate()
{
    valid = false;

    QString data = this->getData();
    QStringList splitData = data.split(",");

    // [id],[x],[y],[mobType],[idTarget/playerName]
    if(splitData.length() != 5)
    {
        errorMsg = "ZOMBIE_CHANGE_TARGET packet error: The packet format is wrong!";
        return false;
    }

    valid = true;
    return true;
}

long PacketZombieChangeTarget::getZombieId()
{
    long id = -1;
    if(!valid) return id;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    id = splitData[0].toLong();

    return id;
}

QPointF PacketZombieChangeTarget::getZombiePosition()
{
    QPointF pos(0.0f, 0.0f);
    if(!valid) return pos;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    float x = splitData[1].toFloat();
    float y = splitData[2].toFloat();

    return QPointF(x, y);
}

Mob* PacketZombieChangeTarget::getTarget()
{
    if(!valid) return nullptr;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    if(splitData[3] == "PLAYER")
    {
        Player* player = server->getGame()->getPlayer(splitData[4]);
        return player;
    }
    else if(splitData[3] == "SCIENTIST")
    {
        Scientist* scientist = server->getGame()->getScientist(splitData[4].toLong());
        return scientist;
    }
}

QString PacketZombieChangeTarget::getTargetId()
{
    QString id = "";
    if(!valid) return id;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    id = splitData[4];

    return id;
}
