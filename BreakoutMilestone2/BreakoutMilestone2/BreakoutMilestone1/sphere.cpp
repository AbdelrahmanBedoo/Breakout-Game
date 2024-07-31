#include "Sphere.h"
#include <qDebug>

Sphere::Sphere(int radius, QColor clr, int x, int y, int speed)
    : QGraphicsEllipseItem(-radius, -radius, 2 * radius, 2 * radius) {
    setPos(x, y);
    setBrush(QBrush(clr));
    this->radius = radius;
    this->clr = clr;
    this->speed = speed;
}
void Sphere::move()
{
    double dx = speed * cos(angle);
    double dy = speed * sin(angle);

    // Move the sphere both horizontally and vertically
    setPos(x() + dx, y() + dy);
}

void Sphere::reflect_vert()
{
    angle = -angle;
}
void Sphere::reflect_horz()
{
    angle = M_PI - angle;
    if (angle > M_PI) angle = -(2 * M_PI - angle);
    else if (angle < -M_PI) angle = -(2 * M_PI + angle);
}
