#ifndef DISPATCH_H
#define DISPATCH_H

#include <QGraphicsRectItem>
#include <QKeyEvent>

class Dispatch : public QGraphicsRectItem
{
public:
    Dispatch();
    void keyPressEvent(QKeyEvent *event) override;
    int last_key = Qt::Key_S;
};

#endif // DISPATCH_H
