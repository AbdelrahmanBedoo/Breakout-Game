#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsRectItem>
#include <QColor>
#include <QPainter>
#include "sphere.h"

class Player : public QGraphicsRectItem {
public:
    Player(int w, int h, QColor clr, double speed, Qt::Key left_key = Qt::Key_Left, Qt::Key right_key = Qt::Key_Right);
    void move(int screen_w, int last_key);
    bool disabled_after_collide(Sphere* sphere, int sphere_y_last_frame);
    QColor clr;
    bool moving = false;
    bool toRight = false;
    bool weapons = false;
    double speed;
    int w;
    int h;
    Qt::Key left_key;
    Qt::Key right_key;
};

#endif // PLAYER_H
