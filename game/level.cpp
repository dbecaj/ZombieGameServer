#include "level.h"

#include <fstream>
#include <iostream>
#include <QChar>
#include <random>
#include <ctime>

#include "core/logger.h"
#include "core/server.h"
#include "core/logger.h"
#include "game/entities/mobs/zombie.h"
#include "game/entities/mobs/scientist.h"

extern Server* server;
Level::Level()
{
    isInitialized = false;
}

bool Level::init(QString levelPath)
{
    if(isInitialized) return false;

    FileType fileType = FileType::INVALID;
    QStringList list = levelPath.split(".");
    if(list.last() == "txt") fileType = FileType::TEXT;
    else if(list.last() == "lvl") fileType = FileType::BINARY;

    if(fileType == FileType::TEXT)
    {
        if(!readFromTxt(levelPath)) return false;
    }
    else if(fileType == FileType::BINARY)
    {
        if(!readFromLvl(levelPath)) return false;
    }
    else
    {
        server->getLogger()->output("Level: " + levelPath + " is the wrong file type!", MessageType::ERROR);
        return false;
    }

    isInitialized = true;
    return true;
}

bool Level::readFromTxt(QString path)
{
    std::fstream level;
    level.open(path.toStdString(), std::ios::in);
    if(!level.is_open())
    {
        server->getLogger()->output("Level: " + path + " could not be opened!", MessageType::ERROR);
        return false;
    }

    std::string line;
    while(std::getline(level, line))
        levelData.push_back(QString::fromStdString(line));

    level.close();

    return true;
}

bool Level::readFromLvl(QString path)
{
    std::fstream level;
    level.open(path.toStdString(), std::ios::binary | std::ios::in);
    if(!level.is_open())
    {
        server->getLogger()->output("Level: " + path + " could not be opened!", MessageType::ERROR);
        return false;
    }

    int listLength;
    level.read((char*)&listLength, sizeof(int));
    levelData.reserve(listLength);
    int lineLength;
    while(level.read((char*)&lineLength, sizeof(int)))
    {
          std::string line;
          line.resize(lineLength);
          level.read((char*)&line[0], lineLength);

          levelData.push_back(QString::fromStdString(line));
    }

    level.close();

    return true;
}

bool Level::save(QString levelPath)
{
    if(!isInitialized)
    {
        server->getLogger()->output("Level is not initialized!", MessageType::ERROR);
        return false;
    }

    std::fstream level;
    level.open(levelPath.toStdString(), std::ios::out | std::ios::binary);

    int listLength = levelData.length();
    level.write((char*)&listLength, sizeof(int));
    for(int i = 0; i < levelData.size(); i++)
    {
        int lineLength = levelData[i].length();
        level.write((char*)&lineLength, sizeof(int));
        level.write(levelData[i].toStdString().c_str(), lineLength);
    }

    // [listSize][lineSize](data)[lineSize](data).....

    level.close();
    return true;
}

bool Level::loadLevel()
{
    if(!isInitialized)
    {
        server->getLogger()->output("Level is not initialized!", MessageType::ERROR);
        return false;
    }

    int y = 0;
    int x = 0;
    for(y = 0; y < levelData.size(); y++)
    {
        for(x = 0; x < levelData[y].length(); x++)
        {
            switch(levelData[y][x].toLatin1())
            {
            case 'W':
            {
                Tile wall(x * TILE_SIZE, y * TILE_SIZE, TileType::WALL);
                tiles.push_back(wall);
                break;
            }
            case 'B':
            {
                Tile box(x * TILE_SIZE, y * TILE_SIZE, TileType::BOX);
                tiles.push_back(box);
                break;
            }
            case 'C':
            {
                Tile computer(x * TILE_SIZE, y * TILE_SIZE, TileType::COMPUTER);
                tiles.push_back(computer);
                break;
            }
            case 'F':
            {
                Tile bookshelf(x * TILE_SIZE, y * TILE_SIZE, TileType::BOOKSHELF);
                tiles.push_back(bookshelf);
                break;
            }

            case 'P':
            {
                playerStartPos = QPointF(x * TILE_SIZE, y * TILE_SIZE);
                break;
            }
            case 'Z':
            {
                Zombie* zombie = new Zombie(QPointF(x * TILE_SIZE, y * TILE_SIZE));
                server->getGame()->addZombie(zombie);
                zombies.push_back(zombie->getPosition());
                break;
            }
            case 'S':
            {
                static std::mt19937 randEngine(time(0));

                std::uniform_int_distribution<int> randGender(0, 1);
                ScientistGender gender = ScientistGender::MALE;
                if(randGender(randEngine) == 1) gender = ScientistGender::FEMALE;
                Scientist* scientist = new Scientist(gender, QPointF(x * TILE_SIZE, y * TILE_SIZE));
                server->getGame()->addScientist(scientist);
                scientists.push_back(scientist->getPosition());
                break;
            }
            default:
            {
                break;
            }
            }
        }
    }
    return true;

    levelWidth = x * TILE_SIZE;
    levelHeight = y * TILE_SIZE;
}
