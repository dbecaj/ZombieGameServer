#include "packet.h"

#include <QString>
#include <QList>

Packet::Packet(QByteArray data)
{
    valid = false;
    errorMsg = "";

    this->data = data;
    // Get PacketType out of the data
    type = Packet::determinePacketType(data);
    QByteArray size = data.left(4);
    memcpy(&packetSize, size, sizeof(quint32));
}

Packet::Packet(PacketType type)
{
    valid = true;

    // Clean QByteArray just in case
    data.clear();
    this->type = type;

    // Add header in the front

    QString idPacketHeader = "";
    if(type < 10) idPacketHeader += "0";
    idPacketHeader += QString::number(type);
    data.insert(0, idPacketHeader);

    packetSize = 2;
    QByteArray number((const char*)&packetSize, sizeof(packetSize));
    data.insert(0, number);
}

QByteArray Packet::getData(bool full)
{
    if(full) return data;

    return data.mid(6);
}

void Packet::writeData(QString data)
{
    this->data.clear();
    this->data.insert(0, data);

    // Rewrite the header
    QString idPacketHeader = "";
    if(type < 10) idPacketHeader += "0";
    idPacketHeader += QString::number(type);
    this->data.insert(0, idPacketHeader);

    packetSize = 2 + data.length();
    QByteArray number((const char*)&packetSize, sizeof(packetSize));
    this->data.insert(0, number);
}

PacketType Packet::getPacketType()
{
    return type;
}

PacketType Packet::determinePacketType(QByteArray data)
{
    QString packetNumber = data.mid(4, 2); // [packetSize]&[type]|[data]
    bool ok;
    PacketType type = PacketType(packetNumber.toInt(&ok));
    if(!ok) type = PacketType::INVALID; // If something is wrong set packet to INVALID(-1)

    return type;
}

QString Packet::extractData(QByteArray data)
{
    return data.mid(6);
}

QString Packet::translatePacket(QByteArray data)
{
    quint32 size = 0;
    QByteArray packetSize = data.left(4);
    memcpy(&size, packetSize, sizeof(quint32));
    return QString::number(data.length()) + " " + QString::number(size);
}
