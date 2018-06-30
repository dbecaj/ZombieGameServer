#ifndef PACKETLEVELDATA_H
#define PACKETLEVELDATA_H

#include <QString>

#include "packet.h"

class PacketLevelData : public Packet
{
public:
    PacketLevelData(QByteArray data);
    PacketLevelData(QList<QString> levelData);
    PacketLevelData();

    void writeLevelData(QList<QString> levelData);
    bool validate();

    QList<QString> getLevelData();
};

#endif // PACKETLEVELDATA_H
