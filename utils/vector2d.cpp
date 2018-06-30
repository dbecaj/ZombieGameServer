#include "vector2d.h"

#include <qmath.h>

Vector2D::Vector2D()
{
    //Vector2D(0.0f, 0.0f);
    x = 0.0f;
    y = 0.0f;
}

Vector2D::Vector2D(QPointF pos)
{
    //Vector2D(pos.x(), pos.y());
    this->x = pos.x();
    this->y = pos.y();
}

Vector2D::Vector2D(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2D Vector2D::operator +(Vector2D vec)
{
    return Vector2D(x + vec.x, y + vec.y);
}

Vector2D Vector2D::operator -(Vector2D vec)
{
    return Vector2D(x - vec.x, y - vec.y);
}

void Vector2D::operator =(Vector2D vec)
{
    x = vec.x;
    y = vec.y;
}

float Vector2D::length()
{
    return qAbs(qSqrt(qPow(x, 2) + qPow(y, 2)));
}

Vector2D Vector2D::normalized()
{
    return Vector2D(x/length(), y/length());
}
