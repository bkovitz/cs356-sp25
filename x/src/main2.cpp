#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QApplication>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

int main(int argc, char *argv[])
{
    class Block : public QGraphicsRectItem
    {
    public:
        Block(QGraphicsItem* parent = nullptr) : QGraphicsRectItem(parent) {}
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override
        {
            std::cout << "Block released!" << std::endl;
            QGraphicsRectItem::mouseReleaseEvent(event);
            // setPos(300, 300);
            qreal x = scenePos().x();
            qreal y = scenePos().y();
            setPos(round(x / 100)*100, round(y / 100)*100);
            // setPos(scenePos() / 2);
        }
    };

    QApplication app(argc, argv);
    // MainWindow window;
    // window.show();

    QGraphicsScene scene(0, 0, 800, 600);
    scene.addText("Hello, world!");
    
    Block* block = new Block();
    block->setRect(10, 10, 100, 100);
    scene.addItem(block);
    // QGraphicsRectItem* rect = scene.addRect(10, 10, 100, 100);

    block->setBrush(Qt::blue);
    block->setPen(Qt::NoPen);
    block->setFlag(QGraphicsItem::ItemIsMovable);

    QGraphicsView view(&scene);
    view.show();

    return app.exec();
}