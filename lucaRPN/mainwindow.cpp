#include "mainwindow.h"
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    QPushButton *button = new QPushButton("Hello, I'm Qt!", this);
    setCentralWidget(button);
}

MainWindow::~MainWindow() {}
