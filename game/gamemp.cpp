#include "gamemp.h"

#include <ctime>

#include "entities/mobs/player.h"
#include "entities/mobs/scientist.h"
#include "entities/mobs/zombie.h"
#include "core/server.h"
#include "core/client.h"
#include "entities/mobs/gun.h"
#include "packets/packetplayerpos.h"
#include "packets/packetfirebullet.h"
#include "packets/packeterror.h"
#include "packets/packetkick.h"
#include "packets/packetentitydata.h"
#include "packets/packetentityupdate.h"

extern Server* server;
GameMP::GameMP(QObject *parent) : QObject(parent)
{
    gameLoopTimer.setInterval((1/FPS)*1000);
    frameCounter = 0;
}

GameMP::~GameMP()
{
    for(Player* p : players) delete p;
    for(Zombie* z : zombies) delete z;
    for(Scientist* s : scientists) delete s;
}

bool GameMP::init()
{
    bool found;
    QString levelName = server->getConfig()->read("LevelName", &found);
    if(!found || levelName == "")
    {
        server->getLogger()->output("LevelName was not found or is empty in config.txt", MessageType::ERROR);
        return false;
    }
    server->getLogger()->output("Loading level: " + levelName);
    level.init(QCoreApplication::applicationDirPath() + "/levels/" + levelName);
    if(!level.loadLevel())
    {
        server->stopServer("Level load failed");

        return false;
    }

    this->connect(server, SIGNAL(udpPacketReceived(QHostAddress,int,QByteArray)), this, SLOT(processUdpPacket(QHostAddress,int,QByteArray)));
    this->connect(&gameLoopTimer, SIGNAL(timeout()), this, SLOT(gameLoop()));
    gameLoopTimer.start();

    wantToReset = false;

    return true;
}

void GameMP::resetGame()
{
    frameCounter = 0;
    gameLoopTimer.stop();

    cleanUp();

    QList<Scientist*> scientists;
    QList<Zombie*> zombies;
    for(QPointF pos : level.getScientists())
    {
        static std::mt19937 randEngine(time(0));

        std::uniform_int_distribution<int> randGender(0, 1);
        ScientistGender gender = ScientistGender::MALE;
        if(randGender(randEngine) == 1) gender = ScientistGender::FEMALE;
        scientists.push_back(new Scientist(gender, pos));
    }

    for(QPointF pos : level.getZombies())
    {
        zombies.push_back(new Zombie(pos));
    }

    for(Player* player : players)
    {
        player->setPosition(server->getGame()->getLevel()->playerStartPos);
        player->setInfected(false);
    }

    PacketEntityData* packet = new PacketEntityData(server->getGame()->players, zombies, scientists);
    sendTcpPacketToAllPlayers(packet);
    delete packet;

    for(Scientist* s : scientists)
        server->getGame()->addScientist(s);
    for(Zombie* z : zombies)
        server->getGame()->addZombie(z);

    gameLoopTimer.start();

    wantToReset = false;

    server->getLogger()->output("The game has restarted!");
}

void GameMP::cleanUp()
{
    for(Scientist* s : scientists)
        if(removeScientist(s)) delete s;
        else server->getLogger()->output("can't delete scientist");
    for(Zombie* z : zombies)
        if(removeZombie(z)) delete z;
        else server->getLogger()->output("can't delete zombie");

    entities.clear();
    scientists.clear();
    zombies.clear();

    for(Player* player : players)
        entities.push_back(player);
}

void GameMP::gameLoop()
{
    if(wantToReset) resetGame();
    for(int i = 0; i < wantToKick.length(); i++)
    {
        kickPlayer(wantToKick.at(i));
        wantToKick.removeAt(i);
    }
    /*if(frameCounter % 300 == 0)
    {
        PacketEntityUpdate packet(zombies, scientists);
        sendUdpPacketToAllPlayers(&packet);
    } this is experimental*/

    update();
    handleCollision();
    frameCounter++;
}

void GameMP::update()
{
    for(int i = 0; i < entities.length(); i++)
        entities.at(i)->update();
}

void GameMP::handleCollision()
{
    for(int i = 0; i < zombies.length(); i++)
        zombies.at(i)->mobCollision();
    for(Scientist* s : scientists)
        s->mobCollision();

    for(Zombie* z : zombies)
        z->tileCollision();
    for(Scientist* s : scientists)
        s->tileCollision();
}

void GameMP::processUdpPacket(QHostAddress ip, int port, QByteArray data)
{
    QString ipAddress = ip.toString();
    if(ipAddress != "" && ipAddress.at(0) == ':') // if we get IPv6 (::ffff:) we just take the IPv4 part at the end
        ipAddress = ipAddress.mid(7);

    if(!getPlayer(QHostAddress(ipAddress)))
    {
        PacketError errorPacket(QString::fromStdString("You are not joined in this server!"));
        server->sendUdpPacket(ip, port, &errorPacket);
        return;
    }

    if(Packet::determinePacketType(data) == PacketType::PLAYER_POS)
    {
        PacketPlayerPos playerPosPacket(data);

        Player* player = getPlayer(playerPosPacket.getUsername());
        if(!player)
        {
            QString error = "You are not in the server " + playerPosPacket.getUsername();
            PacketError errorPacket(error);
            server->sendUdpPacket(ip, port, &errorPacket);
            return;
        }

        if(!player->getClient()->hasUdpPort()) player->getClient()->setUdpPort(port); // if client does not have udp port yet (ps: if you figure out a better way u know ;))
        player->updatePosition(playerPosPacket.getPosition(), playerPosPacket.getDegrees()); // update this player pos and send that to others
    }
    else if(Packet::determinePacketType(data) == PacketType::FIRE_BULLET)
    {
        PacketFireBullet fireBulletPacket(data);
        if(!fireBulletPacket.isValid())
        {
            server->getLogger()->output(fireBulletPacket.getErrorMsg());
            return;
        }

        Player* player = getPlayer(fireBulletPacket.getUsername());
        if(!player)
        {
            PacketError errorPacket(QString::fromStdString("You are not joined in this server!"));
            server->sendUdpPacket(ip, port, &errorPacket);
            return;
        }
        else
        {
            Bullet* bullet = new Bullet(fireBulletPacket.getBulletPos(), (Direction)fireBulletPacket.getBulletDir());
            player->getGun()->addBullet(bullet);
        }
        sendUdpPacketToAllPlayers(&fireBulletPacket, fireBulletPacket.getUsername());
    }
}

