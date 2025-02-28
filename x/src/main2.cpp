#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QApplication>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

using namespace std;
const int gridBlockWidth   = 100;
const int gridBlockHeight  = 100;

const int sceneWidth = 800;
const int sceneHeight = 300;

ostream& operator<<(ostream& os, const QPointF& p) {
    return os << "(" << p.x() << ", " << p.y() << ")";
}

int main(int argc, char *argv[])
{
    class Block : public QGraphicsRectItem
    {
    public:
        Block(QGraphicsItem* parent = nullptr) : QGraphicsRectItem(parent) {}
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
            QGraphicsRectItem::mousePressEvent(event);
            updateShadow();
            
        }
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override
        {
            std::cout << "Block released!" << std::endl;
            QGraphicsRectItem::mouseReleaseEvent(event);
            // setPos(300, 300);

            cout << event->scenePos() << endl;
            setPos(nearestSnapPoint(scenePos()));

            removeShadow();

        }
        void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override
        {
            QGraphicsRectItem::mouseMoveEvent(event);
            updateShadow();

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
                shadow = new QGraphicsRectItem();
                shadow->setRect(rect());
                shadow->setBrush(Qt::NoBrush);
                shadow->setPen(QPen(Qt::black, 1, Qt::DashLine));
                shadow->setPos(pos());
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
        QGraphicsRectItem* shadow = nullptr;
    };

    QApplication app(argc, argv);
    // MainWindow window;
    // window.show();

    QGraphicsScene scene(0, 0, sceneWidth, sceneHeight);
    scene.addText("Hello, world!");

    for(int i = 0; i < sceneWidth / gridBlockWidth; i++) {
        scene.addLine(i * gridBlockWidth, 0, i * gridBlockWidth, sceneHeight, QPen(Qt::gray));
    }
    for(int i = 0; i < sceneHeight / gridBlockHeight; i++) {
        scene.addLine(0, i * gridBlockHeight, sceneWidth, i * gridBlockHeight, QPen(Qt::gray));
    }
    
    // Original block
    Block* block = new Block();
    block->setRect(-50, -50, 100, 100);
    block->setPos(100, 100);
    scene.addItem(block);

    block->setBrush(Qt::blue);
    block->setPen(Qt::NoPen);
    block->setFlag(QGraphicsItem::ItemIsMovable);

    QGraphicsView view(&scene);
    view.show();

    return app.exec();
}