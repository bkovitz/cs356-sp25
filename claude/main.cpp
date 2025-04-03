#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QTimer>
#include <QVector>
#include <QString>
#include <QPolygonF>

class Node {
public:
    Node(QString label, int x, int y) : label(label), x(x), y(y) {}
    QString label;
    int x, y;
};

class Edge {
public:
    Edge(Node* from, Node* to) : from(from), to(to) {}
    Node* from;
    Node* to;
};

class GraphEditor : public QWidget {
    Q_OBJECT

public:
    GraphEditor(QWidget* parent = nullptr) : QWidget(parent), mode(Mode::Normal), cursor(400, 300) {
        resize(800, 600);
        setWindowTitle("Graph Diagram Editor");

        // Set up status bar at the top of the window
        statusBar = new QStatusBar(this);
        statusBar->setFixedHeight(20);
        statusBar->setStyleSheet("background-color: #d3d3d3;");
        statusBar->move(0, 0); // Move the status bar to the top-left corner
        statusBar->resize(width(), 20); // Make it span the entire width of the window

        // Setup timer to refresh status bar and handle key events
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &GraphEditor::updateStatusBar);
        timer->start(100);
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Draw grid
        if (gridVisible) {
            painter.setPen(QPen(Qt::lightGray, 1, Qt::DotLine));
            for (int i = 0; i < width(); i += 100) {
                painter.drawLine(i, 0, i, height());
                painter.drawLine(0, i, width(), i);
            }
        }

        // Draw nodes
        painter.setPen(QPen(Qt::black));
        for (auto& node : nodes) {
            painter.drawText(node.x - node.label.length() * 4, node.y + 4, node.label);
        }

        // Draw edges
        painter.setPen(QPen(Qt::black));
        for (auto& edge : edges) {
            painter.drawLine(edge.from->x, edge.from->y, edge.to->x, edge.to->y);
        }

        // Draw cursor (triangle pointing up-left)
        QPolygonF cursorShape;
        cursorShape << QPointF(cursor.x(), cursor.y()) << QPointF(cursor.x() + 10, cursor.y() - 10) << QPointF(cursor.x() - 10, cursor.y() - 10);
        painter.setBrush(QBrush(Qt::black));
        painter.drawPolygon(cursorShape);
    }

    void keyPressEvent(QKeyEvent* event) override {
        switch (event->key()) {
        case Qt::Key_Escape:
            handleEscape();
            break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
            handleEnter();
            break;
        case Qt::Key_Up:
            cursor.setY(cursor.y() - 100);
            break;
        case Qt::Key_Down:
            cursor.setY(cursor.y() + 100);
            break;
        case Qt::Key_Left:
            cursor.setX(cursor.x() - 100);
            break;
        case Qt::Key_Right:
            cursor.setX(cursor.x() + 100);
            break;
        case Qt::Key_N:
            if (mode == Mode::Normal) setMode(Mode::Node);
            break;
        case Qt::Key_E:
            if (mode == Mode::Normal) setMode(Mode::Edge);
            break;
        case Qt::Key_G:
            gridVisible = !gridVisible;
            update();
            break;
        default:
            if (mode == Mode::Node) {
                currentText += event->text();
                update();
            } else if (mode == Mode::Edge) {
                currentText += event->text();
                update();
            }
            break;
        }
    }

    void mousePressEvent(QMouseEvent* event) override {
        if (mode == Mode::Edge && event->button() == Qt::LeftButton) {
            handleEdgeSelection(event->pos());
        }
    }

private:
    enum class Mode {
        Normal,
        Node,
        Edge
    };

    Mode mode;
    QPoint cursor;
    QString currentText;
    bool gridVisible = true;
    QVector<Node> nodes;
    QVector<Edge> edges;
    QStatusBar* statusBar;
    QTimer* timer;
    Node* currentNode = nullptr;

    void updateStatusBar() {
        QString statusText;
        switch (mode) {
        case Mode::Normal:
            statusText = "Mode: Normal";
            break;
        case Mode::Node:
            statusText = "Mode: Node (Press any key to type)";
            break;
        case Mode::Edge:
            statusText = "Mode: Edge (Select two nodes to connect)";
            break;
        }

        if (currentNode) {
            statusText += " | Selected Node: " + currentNode->label;
        }

        statusBar->showMessage(statusText);
    }

    void setMode(Mode newMode) {
        mode = newMode;
        currentText.clear();
        update();
    }

    void handleEscape() {
        if (mode == Mode::Node) {
            currentText.clear();
            setMode(Mode::Normal);
        } else if (mode == Mode::Edge) {
            setMode(Mode::Normal);
        }
    }

    void handleEnter() {
        if (mode == Mode::Node) {
            nodes.push_back(Node(currentText, cursor.x(), cursor.y()));
            currentText.clear();
            setMode(Mode::Normal);
        } else if (mode == Mode::Edge) {
            if (currentNode != nullptr) {
                Node* targetNode = findNodeByText(currentText);
                if (targetNode != nullptr && targetNode != currentNode) {
                    edges.push_back(Edge(currentNode, targetNode));
                    currentNode = nullptr;
                    currentText.clear();
                    setMode(Mode::Normal);
                }
            }
        }
    }

    void handleEdgeSelection(const QPoint& pos) {
        Node* node = findNodeAtPosition(pos);
        if (node != nullptr) {
            if (currentNode == nullptr) {
                currentNode = node;
                currentText = node->label;
                update();
            } else if (currentNode != node) {
                edges.push_back(Edge(currentNode, node));
                currentNode = nullptr;
                currentText.clear();
                setMode(Mode::Normal);
            }
        }
    }

    Node* findNodeAtPosition(const QPoint& pos) {
        for (auto& node : nodes) {
            if (QRect(node.x - 10, node.y - 10, 20, 20).contains(pos)) {
                return &node;
            }
        }
        return nullptr;
    }

    Node* findNodeByText(const QString& text) {
        for (auto& node : nodes) {
            if (node.label.startsWith(text, Qt::CaseInsensitive)) {
                return &node;
            }
        }
        return nullptr;
    }
};

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    GraphEditor editor;
    editor.show();

    return a.exec();
}

#include "main.moc"
