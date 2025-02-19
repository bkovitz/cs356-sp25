#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "MyCanvas.h"

using namespace std;

MyCanvas::MyCanvas(QWidget *parent)
    : QWidget(parent),
      drawing(false)
{
    setFixedSize(800,500);
    
    // create
    myPen.setColor(Qt::white);
    myPen.setWidth(3);
    
    // Initialize with default color point list
    ColoredPoints initialPoints;
    initialPoints.pen = myPen;
    points.append(initialPoints);
}

MyCanvas::~MyCanvas() {}

void MyCanvas::setPenColor(const QColor &color) {
    myPen.setColor(color);
    

    // Check if we already have a point list for this color
    bool colorExists = false;
    for (ColoredPoints &colored : points) {
        if (colored.pen.color() == color) {
            colorExists = true;
            colored.pen = myPen;
            break;
        }
    }
    
    // If not, create a new ColoredPoints struct 
    if (!colorExists) {
        ColoredPoints newColorPoints;
        newColorPoints.pen = myPen;
        points.append(newColorPoints);
    }
    
    update();
}

void MyCanvas::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor(0, 0, 0));

    for (const ColoredPoints &colored : points) {
        painter.setPen(colored.pen);
        
        //take structs sPoints into new list to iterate through
        const QList<QPoint>& colorPoints = colored.sPoints;
        for (int i = 1; i < colorPoints.size(); i++) {
            // make sure point at i or the QPoint before aren't the dummy QPoint
            if (colorPoints[i] != QPoint(-1, -1) && colorPoints[i-1] != QPoint(-1, -1)) {
                // connect point to point before
                painter.drawLine(colorPoints[i-1], colorPoints[i]);
            }
        }
    }
}

void MyCanvas::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        drawing = true;
        bool found = false;
        for (ColoredPoints &colored : points) {
            if (colored.pen.color() == myPen.color()) {
                colored.sPoints.append(event->pos());
                found = true;
                break;
            }
        }
        
        // If no matching color found, create new one
        if (!found) {
            ColoredPoints newColorPoints;
            newColorPoints.pen = myPen;
            newColorPoints.sPoints.append(event->pos());
            points.append(newColorPoints);
        }
        
        update();
    }
}

void MyCanvas::mouseMoveEvent(QMouseEvent *event) {
    if (drawing) {
        for (ColoredPoints &colored : points) {
            if (colored.pen.color() == myPen.color()) {
                colored.sPoints.append(event->pos());
                break;
            }
        }
        update();
    }
}

void MyCanvas::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        drawing = false;
        for (ColoredPoints &colored : points) {
            if (colored.pen.color() == myPen.color()) {
                // insert dummy point at end of QList to break lines
                colored.sPoints.append(QPoint(-1, -1));
                break;
            }
        }
        update();
    }
}

void MyCanvas::keyPressEvent(QKeyEvent *event) {
   // QKeyEvent *ke = static_cast<QKeyEvent *>(event);
   QKeyEvent *ke = event;
    if (ke->key() == Qt::Key_Z) {
        for (ColoredPoints &colored : points) {
           if (colored.pen.color() == myPen.color() && !colored.sPoints.isEmpty()) {
                colored.sPoints.pop_back();
                update();
           }
        }
    }
    if (ke->key() == Qt::Key_1) {
        // switch funtion for color cycle
        setPenColor(Qt::white);
    }
    if (ke->key() == Qt::Key_2) {
        // switch funtion for color cycle
        setPenColor(Qt::red);
    }
    if (ke->key() == Qt::Key_3) {
        // switch funtion for color cycle
        setPenColor(Qt::green);
    }
    if (ke->key() == Qt::Key_4) {
        // switch funtion for color cycle
        setPenColor(Qt::blue);
    }
    if (ke->key() == Qt::Key_0) {
        // switch funtion for color cycle
        setPenColor(Qt::black);
    }

}