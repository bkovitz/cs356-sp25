#ifndef MYCANVAS_H
#define MYCANVAS_H

#include <QWidget>
#include <QPen>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QLabel>
#include <QPainter>
#include <QBrush>
#include <QColor>
#include <iostream>


struct ColoredPoints {
    QPen pen;
    QList<QPoint> sPoints;
};


class MyCanvas : public QWidget {
    Q_OBJECT

    public: 
        explicit MyCanvas(QWidget *parent = nullptr);
        ~MyCanvas();
        void setPenColor(const QColor &color);
        void paintEvent(QPaintEvent *event) override;

    protected:
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;

    private:
        QList<ColoredPoints> points;   // list to store points where the mouse has been dragged
        bool drawing;           // a flag to check if user is currently drawing
        QPen myPen;
};


#endif //MYCANVAS_H