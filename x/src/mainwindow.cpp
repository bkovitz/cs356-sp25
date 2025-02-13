#include "mainwindow.h"
#include <QDebug>
#include <iostream>

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

void MainWindow::handleButtonClick() {
    std::cout << "Button was clicked!" << std::endl;
    // You can also use Qt's logging system:
    // qDebug() << "Button was clicked!";
}
