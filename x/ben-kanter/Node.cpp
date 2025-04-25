#include "Node.h"
#include <QKeyEvent>
#include <QGraphicsScene>
#include <iostream>

using namespace std;

extern vector<CreatedNode> actionsHistory;

void Node::keyPressEvent(QKeyEvent* event) {
    bool isEmpty = toPlainText() == "";
    switch(event->key()) {
        case Qt::Key_Escape:
        case Qt::Key_Enter:
        case Qt::Key_Return:
            if(isEmpty) {
                cout << "We are removing this node!" << endl;
                scene()->removeItem(this);
                delete this;
                return;
            }
            else {
                actionsHistory.push_back(CreatedNode(*this));
            }

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