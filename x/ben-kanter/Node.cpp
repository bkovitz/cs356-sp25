#include "Node.h"
#include <QKeyEvent>
#include <QGraphicsScene>

void Node::keyPressEvent(QKeyEvent* event) {
    switch(event->key()) {
        case Qt::Key_Escape:
        case Qt::Key_Enter:
        case Qt::Key_Return:
            clearFocus();
            break;
        default:
            QGraphicsTextItem::keyPressEvent(event);
            break;
    }
    centerText();
}

void Node::centerText(){
    setPos(centerPos.x() + boundingRect().width() / -2, centerPos.y() + boundingRect().height() / -2);
}