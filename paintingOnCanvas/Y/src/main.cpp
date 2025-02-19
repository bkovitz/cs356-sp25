#include <QApplication>
#include "MyCanvas.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MyCanvas canvas;
    canvas.show();
    return app.exec();
}