#include "client.h"

#include "packets/packet.h"
#include "packets/packetentitydata.h"
#include "packets/packetleveldata.h"
#include "packets/packetplayerchat.h"
#include "packets/packetinfo.h"
#include "game/entities/mobs/zombie.h"
#include "game/entities/mobs/scientist.h"
#include "packets/packeterror.h"
#include "packets/packetjoin.h"
#include "packets/packetdisconnect.h"

extern Server* server;
Client::Client(QTcpSocket *tcpSocket, QObject* parent) : QObject(parent)
{
    this->tcpSocket = tcpSocket;
    ip = tcpSocket->peerAddress();
    udpPort = tcpSocket->peerPort(); // this is temporary
    udpPortInitialized = false;
    player = nullptr;
    this->connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(processTcpDatagram()));
    this->connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnectClient()));
    this->connect(tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(sendNext(qint64)));
}

void Client::processTcpDatagram()
{
    // Read the sent data and put it in a packet
    QByteArray datagram = tcpSocket->readAll();

    PacketType type = Packet::determinePacketType(datagram);

    if(type == PacketType::INVALID)
    {
        PacketError errorPacket(QString::fromStdString("TCP: Client invalid packet!"));
        sendPacket(&errorPacket, Protocol::TCP);
    }
    else if(type == PacketType::ERROR)
    {
        PacketError packet(datagram);
        server->getLogger()->output(packet.getError(), MessageType::ERROR);
    }
    else if(type == PacketType::INFO)
    {
        PacketInfo infoPacket(server->getServerName(), server->getGame()->getNumOfConnectedPlayers(), server->getMaxPlayers());
        sendPacket(&infoPacket, Protocol::TCP);
    }
    else if(type == PacketType::JOIN)
    {
        PacketJoin joinPacket(datagram);
        if(server->getGame()->getNumOfConnectedPlayers() >= server->getMaxPlayers()) return;
        if(server->getGame()->hasPlayerWithUsername(joinPacket.getUsername()))
        {
            PacketError errorPacket("Player named " + joinPacket.getUsername() + " is already in this server!");
            sendPacket(&errorPacket, Protocol::TCP);
        }
        else
        {
            server->getGame()->sendTcpPacketToAllPlayers(&joinPacket);
            sendPacket(&joinPacket, Protocol::TCP); // return confirm JOIN packet to the client

            QString playerName = joinPacket.getUsername();
            emit playerJoined(playerName, this);
        }
    }
    else if(type == PacketType::LEVEL_DATA)
    {
        if(!player)
        {
            PacketError errorPacket(QString::fromStdString("You are not joined in this server!"));
            sendPacket(&errorPacket, Protocol::TCP);
        }

        PacketLevelData levelDataPacket(server->getGame()->getLevel()->getLevelData());
        sendPacket(&levelDataPacket, Protocol::TCP);
    }
    else if(type == PacketType::ENTITY_DATA)
    {
        if(!player)
        {
            PacketError errorPacket(QString::fromStdString("You are not joined in this server!"));
            sendPacket(&errorPacket, Protocol::TCP);
            return;
        }

        PacketEntityData entityDataPacket(server->getGame()->players, server->getGame()->zombies, server->getGame()->scientists);
        sendPacket(&entityDataPacket, Protocol::TCP);
    }
    else if(type == PacketType::PLAYER_CHAT)
    {
        PacketPlayerChat playerChatPacket(datagram);
        if(!playerChatPacket.isValid())
        {
            server->getLogger()->output(playerChatPacket.getErrorMsg(), MessageType::ERROR);
            return;
        }

        server->getGame()->sendTcpPacketToAllPlayers(&playerChatPacket, playerChatPacket.getUsername());
    }
    else if(type == PacketType::DISCONNECT)
    {
        disconnectPlayer();
    }
    else if(type == PacketType::BASIC)
    {
        QHostAddress ip = tcpSocket->peerAddress();
        int port = tcpSocket->peerPort();
        QString ipAddress = ip.toString();
        if(ipAddress != "" && ipAddress.at(0) == ':') // if we get IPv6 (::ffff:) we just take the IPv4 part at the end
            ipAddress = ipAddress.mid(7);

        QString message("CLIENT:TCP[");
        message += ipAddress + ":" + QString::number(port) + "]:";
        message += datagram.mid(2);

        server->getLogger()->output(message);
    }
    else emit tcpPacketRecived(datagram);
}

bool Client::isConnectedToPlayer()
{
    // If player pointer is pointing anywhere then we have a player connected
    if(player) return true;

    return false;
}

void Client::setUdpPort(int udpPort)
{
    this->udpPort = udpPort;
    udpPortInitialized = true;
}

void Client::disconnectPlayer()
{
    if(isConnectedToPlayer())
    {
        // We remove the player and send the DISCONNECT packet to all other player
        QString username = server->getGame()->removePlayer(this); // this removes the player from the game and returns his username
        PacketDisconnect packet(username);
        server->getGame()->sendTcpPacketToAllPlayers(&packet);
        server->getLogger()->output(username + " has disconnected!");
        player = nullptr;
    }
}

void Client::disconnectClient()
{
    // When we disconnectClient we also remove the player
    if(isConnectedToPlayer())
    {
        disconnectPlayer();
    }

    emit removeClient(this);
}

void Client::sendNext(qint64 bytes)
{
    if(sendQueue.length() == 0) return;

    tcpSocket->write(sendQueue.first());

    sendQueue.removeFirst();
}

void Client::sendPacket(Packet *packet, Protocol protocol)
{
    if(protocol == Protocol::TCP)
    {
        if(tcpSocket->bytesToWrite())
        {
            sendQueue.push_back(packet->getData(true));
            return;
        }
        tcpSocket->write(packet->getData(true));
    }
    else if(protocol == Protocol::UDP) server->sendUdpPacket(ip, udpPort, packet);
}
