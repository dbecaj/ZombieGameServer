#include "packetentitydata.h"

#include <QStringList>

PacketEntityData::PacketEntityData(QByteArray data) : Packet(data)
{
    valid = validate();
}

PacketEntityData::PacketEntityData(QList<Player*> players, QList<Zombie*> zombies, QList<Scientist*> scientists) : Packet(PacketType::ENTITY_DATA)
{
    writeEntityData(players, zombies, scientists);
}

PacketEntityData::PacketEntityData() : Packet(PacketType::ENTITY_DATA)
{
    valid = false;
}

void PacketEntityData::writeEntityData(QList<Player*> players, QList<Zombie*> zombies, QList<Scientist*> scientists)
{
    QString playerData = ""; // [playerName]*...
    for(Player* p : players)
    {
        if(p->isInfected()) continue;
        playerData += p->getUsername() + "/" + QString::number(p->getPosition().x()) + "/" + QString::number(p->getPosition().y()) + "*";
    }
    if(playerData.length() != 0) playerData = playerData.mid(0, playerData.length() - 1); // trim the extra *

    QString zombieData = ""; // [zombieId]/[x]/[y]/[lives]*...
    for(Zombie* z : zombies)
    {
        QString extraData = QString::number(z->getId()) + "/" + QString::number(z->getPosition().x()) + "/" +
                            QString::number(z->getPosition().y()) + "/" + QString::number(z->getLives());
        zombieData += extraData + "*";
    }
    if(zombieData.length() != 0) zombieData = zombieData.mid(0, zombieData.length() - 1); // again trim the extra *

    QString scientistData = ""; // [scientistId]/[gender]/[x]/[y]/[dir]*...
    for(Scientist* s : scientists)
    {
        QString extraData = "";
        extraData += QString::number(s->getId()) + "/";
        if(s->getGender() == ScientistGender::MALE) extraData += "M/";
        else extraData += "F/";
        extraData += QString::number(s->getPosition().x()) + "/" + QString::number(s->getPosition().y()) +
                         "/" + QString::number(s->getDirection());
        scientistData += extraData + "*";
    }
    if(scientistData.length() != 0) scientistData = scientistData.mid(0, scientistData.length() - 1); // you get the idea

    QString data = playerData + "," + zombieData + "," + scientistData;
    this->writeData(data);
}

bool PacketEntityData::validate()
{
    valid = false;

    usernames = "";
    zombies = "";
    scientists = "";

    QString data = this->getData();
    // [playerName]/[x]/[y]*...,[zombieId]/[x]/[y]/[lives]*...,[scientistId]/[gender]/[x]/[y]/[dir]*...
    QStringList splitData = data.split(",");
    if(splitData.length() != 3)
    {
        errorMsg = "ENTITY_DATA packet error: The packet format is wrong!";
        return false;
    }

    for(QString playerData : splitData[0].split("*"))
    {
        if(playerData == "") continue; // No players
        if(playerData.split("/").length() != 3)
        {
            errorMsg = "ENTITY_DATA packet error: The packet player format is wrong!";
            return false;
        }
    }

    for(QString zombieData : splitData[1].split("*"))
    {
        if(zombieData == "") continue; // No zombies
        if(zombieData.split("/").length() != 4)
        {
            errorMsg = "ENTITY_DATA packet error: The packet zombie format is wrong!";
            return false;
        }
    }

    for(QString scientistData : splitData[2].split("*"))
    {
        if(scientistData == "") continue; // No scientists
        if(scientistData.split("/").length() != 5)
        {
            errorMsg = "ENTITY_DATA packet error: The packet scientist format is wrong!";
            return false;
        }
    }

    usernames = splitData[0];
    zombies = splitData[1];
    scientists = splitData[2];

    valid = true;
    return true;
}

QList<Player*> PacketEntityData::getPlayers()
{
    QList<Player*> players;
    if(!valid) return players;

    for(QString playerData : usernames.split("*"))
    {
        QStringList splitData = playerData.split("/");
        float x = splitData[1].toFloat();
        float y = splitData[2].toFloat();
        //Player* player = new Player(QPointF(x, y), splitData[0]);
        //players.push_back(player);
    }

    return players;
}

QList<Zombie*> PacketEntityData::getZombies()
{
    QList<Zombie*> zombies;
    if(!valid) return zombies;

    for(QString zombieData : this->zombies.split("*"))
    {
        if(zombieData == "") break;
        QStringList splitData = zombieData.split("/");
        long id = splitData[0].toLong();
        float x = splitData[1].toFloat();
        float y = splitData[2].toFloat();
        int lives = splitData[3].toInt();
        Zombie* zombie = new Zombie(QPointF(x, y), id);
        zombie->setLives(lives);
        zombies.push_back(zombie);
    }

    return zombies;
}

QList<Scientist*> PacketEntityData::getScientists()
{
    QList<Scientist*> scientists;
    if(!valid) return scientists;

    for(QString scientistData : this->scientists.split("*"))
    {
        if(scientistData == "") break;
        QStringList splitData = scientistData.split("/");
        long id = splitData[0].toLong();
        ScientistGender gender = ScientistGender::MALE;
        if(splitData[1] == "F") gender = ScientistGender::FEMALE;
        float x = splitData[2].toFloat();
        float y = splitData[3].toFloat();
        Scientist* scientist = new Scientist(gender, QPointF(x, y), id);
        float angle = splitData[4].toFloat();
        scientist->changeAngle(angle);
        scientists.push_back(scientist);
    }

    return scientists;
}
