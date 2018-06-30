#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <QList>
#include <QPointF>

#include "tile.h"

enum class FileType
{
    INVALID,
    TEXT,
    BINARY
};

class Level
{
    QList<QString> levelData;
    bool isInitialized;
    QList<Tile> tiles;
    QList<QPointF> scientists;
    QList<QPointF> zombies;

    bool readFromTxt(QString path);
    bool readFromLvl(QString path);
public:
    Level();
    bool init(QString levelPath);
    bool save(QString levelPath);

    bool loadLevel();
    QList<QString> getLevelData() { return levelData; }
    QList<Tile> getTiles() { return tiles; }

    QPointF playerStartPos;

    float levelWidth;
    float levelHeight;

    QList<QPointF> getScientists() { return scientists; }
    QList<QPointF> getZombies() { return zombies; }
};

#endif // LEVEL_H
