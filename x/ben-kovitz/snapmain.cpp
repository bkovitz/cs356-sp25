#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QApplication>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>

std::ostream& operator<<(std::ostream& os, const QPointF& point) {
  os << "(" << point.x() << ", " << point.y() << ")";
  return os;
}

std::ostream& operator<<(std::ostream& os, const QRectF& rect) {
  os << "{" << rect.x() << ", " << rect.y() << ", " << rect.width() << ", " << rect.height() << "}";
  return os;
}

class Block : public QGraphicsRectItem {
  QAbstractGraphicsShapeItem* snapHint = nullptr;
public:
  Block(QGraphicsItem* parent = nullptr) : QGraphicsRectItem(parent) {
    setRect(0, 0, 100, 100);
    setBrush(Qt::blue);
    setFlag(QGraphicsItem::ItemIsMovable);
  }

  QPointF loc() const {
    return QPointF(
      boundingRect().center().x() + pos().x(),
      boundingRect().center().y() + pos().y()
    );
  }

  void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
    QGraphicsRectItem::mouseMoveEvent(event);
    addSnapHint(event);
    std::cout << "pos: " << pos() << ' ' << "loc: " << loc() << std::endl;
  }

  // NEXT write pos2loc

  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
    QGraphicsRectItem::mouseReleaseEvent(event);
    removeSnapHint();
    setPos(nearestSnapPoint(event));
    std::cout << "pos: " << pos() << ' ' << "loc: " << loc() << std::endl;
  }

  void addSnapHint(QGraphicsSceneMouseEvent* event) {
    removeSnapHint();
    auto snapPoint = nearestSnapPoint(event);
    snapHint = new QGraphicsEllipseItem(snapPoint.x(), snapPoint.y(), 3, 3);
    snapHint->setBrush(Qt::red);
    snapHint->setPen(Qt::NoPen);
    scene()->addItem(snapHint);
    //std::cout << boundingRect().center() << std::endl;
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