bool GameMP::hasPlayerWithUsername(QString username)
{
    for(Player* p : players)
        if(p->getUsername() == username) return true;

    return false;
}

bool GameMP::addPlayer(Player *player)
{
    if(hasPlayerWithUsername(player->getUsername())) return false;

    players.push_back(player);
    entities.push_back(player);
    player->getClient()->connectToPlayer(player);
    server->getLogger()->output(player->getUsername() + " has joined!");

    return true;
}

void GameMP::addScientist(Scientist *scientist)
{
    scientists.push_back(scientist);
    entities.push_back(scientist);
}

void GameMP::addZombie(Zombie* zombie)
{
    entities.push_back(zombie);
    zombies.push_back(zombie);
}

bool GameMP::removePlayer(QString username)
{
    for(int i = 0; i < players.size(); i++)
    {
        if(players[i]->getUsername() == username)
        {
            entities.removeOne(players[i]);
            delete players[i];
            players.removeAt(i);
            return true;
        }
    }

    return false;
}

QString GameMP::removePlayer(Client *client)
{
    QString username = "";
    for(int i = 0; i < players.size(); i++)
    {
        if(players[i]->getClient() == client)
        {
            username = players[i]->getUsername();
            entities.removeOne(players[i]);
            delete players[i];
            players.removeAt(i);
            return username;
        }
    }

    return username;
}

void GameMP::infectPlayer(QString username)
{
    Player* player = getPlayer(username);
    if(!player) return;

    player->setInfected(true);
    entities.removeOne(player);
}

bool GameMP::removeZombie(Zombie *zombie)
{
    if(!zombies.removeOne(zombie))
    {
        server->getLogger()->output("can't delete zombie " + QString::number(zombie->getId()) + " from zombies!");
        return false;
    }
    else if(!entities.removeOne(zombie))
    {
        server->getLogger()->output("can't delete zombie " + QString::number(zombie->getId()) + " from entities!");
        return false;
    }

    return true;
}

bool GameMP::removeScientist(Scientist *scientist)
{
    return (scientists.removeOne(scientist) && entities.removeOne(scientist));
}

bool GameMP::kickPlayer(QString username)
{
    Player* player = getPlayer(username);
    if(!player) return false;

    PacketKick kickPacket(username);
    sendTcpPacketToAllPlayers(&kickPacket);
    player->getClient()->disconnectFromPlayerHandle();
    removePlayer(username);
    server->getLogger()->output(username + " has been kicked!");

    return true;
}

bool GameMP::doesPlayerExist(QString username)
{
    for(Player* p : players)
    {
        if(p->getUsername() == username) return true;
    }

    return false;
}

Player* GameMP::getPlayer(QHostAddress ip)
{
    Player* player = nullptr;
    for(Player* p : players)
    {
        if(p->getClient()->getIP() == ip)
        {
            player = p;
            return player;
        }
    }

    return player;
}

Player* GameMP::getPlayer(QString username)
{
    Player* player = nullptr;
    for(Player* p : players)
    {
        if(p->getUsername() == username)
        {
            player = p;
            return player;
        }
    }

    return player;
}

Zombie* GameMP::getZombie(long id)
{
    Zombie* zombie = nullptr;
    for(Zombie* z : zombies)
    {
        if(z->getId() == id)
        {
            zombie = z;
            return zombie;
        }
    }

    return zombie;
}

Scientist* GameMP::getScientist(long id)
{
    Scientist* scientist = nullptr;
    for(Scientist* s : scientists)
    {
        if(s->getId() == id)
        {
            scientist = s;
            return scientist;
        }
    }

    return scientist;
}

QList<Mob*> GameMP::getMobs()
{
    QList<Mob*> mobs;
    for(Player* p : players)
    {
        if(p->isInfected()) continue;
        mobs.push_back(p);
    }
    for(Zombie* z : zombies) mobs.push_back(z);
    for(Scientist* s : scientists) mobs.push_back(s);

    return mobs;
}

long GameMP::getNumOfConnectedPlayers()
{
    return players.length();
}

void GameMP::sendUdpPacketToAllPlayers(Packet *packet, QString exept)
{
    for(Player* p : players)
    {
        if(p->getUsername() != exept)
        {
            server->sendUdpPacket(p->getClient()->getIP(), p->getClient()->getPort(), packet); // sending using udp protocol
        }
    }
}

void GameMP::sendTcpPacketToAllPlayers(Packet *packet, QString exept)
{
    for(Player* p : players)
    {
        if(p->getUsername() != exept) p->sendPacket(packet);
    }
}
