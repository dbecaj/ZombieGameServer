#include "server.h"

#include <iostream>
#include <string>

#include "packets/packet.h"
#include "client.h"
#include "packets/packetshutdown.h"
#include "packets/packeterror.h"
#include "packets/packetping.h"

Server::Server(QObject *parent) : QObject(parent)
{
    if(config.isNew()) // if the config is empty we will it with default data
    {
        config.write("ServerName", "ZombieGameServer");
        config.write("ServerIP", findLocalIP());
        config.write("LevelName", "level1.lvl");
        config.write("MaxPlayers", "24");
    }

    bool found;
    serverName = config.read("ServerName", &found);
    if(!found) // if someone messes with config -.-
    {
        config.write("ServerName", "ZombieGameServer");
        serverName = QString::fromStdString("ZombieGameServer");
    }

    serverIP = QHostAddress(config.read("ServerIP", &found));
    if(!found)
    {
        config.write("ServerIP", findLocalIP());
        serverIP = QHostAddress(findLocalIP());
    }

    bool conversion = false;
    maxPlayers = config.read("MaxPlayers", &found).toInt(&conversion);
    if(!found)
    {
        config.write("MaxPlayers", "24");
        maxPlayers = 24;
    }
    else if(!conversion)
    {
        maxPlayers = 24;
        logger.output("MaxPlayers option does not contain a number!", MessageType::ERROR);
    }
}

bool Server::init()
{
    logger.output("Starting " + serverName + " on " + serverIP.toString() + ":" + QString::number(SERVER_PORT));
    game = new GameMP(this);
    if(!game->init())
    {
        return false;
    }

    logger.run();

    return true;
}

Server::~Server()
{
    // Trys to send SERVER_SHUTDOWN packet before the server object is destroyed
    PacketShutdown shutdownPacket;
    sendPacketToAllClients(&shutdownPacket, Protocol::TCP);
}

void Server::stopServer(QString reason)
{
    QString closingMessage = "Closing the server!";
    if(reason != "") closingMessage += " (" + reason + ")";
    logger.output(closingMessage);

    logger.stop();
    QCoreApplication::quit(); // Trys to quit the application event loop
}

void Server::bind(int port)
{
    udpSocket.bind(port, QUdpSocket::ShareAddress);
    this->connect(&udpSocket, SIGNAL(readyRead()), this, SLOT(processUdpPendingDatagrams()));

    tcpServer.listen(serverIP, port);
    this->connect(&tcpServer, SIGNAL(newConnection()), this, SLOT(acceptNewConnection())); // connect the newConnection() signal so we can add new Clients when they try to connect
}

QString Server::findLocalIP()
{
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // Use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address())
        {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }

    if(ipAddress.isEmpty()) ipAddress = QHostAddress(QHostAddress::LocalHost).toString(); // if we did not find one, use IPv4 localhost

    return ipAddress;
}

void Server::processUdpPendingDatagrams()
{
    while(udpSocket.hasPendingDatagrams())
    {
        QByteArray datagram;
        QHostAddress ip;
        quint16 port;
        datagram.resize(udpSocket.pendingDatagramSize());
        udpSocket.readDatagram(datagram.data(), datagram.size(), &ip, &port);

        PacketType type = Packet::determinePacketType(datagram);

        if(type == PacketType::INVALID)
        {
            PacketError errorPacket(QString::fromStdString("UDP: invalid packet"));
            sendUdpPacket(ip, port, &errorPacket);
        }
        else if(type == PacketType::ERROR)
        {
            logger.output(datagram.mid(2), MessageType::ERROR);
        }
        else if(type == PacketType::PING)
        {
            PacketPing pingPacket(datagram);
            sendUdpPacket(ip, port, &pingPacket); // just send back the same packet
        }
        else if(type == PacketType::BASIC)
        {
            QString ipAddress = ip.toString();
            if(ipAddress != "" && ipAddress.at(0) == ':') // if we get IPv6 (::ffff:) we just take the IPv4 part at the end
                ipAddress = ipAddress.mid(7);

            QString message("CLIENT:UDP[");
            message += ipAddress.toUtf8().data();
            message += ":";
            message += QString::number(port);
            message += "]:";
            message += datagram.mid(2).data();

            logger.output(message);
        }
        else emit udpPacketReceived(ip, port, datagram);
    }
}

void Server::acceptNewConnection()
{
    QTcpSocket* socket = tcpServer.nextPendingConnection();
    if(!doesClientExist(socket))
    {
        // If client with this socket doesn't exist we create it and add it to the Server
        Client* client = new Client(socket, this);
        clients.push_back(client);
        this->connect(client, SIGNAL(playerJoined(QString,Client*)), this, SLOT(acceptNewPlayer(QString,Client*)));
        this->connect(client, SIGNAL(removeClient(Client*)), this, SLOT(removeClient(Client*)));
    }
}

bool Server::doesClientExist(QTcpSocket* socket)
{
    QHostAddress ip = socket->peerAddress();
    int port = socket->peerPort();

    for(Client* c : clients)
    {
        if(c->getIP() == ip && c->getPort() == port) return true;
    }

    return false;
}

void Server::dropAllClients()
{
    PacketShutdown serverShutdown;

    for(int i = 0; i < clients.size(); i++)
    {
        clients[i]->sendPacket(&serverShutdown, Protocol::TCP); // when we drop the client we just tell it SERVER_SHUTDOWN
        clients[i]->disconnectClient();
    }
}

void Server::removeClient(Client* client)
{
    for(int i = 0; i < clients.size(); i++)
    {
        if(client == clients[i])
        {
            clients.removeAt(i);
            delete client;
            return;
        }
    }
}

void Server::acceptNewPlayer(QString username, Client* client)
{
    Player* player = new Player(username, client);
    if(!game->addPlayer(player)) // this will atempt to add player to game if the username is the already in game it will not add him and return false
    {
        logger.output("Player named " + username + " has already joined!");
        delete player;
    }
}

void Server::sendPacketToAllClients(Packet *packet, Protocol protocol)
{
    for(Client* c : clients)
    {
        if(protocol == Protocol::TCP) c->sendPacket(packet, protocol);
        else if(protocol == Protocol::UDP) sendUdpPacket(c->getIP(), c->getPort(), packet);
    }
}

void Server::sendUdpPacket(QHostAddress ip, int port, Packet* packet)
{
    sendUdpMessage(ip, port, packet->getData(true));
}

void Server::sendUdpMessage(QHostAddress ip, int port, QByteArray datagram)
{
    udpSocket.writeDatagram(datagram, ip, port);
}
