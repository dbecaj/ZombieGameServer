#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork>
#include <QObject>
#include <QList>
#include <string>

#include "logger.h"
#include "config.h"
#include "game/entities/mobs/player.h"
#include "packets/packet.h"
#include "game/gamemp.h"

#define SERVER_PORT 6336
#define CLIENT_PORT 6337

enum class Protocol
{
    TCP,
    UDP
};

class Client;

class Server : public QObject
{
    Q_OBJECT

    QString serverName;
    QHostAddress serverIP;

    Config config;
    QUdpSocket udpSocket;
    QTcpServer tcpServer;
    Logger logger;
    GameMP* game;
    QList<Client*> clients;
    long maxPlayers;
private slots:
    void processUdpPendingDatagrams(); // Process the recived message/packet first and then passes it down the line
    void acceptNewConnection(); // This gets called when new client tryes to connect to tcpServer
    void acceptNewPlayer(QString username, Client *client); // This gets called when client gets the JOIN packet
    void removeClient(Client* client); // Gets called when client->disconnectClient() is called
signals:
    void udpPacketReceived(QHostAddress ip, int port, QByteArray data); // Is used to give data that we didn't process to those who want it down the line
public:
    explicit Server(QObject *parent = nullptr);
    ~Server();
    void stopServer(QString reason = ""); // If we call this the server will output the message for closing and close

    bool init(); // If this return false we don't just shutdown the program
    void bind(int port); // Inits the udpSocket and tcpServer
    QString findLocalIP(); // this is copied from QT example FortuneServer
    bool doesClientExist(QTcpSocket* socket); // Used so we don't get the same clients (compares IP and port of all clients tcpSocket)
    void dropAllClients();

    void sendUdpMessage(QHostAddress ip, int port, QByteArray datagram); // Send simple UDP message passing in raw data
    void sendUdpPacket(QHostAddress ip, int port, Packet* packet); // Send UDP packet with automatic header
    void sendPacketToAllClients(Packet* packet, Protocol protocol); // Send UDP packet to all clients

    Config* getConfig() { return &config; }
    Logger* getLogger() { return &logger; }
    GameMP* getGame() { return game; }
    QString getServerName() { return serverName; }
    QList<Client*>* getClients() { return &clients; }
    long getMaxPlayers() { return maxPlayers; }
};

#endif // SERVER_H
