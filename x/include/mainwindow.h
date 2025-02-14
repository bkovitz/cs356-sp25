#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QDebug>
#include <QPainter>
#include <QMainWindow>
#include <QPushButton>
#include <QWidget>
#include <QMouseEvent>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    ~MainWindow() = default;

private slots:
    void handleButtonClick();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QPushButton *button;
};

#endif
