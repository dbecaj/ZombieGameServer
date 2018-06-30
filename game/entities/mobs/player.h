#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QHostAddress>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QObject>

#include "packets/packet.h"
#include "mob.h"

class Gun;
class Client;

class Player : public QObject, public Mob
{
    Q_OBJECT

    QString username;
    Client* client;
    float degrees;
    Gun* gun;
    bool infected;
private slots:
    void mobHit(Mob* mob);
public:
    explicit Player(QString username, Client* client, QObject* parent = nullptr);

    void update();
    void mobCollision();
    void tileCollision();

    QString getUsername() { return username; }
    void sendPacket(Packet* packet);
    void updatePosition(QPointF position, float degrees); // this sets and sends the new position to all other players
    Client* getClient() { return client; }
    Gun* getGun() { return gun; }
    void setInfected(bool infected) { this->infected = infected; }
    bool isInfected() { return infected; }
};

#endif // PLAYER_H
