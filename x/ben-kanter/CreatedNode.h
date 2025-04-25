#ifndef CREATED_NODE_H
#define CREATED_NODE_H

#include <QGraphicsScene>
#include "Node.h"
class Node;
class CreatedNode 
{
    public:
        CreatedNode(const Node& node);
       // void call();
        void undo(QGraphicsScene* scene);
    private:
        qreal x;
        qreal y;
};

#endif