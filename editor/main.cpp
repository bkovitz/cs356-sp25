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

using namespace std;
const int gridBlockWidth   = 100;
const int gridBlockHeight  = 100;

const int sceneWidth = 600;
const int sceneHeight = 300;

ostream& operator<<(ostream& os, const QPointF& p) {
    return os << "(" << p.x() << ", " << p.y() << ")";
}

ostream& operator<<(ostream& os, const QRectF& r) {
    return os << "(" << r.x() << ", " << r.y() << ", " << r.width() << ", " << r.height() << ")";
}

ostream& operator<<(ostream& os, const QRect& r) {
    return os << "(" << r.x() << ", " << r.y() << ", " << r.width() << ", " << r.height() << ")";
}

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


class Block : public QGraphicsPolygonItem
    {
    public:
        Block(QGraphicsItem* parent = nullptr) : QGraphicsPolygonItem(parent) {}
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
            QGraphicsPolygonItem::mousePressEvent(event);
            updateShadow();
            
        }
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override
        {
            std::cout << "Block released!" << std::endl;
            QGraphicsPolygonItem::mouseReleaseEvent(event);
            // setPos(300, 300);

            cout << event->pos() << endl;
            setPos(nearestSnapPoint(scenePos()));

            removeShadow();

        }
        void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override
        {
            QGraphicsPolygonItem::mouseMoveEvent(event);
            updateShadow();
            cout << "scene()->sceneRect():" << scene()->sceneRect() << endl;
            // cout << scene()->views()[0]->viewport()->rect() << endl;

            // Printing scene relative coordinates
            cout << scene()->views()[0]->mapToScene(0,0) << endl;

        }
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override
        {
            QGraphicsPolygonItem::mouseDoubleClickEvent(event);

            if (!label)
            {
                label = new ShapeLabel(this);

            }
            label->setFocus();
        }
        QPointF nearestSnapPoint(const QPointF& pos)
        {
            qreal x = pos.x();
            qreal y = pos.y();
            return QPointF(round(x / gridBlockWidth)*gridBlockWidth, round(y / gridBlockHeight)*gridBlockHeight);
        }
        void updateShadow()
        {
            if(!shadow)
            {
                shadow = new QGraphicsPolygonItem();
                shadow->setPolygon(polygon());
                shadow->setBrush(Qt::NoBrush);
                shadow->setPen(QPen(Qt::black, 1, Qt::DashLine));
                shadow->setPos(pos());
                shadow->setZValue(10);
                scene()->addItem(shadow);
            }
            else
            {
                shadow->setPos(nearestSnapPoint(scenePos()));
            }
        }
        void removeShadow()
        {
            if(shadow)
            {
                scene()->removeItem(shadow);
                delete shadow;
                shadow = nullptr;
            }
        }
    private:
        QGraphicsPolygonItem* shadow = nullptr;
        ShapeLabel* label = nullptr;
};



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
        cursor->setZValue(9001);

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
        if(event->key() == Qt::Key_Space)
        {
           // std::cout << "Space key pressed!" << std::endl;
        }
        std::cout << "Key pressed: " << event->key() << std::endl;

        // Original block
        // Block* block = new Block();
        // QPolygonF poly;

        switch (mode)
        {
            case NORMAL_MODE:
                switch(event->key())
                {
                    case Qt::Key_N:
                        mode = NODE_MODE;
                        break;
                    case Qt::Key_E:
                        mode = EDGE_MODE;
                        break;
                    case Qt::Key_G:
                        toggleGrid();
                        break;
                }
                break;
            case NODE_MODE:
                switch(event->key())
                {
                    case Qt::Key_Escape:
                        mode = NORMAL_MODE;
                        break;
                }
                break;
            case EDGE_MODE:
                switch(event->key())
                {
                    case Qt::Key_Escape:
                        mode = NORMAL_MODE;
                        break;
                }
                break;
        }

        updateStatusBar();

        switch (event->key())
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
            // case Qt::Key_R:
            //     poly << QPointF(-75, -50) << QPointF(75, -50) << QPointF(75, 50) << QPointF(-75, 50);
            //     break;
            // case Qt::Key_T:
            //     poly << QPointF(-50, 50) << QPointF(0, -50) << QPointF(50, 50);
            //     break;
            // case Qt::Key_C:
            //     {
            //     QPainterPath path;
            //     path.addEllipse(-50, -50, 100, 100);
            //     poly = path.toFillPolygon();
            //     }
            //     break;
            // case Qt::Key_D:
            //     // poly << QPointF(-50, 0) << QPointF(0, -50) << QPointF(50, 0) << QPointF(0, 50);
            //     {
            //         QPainterPath path;
            //         path.addEllipse(-10, -10, 20, 20);
            //         poly = path.toFillPolygon();
            //     }
            //     break;
            // case Qt::Key_S:
            //     poly << QPointF(-50, -50) << QPointF(50, -50) << QPointF(50, 50) << QPointF(-50, 50);
            //     break;
            case Qt::Key_X:
                QList current_Items = scene()->items(cursor->scenePos());
                for (QGraphicsItem* item : current_Items)
                {
                    if (item != cursor && item->type() == QGraphicsPolygonItem::Type)
        
                    {
                        scene()->removeItem(item);
                        delete item;
                        break;
                    }
                }
            
                //scene()->removeItem(scene()->itemAt(cursor->scenePos(), QTransform()));
            
                break;

            /* case Qt::Key_V:
            {
                scene()->removeItem(cursor);
                cursor = nullptr;
                    break;
            }*/
        }

        
        // block->setPolygon(poly);
        // block->setPos(cursor->scenePos().x(), cursor->scenePos().y());
        // scene()->addItem(block);

        // block->setBrush(Qt::blue);
        // block->setPen(Qt::NoPen);
        // block->setFlag(QGraphicsItem::ItemIsMovable);
    }

    private:
        QGraphicsPolygonItem* cursor;
        enum { NORMAL_MODE, NODE_MODE, EDGE_MODE } mode = NORMAL_MODE;

        QStatusBar* statusBar;

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
    
            // if (currentNode) {
            //     statusText += " | Selected Node: " + currentNode->label;
            // }
    
            statusBar->showMessage(statusText);
        }

        void toggleGrid(){
            
        }
        
};




int main(int argc, char *argv[])
{
    

    QApplication app(argc, argv);
    // QApplication::styleHints()->setColorScheme(Qt::ColorScheme::Light);
    
    QGraphicsScene scene(0, 0, sceneWidth, sceneHeight);
    scene.addText("Hello, world!");

    // This is the line drawing code (Might need to use paintEvent in QGraphicsScene)
    
    // Two Possible Approaches (For attempting implementation from home before next Tuesday):

    // 1) Subclass for QGraphicsView and QGraphicsScene. Override methods so view called scene when rect changes.
    // 2) New Class: ViewMonitor: Connects signal or property between a view and a scene so scene is notified when rect changes.
    for(int i = 0; i < sceneWidth / gridBlockWidth; i++) {
        scene.addLine(i * gridBlockWidth, 0, i * gridBlockWidth, sceneHeight, QPen(Qt::gray));
    }
    for(int i = 0; i < sceneHeight / gridBlockHeight; i++) {
        scene.addLine(0, i * gridBlockHeight, sceneWidth, i * gridBlockHeight, QPen(Qt::gray));
    }


    
    EditorView view(&scene);
    view.show();
    view.setDragMode(QGraphicsView::ScrollHandDrag);

    return app.exec();
}

