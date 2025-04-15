#include "Node.h"

void Node::addTextToNode(QString textToAdd) {
    setPlainText(toPlainText() + textToAdd);
}