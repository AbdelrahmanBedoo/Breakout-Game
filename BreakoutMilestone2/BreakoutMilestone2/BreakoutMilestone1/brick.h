#ifndef BRICK_H
#define BRICK_H

#include <QGraphicsRectItem>
#include <QColor>
#include <QPainter>
class Brick : public QGraphicsRectItem {
public:
    Brick(int x, int y, int w, int h, QColor clr);
    int w;
    int h;
    int x_pos;
    int y_pos;
    QColor clr;
};

#endif // BRICK_H
