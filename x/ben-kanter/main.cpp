#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QApplication>
#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>
#include <QPalette>
#include <QStyle>
#include <QStyleHints>
#include <QKeyEvent>
#include <QTextDocument>
#include <QStatusBar>
#include <QList>
#include "Node.h"
#include "CreatedNode.h"

using namespace std;
const int gridBlockWidth   = 100;
const int gridBlockHeight  = 100;

const int sceneWidth = 600;
const int sceneHeight = 300;

vector<CreatedNode> actionsHistory;

ostream& operator<<(ostream& os, const QPointF& p) {
    return os << "(" << p.x() << ", " << p.y() << ")";
}
ostream& operator<<(ostream& os, const QRectF& r) {
    return os << "(" << r.x() << ", " << r.y() << ", " << r.width() << ", " << r.height() << ")";
}
ostream& operator<<(ostream& os, const QRect& r) {
    return os << "(" << r.x() << ", " << r.y() << ", " << r.width() << ", " << r.height() << ")";
}

/*
class ShapeLabel : public QGraphicsTextItem
{
    public:
        ShapeLabel(QGraphicsItem* parent = nullptr) : QGraphicsTextItem(parent) {
            // these three lines make the text align to the center
            QTextOption option = document()->defaultTextOption();
            option.setAlignment(Qt::AlignCenter);
            document()->setDefaultTextOption(option);

            // this line makes the text interactable
            setTextInteractionFlags(Qt::TextEditorInteraction);
        
            updateSize();
        }

        void updateSize() {
            // sets the text's boundingRect to a reasonable size
            adjustSize();

            // centers the text within the parent item
            // (0, 0) for this label means the top left will be at the center of the parent
            setPos(boundingRect().width() / -2, boundingRect().height() / -2);
        }

        // update label when typing
        void keyPressEvent(QKeyEvent* event) override {
            // do what it normally does
            QGraphicsTextItem::keyPressEvent(event);

            updateSize();
        }
};
*/

class EditorView : public QGraphicsView
{
public:
    EditorView(QGraphicsScene* scene) : QGraphicsView(scene) {
        cursor = new QGraphicsPolygonItem();
        QPolygonF cursorPolygon;
        cursorPolygon << QPointF(0, 0) << QPointF(25, 0) << QPointF(0, 25);

        cursor->setPolygon(cursorPolygon);
        cursor->setPos(gridBlockWidth * 1, gridBlockHeight * 1);
        cursor->setBrush(QColor(150, 150, 150, 255));
        cursor->setPen(Qt::NoPen);
        cursor->setZValue(1);

        scene->addItem(cursor);

        statusBar = new QStatusBar(this);
        statusBar->setFixedHeight(20);
        statusBar->setStyleSheet("background-color: #d3d3d3;");
        statusBar->move(0, 0); // Move the status bar to the top-left corner
        statusBar->resize(width(), 20); // Make it span the entire width of the window
        updateStatusBar();
    }

    void keyPressEvent(QKeyEvent* event) override
    {
        QGraphicsView::keyPressEvent(event);
        
        std::cout << "Key pressed: " << event->key() << std::endl;

        // switch (mode)
        // {
        //     case NORMAL_MODE:
        //         switch(event->key())
        //         {
        //             case Qt::Key_N:
        //                 mode = NODE_MODE;
        //                 break;
        //             case Qt::Key_E:
        //                 mode = EDGE_MODE;
        //                 break;
        //             case Qt::Key_G:
        //                 toggleGrid();
        //                 break;
        //         }
        //         break;
        //     case NODE_MODE:
        //         switch(event->key())
        //         {
        //             case Qt::Key_Escape:
        //                 mode = NORMAL_MODE;
        //                 break;
        //             default:
        //                 bool isACharacter = !event->text().isEmpty();

        //                 if (isACharacter) {
        //                     Node* node = nodeAtCurrentLocation();

        //                     if(node == nullptr) {
        //                         node = createNodeAtCursor();
        //                     }
                            
        //                     // if not currently typing in the node, start typing, and pass letter typed
        //                     if(!(node->hasFocus())) {
        //                         node->setFocus();
        //                         node->keyPressEvent(event);
        //                     }
        //                 }

        //         }
        //         break;
        //     case EDGE_MODE:
        //         switch(event->key())
        //         {
        //             case Qt::Key_Escape:
        //                 mode = NORMAL_MODE;
        //                 break;
        //         }
        //         break;
        // }


        // Adding/editing a node
        if (event->key() == Qt::Key_N && event->modifiers() == Qt::ControlModifier) {
            Node* node = nodeAtCurrentLocation();
            
            if(node == nullptr) {
                node = createNodeAtCursor();
            }
            node->setFocus();
            // nodes.append(node);
            // addSnapshot();

        } else if (event->key() == Qt::Key_Delete) {
            Node* node = nodeAtCurrentLocation();
            if (node != nullptr) {
                scene()->removeItem(node);
                // for (int i = 0; i < nodes.size(); i++)
                // {
                //     if(nodes[i] == node){
                //         nodes.removeAt(i);
                //         break;
                //     }
                // }
                delete node;
            }
        } else if (event->key() == Qt::Key_Z && event->modifiers() == Qt::ControlModifier) {
            cout << "Trying to undo" << endl;
            undoLastAction();
        }

        // don't move cursor if we are typing in the node
        bool shouldAllowCursorMovement = !(nodeAtCurrentLocation() != nullptr && nodeAtCurrentLocation()->hasFocus());

        if(shouldAllowCursorMovement)
        {
            switch(event->key())
            {
                case Qt::Key_Left:
                    cursor->moveBy(-gridBlockWidth, 0);
                    break;
                case Qt::Key_Right:
                    cursor->moveBy(gridBlockWidth, 0);
                    break;
                case Qt::Key_Up:
                    cursor->moveBy(0, -gridBlockHeight);
                    break;
                case Qt::Key_Down:
                    cursor->moveBy(0, gridBlockHeight);
                    break;
            }
        }

        updateStatusBar();
    }

