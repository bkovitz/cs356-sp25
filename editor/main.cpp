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
};




class EditorView : public QGraphicsView
{
public:
    EditorView(QGraphicsScene* scene) : QGraphicsView(scene) {}
    void keyPressEvent(QKeyEvent* event) override
    {
        QGraphicsView::keyPressEvent(event);
        if(event->key() == Qt::Key_Space)
        {
           // std::cout << "Space key pressed!" << std::endl;
        }
        std::cout << "Key pressed: " << event->key() << std::endl;

        // Original block
        Block* block = new Block();
        QPolygonF poly;

        switch (event->key())
        {
            case Qt::Key_R:
                poly << QPointF(-75, -50) << QPointF(75, -50) << QPointF(75, 50) << QPointF(-75, 50);
                break;
            case Qt::Key_T:
                poly << QPointF(-50, 50) << QPointF(0, -50) << QPointF(50, 50);
                break;
            case Qt::Key_C:
                {
                QPainterPath path;
                path.addEllipse(-50, -50, 100, 100);
                poly = path.toFillPolygon();
                }
                break;
            case Qt::Key_D:
                poly << QPointF(-50, 0) << QPointF(0, -50) << QPointF(50, 0) << QPointF(0, 50);
                break;
            case Qt::Key_S:
                poly << QPointF(-50, -50) << QPointF(50, -50) << QPointF(50, 50) << QPointF(-50, 50);
                break;
        }

        
        block->setPolygon(poly);
        block->setPos(100, 100);
        scene()->addItem(block);

        block->setBrush(Qt::blue);
        block->setPen(Qt::NoPen);
        block->setFlag(QGraphicsItem::ItemIsMovable);
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

