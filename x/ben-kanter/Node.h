#ifndef NODE_H
#define NODE_H

#include <QGraphicsTextItem>
#include "CreatedNode.h"

class Node : public QGraphicsTextItem
{
    public:
        Node(int x, int y, QGraphicsItem* parent = nullptr) : QGraphicsTextItem(parent) 
        {
            setPos(x, y);
            setZValue(2);
            setFlag(QGraphicsItem::ItemIsMovable);
            setFlag(QGraphicsItem::ItemIsSelectable);
            setTextInteractionFlags(Qt::TextEditorInteraction);
            centerPos = pos();
            centerText();
        }

        void centerText();

        void keyPressEvent(QKeyEvent* event);
    
    private:
        QPointF centerPos;

};

#endif