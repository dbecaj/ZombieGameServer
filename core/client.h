#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

#include "packets/packet.h"
#include "server.h"

class Client : public QObject
{
    Q_OBJECT
    QTcpSocket* tcpSocket;
    QHostAddress ip;
    int udpPort;
    bool udpPortInitialized; // this will be false until we get the port from the udp packet
    Player* player;
    QList<QByteArray> sendQueue;
private slots:
    void processTcpDatagram(); // Handles the recevied message/packet first and then passes it down the line
    void sendNext(qint64 bytes);
public slots:
    void disconnectClient(); // If we have connected player it calls disconnectPlayer() and then emits removeClint signal
signals:
    void playerJoined(QString playerName, Client* client); // Fires when a JOIN packet is recived
    void tcpPacketRecived(QByteArray data); // Is used for sending data that is not processed in processTcpDatagram() down the line
    void removeClient(Client* client); // Fires when we want Server to remove this client
public:
    explicit Client(QTcpSocket* tcpSocket, QObject *parent = nullptr);

    QHostAddress getIP() { return ip; }
    int getPort() { return udpPort; }
    QTcpSocket* getTcpSocket() { return tcpSocket; }

    bool isConnectedToPlayer();
    void connectToPlayer(Player* player) { this->player = player; }
    void disconnectFromPlayerHandle() { player = nullptr; }
    void disconnectPlayer();

    // This methods are used for when we want to initialize udp port to correct port which we get through a udp packet
    bool hasUdpPort() { return udpPortInitialized; }
    void setUdpPort(int udpPort);
    void sendPacket(Packet* packet, Protocol protocol);
};

#endif // CLIENT_H
