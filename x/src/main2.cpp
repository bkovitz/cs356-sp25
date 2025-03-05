#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QApplication>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QPalette>
#include <QStyle>
#include <QStyleHints>

using namespace std;
const int gridBlockWidth   = 100;
const int gridBlockHeight  = 100;

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
    QApplication::styleHints()->setColorScheme(Qt::ColorScheme::Light);

    QGraphicsScene scene(0, 0, 400, 300);
    scene.addText("Hello, world!");
    
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