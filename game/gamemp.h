#ifndef GAMEMP_H
#define GAMEMP_H

#include <QObject>
#include <QTimer>
#include <QHostAddress>

#include "level.h"
#include "packets/packet.h"
#include "entities/entity.h"

#define FPS 60.0f

enum class GameState
{
    RUNNING,
    PAUSED,
    WIN,
    LOSE,
    OUT_OF_TIME,
    SERVER_STOP
};

class Player;
class Zombie;
class Scientist;
class Client;
class Mob;

class GameMP : public QObject
{
    Q_OBJECT

    GameState gameState;
    Level level;
    QList<Entity*> entities;
    long frameCounter;
private slots:
    void gameLoop();
    void processUdpPacket(QHostAddress ip, int port, QByteArray data);
public:
    explicit GameMP(QObject *parent = nullptr);
    ~GameMP();
    bool init();
    void update();
    void handleCollision();

    QTimer gameLoopTimer;

    QList<Player*> players;
    QList<Zombie*> zombies;
    QList<Scientist*> scientists;
    bool wantToReset;
    QList<QString> wantToKick;

    bool addPlayer(Player* player);
    void addScientist(Scientist* scientist);
    void addZombie(Zombie* zombie);
    bool removePlayer(QString username);
    QString removePlayer(Client* client); // returns the removed player name for convenience
    void infectPlayer(QString username);
    bool removeScientist(Scientist* scientist);
    bool removeZombie(Zombie* zombie);
    bool kickPlayer(QString username);
    bool doesPlayerExist(QString username);
    void cleanUp();
    void resetGame();
    bool hasPlayerWithUsername(QString username);

    void sendUdpPacketToAllPlayers(Packet* packet, QString exept = ""); // this will send a tcp packet
    void sendTcpPacketToAllPlayers(Packet* packet, QString exept = ""); // you get the idea

    Level* getLevel() { return &level; }
    Player* getPlayer(QHostAddress ip);
    Player* getPlayer(QString username);
    Zombie* getZombie(long id);
    Scientist* getScientist(long id);
    QList<Entity*>* getEntities() { return &entities; }
    QList<Mob*> getMobs();
    long getNumOfConnectedPlayers();
};

#endif // GAMEMP_H
