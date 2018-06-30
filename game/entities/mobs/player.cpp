#include "player.h"

#include "core/server.h"
#include "core/client.h"
#include "gun.h"
#include "packets/packetplayerpos.h"
#include "packets/packetbullethit.h"
#include "game/entities/mobs/zombie.h"

extern Server* server;
Player::Player(QString username, Client *client, QObject *parent) : QObject(parent),
                                                                    Mob(server->getGame()->getLevel()->playerStartPos,
                                                                        MOB_WIDTH, MOB_HEIGHT, MobType::PLAYER)
{
    this->username = username;
    degrees = 180.0f;
    gun = new Gun(this);
    infected = false;

    this->client = client;
    this->connect(gun, SIGNAL(mobHit(Mob*)), this, SLOT(mobHit(Mob*)));
}

void Player::mobHit(Mob *mob)
{
    if(mob->getMobType() == MobType::ZOMBIE)
    {
        Zombie* zombie = (Zombie*)mob;
        zombie->damage(1);
        PacketBulletHit bulletHitPacket(zombie->getId(), zombie->getLives(), username);
        server->sendPacketToAllClients(&bulletHitPacket, Protocol::TCP);

        if(zombie->getLives() == 0)
        {
            if(server->getGame()->removeZombie(zombie)) delete zombie;
            else server->getLogger()->output("Player mobHit() cannot removeZombie()!");
        }
    }
}

void Player::updatePosition(QPointF position, float degrees)
{
    this->setPosition(position);
    this->degrees = degrees;

    if(infected) return;
    PacketPlayerPos playerPosPacket(username, this->getPosition(), this->degrees);
    server->getGame()->sendUdpPacketToAllPlayers(&playerPosPacket, username); // send to all exept the player who sent it
}

void Player::update()
{
    gun->update();
}

void Player::mobCollision() // mob collision is handled by the player
{
}

void Player::tileCollision() // tile collision is handled by the player
{
}

void Player::sendPacket(Packet *packet) // Protocol has not been declared is rip
{
    client->sendPacket(packet, Protocol::TCP);
}
