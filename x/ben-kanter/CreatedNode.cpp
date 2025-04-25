#include "CreatedNode.h"
#include <iostream>

using namespace std;

CreatedNode::CreatedNode(const Node& node) 
: x(node.x()), y(node.y()) {}

void CreatedNode::undo(QGraphicsScene* scene) {
    QList current_Items = scene->items();
    cout << "2" << endl;
    cout << "x:" << x << " y: " << y << endl;
    for (QGraphicsItem* item : current_Items)
    {
        cout << "4" << endl;
        cout << item->x() << " " << item->y() << endl;
        if (item->type() == QGraphicsTextItem::Type && item->x() == x && item->y() == y)
        {
            cout << "3" << endl;
            scene->removeItem(item);
            delete item;
            return;
        }
    }
}