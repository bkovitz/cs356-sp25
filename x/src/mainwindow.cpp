#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setWindowTitle("Qt Example");
    resize(800, 600);
    
    button = new QPushButton("Click Me!", this);
    button->setGeometry(50, 50, 100, 30);
    
    // Connect the button's clicked signal to our slot
    // Arguements: Sender object, signal type, receiver object, slot function
    connect(button, &QPushButton::clicked, this, &MainWindow::handleButtonClick);
}

void MainWindow::paintEvent(QPaintEvent *)
{   
    QPainter p;
    p.begin(this);
    p.fillRect(QRect(100, 30, 50, 100), QColor(255, 0, 0));
    p.end();

}

void MainWindow::handleButtonClick() {
    std::cout << "Button was clicked!" << std::endl;
    // You can also use Qt's logging system:
    // qDebug() << "Button was clicked!";
    button->setText("Clicked");
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    QPointF point = event->position();
    std::cout << "mouse has been pressed" << std::endl;
    std::cout << point.x() << " " << point.y() << std::endl;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    std::cout << "mouse has been moved" << std::endl;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    std::cout << "mouse has been released" << std::endl;
}