#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <QPointF>

class Vector2D
{
    float x,y;
public:
    Vector2D();
    Vector2D(QPointF pos);
    Vector2D(float x, float y);

    Vector2D operator +(Vector2D vec);
    Vector2D operator -(Vector2D vec);
    void operator =(Vector2D vec);

    Vector2D normalized();
    float length();

    float getX() { return x; }
    float getY() { return y; }
};

#endif // VECTOR2D_H
