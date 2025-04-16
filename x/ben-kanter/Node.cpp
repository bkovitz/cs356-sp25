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
        case Qt::Key_Backspace:
            // if there's no text, delete the node
            if(toPlainText() == "") {
                scene()->removeItem(this);
                delete this;
                break;
            }
            // else act normally
        default:
            QGraphicsTextItem::keyPressEvent(event);
            break;
    }
}