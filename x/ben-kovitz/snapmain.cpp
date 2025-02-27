#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QApplication>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

class Block : public QGraphicsRectItem {
public:
  Block(QGraphicsItem* parent = nullptr) : QGraphicsRectItem(parent) {
    setRect(0, 0, 100, 100);
    setBrush(Qt::blue);
    setFlag(QGraphicsItem::ItemIsMovable);
  }

  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
    QGraphicsRectItem::mouseReleaseEvent(event);
    // qreal x = event->scenePos().x();
    // qreal y = event->scenePos().y();
    // setPos(round(x / 100) * 100, round(y / 100) * 100);
    setPos(nearestSnapPoint(event));
  }

  QPointF nearestSnapPoint(QGraphicsSceneMouseEvent* event) {
    qreal x = event->scenePos().x();
    qreal y = event->scenePos().y();
    return QPointF(round(x / 100) * 100, round(y / 100) * 100);
  }
};

int main(int argc, char** argv) {
  QApplication app(argc, argv);
  QGraphicsScene scene(0, 0, 500, 500);
  
  Block* block = new Block();
  scene.addItem(block);

  QGraphicsView view(&scene);
  view.show();
  return app.exec();
}