#include "brick.h"

Brick::Brick(int x, int y, int w, int h, QColor clr)
    : QGraphicsRectItem(x, y, w, h) {
    setRect(x, y, w, h);
    setBrush(QBrush(clr));
    x_pos = x;
    y_pos = y;
    this->w = w;
    this->h = h;
    this->clr = clr;
}
