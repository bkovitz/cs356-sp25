#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT  // Required for Qt's meta-object system

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif
