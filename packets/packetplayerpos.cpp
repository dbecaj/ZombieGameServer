#include "packetplayerpos.h"

#include <QStringList>

#include "game/entities/mobs/mob.h"

PacketPlayerPos::PacketPlayerPos(QByteArray data) : Packet(data)
{
    valid = validate();
}

PacketPlayerPos::PacketPlayerPos(QString username, QPointF pos, float degrees) : Packet(PacketType::PLAYER_POS)
{
    writePlayerPos(username, pos, degrees);
}

PacketPlayerPos::PacketPlayerPos() : Packet(PacketType::PLAYER_POS)
{
    valid = false;
}

bool PacketPlayerPos::validate()
{
    valid = false;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    if(splitData.length() != 4) // [playerName],[x],[y],[degrees]
    {
        errorMsg = "PLAYER_POS packet error: The packet format is wrong!";
        return false;
    }

    valid = true;
    return true;
}

void PacketPlayerPos::writePlayerPos(QString username, QPointF pos, float degrees)
{
    float x = pos.x();
    float y = pos.y(); // we send the collision box pos
    QString data = username + "," + QString::number(x) + "," + QString::number(y) + "," + QString::number(degrees);
    this->writeData(data);
}

QString PacketPlayerPos::getUsername()
{
    QString username = "";
    if(!valid) return username;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    username = splitData[0];

    return username;
}

QPointF PacketPlayerPos::getPosition()
{
    QPointF pos(0.0f, 0.0f);
    if(!valid) return pos;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    float x = splitData[1].toFloat();
    float y = splitData[2].toFloat(); // transform to bounding box pos

    return QPointF(x, y);
}

float PacketPlayerPos::getDegrees()
{
    float degrees = 0.0f;
    if(!valid) return degrees;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    degrees = splitData[3].toFloat();

    return degrees;
}
