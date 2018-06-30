#include "packetleveldata.h"

#include <QList>

PacketLevelData::PacketLevelData(QByteArray data) : Packet(data)
{
    valid = validate();
}

PacketLevelData::PacketLevelData(QList<QString> levelData) : Packet(PacketType::LEVEL_DATA)
{
    writeLevelData(levelData);
}

PacketLevelData::PacketLevelData() : Packet(PacketType::LEVEL_DATA)
{
    valid = false;
}

void PacketLevelData::writeLevelData(QList<QString> levelData)
{
    QString data = "";
    for(QString lineData : levelData)
    {
        data += lineData + "#";
    }
    if(data.length() != 0) data = data.mid(0, data.length() - 1); // trim the extra #

    this->writeData(data);
}

bool PacketLevelData::validate()
{
    valid = false;

    QString last = this->getData().mid(data.length() - 1, data.length());
    if(last == "#")
    {
        errorMsg = "LEVEL_DATA packet error: The last sign is # aka. it wasn't trimed!";
        return false;
    }

    valid = true;
    return true;
}

QList<QString> PacketLevelData::getLevelData()
{
    QList<QString> levelData;
    if(!valid) return levelData;

    QString data = this->getData();
    for(QString line : data.split("#"))
    {
        levelData.push_back(line);
    }

    return levelData;
}
