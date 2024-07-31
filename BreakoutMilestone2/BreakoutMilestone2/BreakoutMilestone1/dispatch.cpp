#include "dispatch.h"
#include <QKeyEvent>
#include <QDebug>

Dispatch::Dispatch() : QGraphicsRectItem(0, 0, 0, 0) {

}

void Dispatch::keyPressEvent(QKeyEvent *event)
{
    last_key = event->key();
}
