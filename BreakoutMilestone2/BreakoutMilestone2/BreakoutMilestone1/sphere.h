#ifndef SPHERE_H
#define SPHERE_H

#include <QPainter>
#include <QGraphicsEllipseItem>
#include <QColor>

class Sphere : public QGraphicsEllipseItem
{
public:
    Sphere(int radius, QColor clr, int x, int y, int speed);
    void reflect_vert();
    void reflect_horz();
    void move();
    int radius;
    QColor clr;
    double angle = M_PI_4;  // Initial angle in radians (45 degrees)
    double speed;  // Speed of the sphere
};

#endif // SPHERE_H
