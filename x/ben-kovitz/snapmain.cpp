#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QApplication>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>

class Block : public QGraphicsRectItem {
  QAbstractGraphicsShapeItem* snapHint = nullptr;
public:
  Block(QGraphicsItem* parent = nullptr) : QGraphicsRectItem(parent) {
    setRect(0, 0, 100, 100);
    setBrush(Qt::blue);
    setFlag(QGraphicsItem::ItemIsMovable);
  }

  void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
    QGraphicsRectItem::mouseMoveEvent(event);
    addSnapHint(event);
  }

  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
    QGraphicsRectItem::mouseReleaseEvent(event);
    removeSnapHint();
    setPos(nearestSnapPoint(event));
  }

  void addSnapHint(QGraphicsSceneMouseEvent* event) {
    removeSnapHint();
    auto snapPoint = nearestSnapPoint(event);
    snapHint = new QGraphicsEllipseItem(snapPoint.x(), snapPoint.y(), 3, 3);
    snapHint->setBrush(Qt::red);
    snapHint->setPen(Qt::NoPen);
    scene()->addItem(snapHint);
  }

  void removeSnapHint() {
    if (snapHint) {
      scene()->removeItem(snapHint);
      delete snapHint;
      snapHint = nullptr;
    }
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