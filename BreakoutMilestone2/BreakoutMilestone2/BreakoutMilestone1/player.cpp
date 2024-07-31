#include "player.h"
#include "sphere.h"
#include <QKeyEvent>
#include <qDebug>

Player::Player(int w, int h, QColor clr, double speed, Qt::Key left_key, Qt::Key right_key)
    : QGraphicsRectItem(0, 0, w, h) {
    this->w = w;
    this->h = h;
    this->clr = clr;
    this->speed = speed;
    this->left_key = left_key;
    this->right_key = right_key;
    setRect(0, 0, w, h);
    setBrush(QBrush(clr));
}

void Player::move(int screen_w, int last_key)
{
    if(last_key == left_key) {
        moving = true;
        toRight = false;
    } else if(last_key == right_key) {
        moving = true;
        toRight = true;
    } else if (last_key == Qt::Key_Space)
    {
        qDebug() << "SPACE!";
    }
    else {
        moving = false;
    }

    if (moving)
    {
        if (toRight)
        {
            if (x() + w < screen_w)
                setX(x() + speed);
            else
                moving = false;
        }
        else
        {
            if (x() > 0)
                setX(x() - speed);
            else
                moving = false;
        }
    }
}

bool Player::disabled_after_collide(Sphere* sphere, int sphere_y_last_frame)
{
    if (sphere_y_last_frame + sphere->radius <= y())
    {
        sphere->setY(y() - sphere->radius);
        sphere->reflect_vert();
        // Reflect vertically
        return false;
    }
    else
    {
        if ((abs(sphere->angle - 3 * M_PI_4) < 1e-5) && sphere->x() > x())
            sphere->reflect_horz();
        if ((abs(sphere->angle - M_PI_4) < 1e-5) && sphere->x() < x())
            sphere->reflect_horz();
        return true;
    }
}

