#include "tile.h"

Tile::Tile(float x, float y, TileType type)
{
    collisionBox.setRect(x, y, TILE_SIZE, TILE_SIZE);
    this->type = type;
}

QPointF Tile::getCenterPosition()
{
    QPointF center(collisionBox.x() + TILE_SIZE/2.0f, collisionBox.y() + TILE_SIZE/2.0f);
    return center;
}
