#ifndef TILE_H
#define TILE_H

#include <QRectF>
#include <QPointF>

#define TILE_SIZE 100.0f

enum class TileType
{
    WALL,
    BOX,
    COMPUTER,
    BOOKSHELF
};

class Tile
{
    QRectF collisionBox;
    TileType type;
public:
    Tile(float x, float y, TileType type);

    QRectF getCollisionBox() { return collisionBox; }
    TileType getTileType() { return type; }
    QPointF getPosition() { return QPointF(collisionBox.x(), collisionBox.y()); }
    QPointF getCenterPosition();
};

#endif // TILE_H
