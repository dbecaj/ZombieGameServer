#include "zombie.h"

#include "core/server.h"
#include "game/gamemp.h"
#include "player.h"
#include "scientist.h"
#include "utils/vector2d.h"
#include "packets/packetzombiechangetarget.h"
#include "packets/packetzombiehit.h"
#include "packets/packetinfected.h"

extern Server* server;
Zombie::Zombie(QPointF position, long id) : Mob(position, MOB_WIDTH, MOB_HEIGHT, MobType::ZOMBIE) // remember to send the target after you create them
{
    target = nullptr;
    speed = 7.0f;
    lives = 3;
    lockTarget = false;
    if(id >= 0) this->id = id;
    else this->id = idIncrement++;
}

Zombie::~Zombie()
{
}

long Zombie::idIncrement = 0;

void Zombie::update()
{
    if(!lockTarget)
    {
        Mob* scanTarget = scanForTarget();
        if(target != scanTarget && scanTarget)
        {
            target = scanTarget;
            sendChangeTarget();
        }
        else if(!scanTarget) target = nullptr;
    }

    if(!target) return;

    Vector2D zombieCenter(this->getCenterPosition());
    Vector2D targetCenter(target->getCenterPosition());
    Vector2D targetLocation = zombieCenter - targetCenter;

    QPointF newPos(this->getPosition().x() - (targetLocation.normalized().getX() * speed),
                   this->getPosition().y() - (targetLocation.normalized().getY() * speed));
    this->setPosition(newPos);
}

void Zombie::setTarget(Mob *mob)
{
    target = mob;
    sendChangeTarget();
    lockTarget = true;
}

void Zombie::damage(int ammount)
{
    lives -= ammount;
    if(lives < 0) lives = 0;
}

bool Zombie::hasTarget()
{
    if(target) return true;
    return false;
}

QPointF Zombie::getTargetLocation()
{
    if(!target) return QPointF(-1, -1);

    return target->getPosition();
}

void Zombie::sendChangeTarget()
{
    if(!target) return;

    PacketZombieChangeTarget changeTargetPacket(id, this->getPosition(), target);
    server->getGame()->sendTcpPacketToAllPlayers(&changeTargetPacket);
}

Mob* Zombie::scanForTarget()
{
    Mob* mob = nullptr;
    if(server->getGame()->scientists.size() != 0) mob = server->getGame()->scientists[0];
    else if(server->getGame()->players.size() != 0)
    {
        for(int i = 0; i < server->getGame()->players.length(); i++)
        {
            if(!server->getGame()->players.at(i)->isInfected())
            {
                mob = server->getGame()->players.at(i);
                break;
            }
        }
    }

    if(!mob) return nullptr;

    Vector2D zombieCenter(this->getCenterPosition());
    Vector2D targetCenter(mob->getCenterPosition());
    for(Player* p : server->getGame()->players)
    {
        if(p->isInfected()) continue;
        Vector2D playerCenter(p->getCenterPosition());
        if((zombieCenter - targetCenter).length() > (zombieCenter - playerCenter).length())
        {
            mob = p;
            targetCenter = Vector2D(mob->getCenterPosition());
        }
    }

    for(Scientist* s : server->getGame()->scientists)
    {
        Vector2D scientistCenter(s->getCenterPosition());
        if((zombieCenter - targetCenter).length() > (zombieCenter - scientistCenter).length())
        {
            mob = s;
            targetCenter = Vector2D(mob->getCenterPosition());
        }
    }

    return mob;
}

void Zombie::checkIfHitTarget()
{
    for(Mob* mob : server->getGame()->getMobs())
    {
        if(!this->isColliding(mob)) continue;

        if(mob->getMobType() == MobType::ZOMBIE) continue;

        if(mob->getMobType() == MobType::SCIENTIST)
        {
            Scientist* scientist = (Scientist*)mob;
            // We remove the scientists from the entities that we use for updating and from the list
            Zombie* newZombie = new Zombie(scientist->getPosition());
            server->getGame()->addZombie(newZombie);

            PacketZombieHit zombieHitPacket(this, mob, newZombie->getId());
            server->sendPacketToAllClients(&zombieHitPacket, Protocol::TCP);

            server->getGame()->removeScientist(scientist);
            delete scientist;
        }
        else if(mob->getMobType() == MobType::PLAYER)
        {
            Player* player = (Player*)mob;

            Zombie* newZombie = new Zombie(player->getPosition());
            server->getGame()->addZombie(newZombie);

            PacketInfected packetInfected(player->getUsername(), newZombie->getId());
            server->sendPacketToAllClients(&packetInfected, Protocol::TCP);

            server->getGame()->infectPlayer(player->getUsername());
        }
    }
}

void Zombie::mobCollision()
{
    checkIfHitTarget();
    Mob::mobCollision();
}