    void createGrid() {
        for(int i = 0; i < sceneWidth / gridBlockWidth; i++) {
            QLineF line(i * gridBlockWidth, 0, i * gridBlockWidth, sceneHeight);
            QGraphicsLineItem* item = new QGraphicsLineItem(line);
            gridLines.append(item);
            scene()->addItem(item);
        }
        for(int i = 0; i < sceneHeight / gridBlockHeight; i++) {
            QLineF line(0, i * gridBlockHeight, sceneWidth, i * gridBlockHeight);
            QGraphicsLineItem* item = new QGraphicsLineItem(line);
            gridLines.append(item);
            scene()->addItem(item);
        }
    }

    private:
        QGraphicsPolygonItem* cursor;
        enum { NORMAL_MODE, NODE_MODE, EDGE_MODE } mode = NORMAL_MODE;
        QList<QGraphicsLineItem*> gridLines;
        bool gridVisible = true;
        QStatusBar* statusBar;

        // QList<Node*> nodes;
        // QList<QList<Node>> snapshots;

        // void addSnapshot()
        // {
        //     QList<Node> newSnapshot;
        //     for (auto node : nodes) {
        //         Node copyOfNode(node);
        //         newSnapshot.append(copyOfNode);
        //     }
        //     snapshots.append(newSnapshot);
        // }

        void undoLastAction() {
            cout << actionsHistory.size() << endl;
            if (!actionsHistory.empty()) {
                CreatedNode action = actionsHistory.back();
                actionsHistory.pop_back();
                action.undo(scene());
            }
        }

        Node* createNodeAtCursor() {
            int x = cursor->x();
            int y = cursor->y();
            Node* node = new Node(x, y);
            scene()->addItem(node);
            return node;
        }

        Node* nodeAtCurrentLocation() {
            QList current_Items = scene()->items(cursor->scenePos());
            for (QGraphicsItem* item : current_Items)
            {
                if (item != cursor && item->type() == QGraphicsTextItem::Type)
                {
                    return static_cast<Node*>(item);
                }
            }
            return nullptr;
        }

        void updateStatusBar() {
            QString statusText;

            switch (mode) {
                case NORMAL_MODE:
                    statusText = "Mode: Normal";
                    break;
                case NODE_MODE:
                    statusText = "Mode: Node (Press any key to type)";
                    break;
                case EDGE_MODE:
                    statusText = "Mode: Edge (Select two nodes to connect)";
                    break;
            }
    
            statusBar->showMessage(statusText);
        }

        void toggleGrid() {
            if(gridVisible){
                for(auto line : gridLines){
                    line->setVisible(!gridVisible);
                }
                gridVisible = false;
            }
            else
            {
                for(auto line : gridLines){
                    line->setVisible(!gridVisible);
                }
                gridVisible = true;
            }
        }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // QApplication::styleHints()->setColorScheme(Qt::ColorScheme::Light);
    
    QGraphicsScene scene(0, 0, sceneWidth, sceneHeight);

    EditorView view(&scene);
    view.createGrid();
    view.show();
    view.setDragMode(QGraphicsView::ScrollHandDrag);

    return app.exec();
}

