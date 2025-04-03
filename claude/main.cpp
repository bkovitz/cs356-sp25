#include <QApplication>
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QMenuBar>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QKeyEvent>

enum Mode { NormalMode, NodeMode, EdgeMode };

class NodeItem : public QGraphicsEllipseItem {
public:
    explicit NodeItem(QString label, QPointF position)
        : QGraphicsEllipseItem(-25, -25, 50, 50) {
        setPos(position);
        text = new QGraphicsTextItem(label, this);
        text->setPos(-15, -10);
    }

    void setLabel(QString label) { text->setPlainText(label); }
    QString getLabel() const { return text->toPlainText(); }

private:
    QGraphicsTextItem *text;
};

class EdgeItem : public QGraphicsLineItem {
public:
    EdgeItem(NodeItem *start, NodeItem *end) {
        setLine(QLineF(start->pos(), end->pos()));
        startNode = start;
        endNode = end;
    }

    NodeItem *startNode;
    NodeItem *endNode;
};

class GraphScene : public QGraphicsScene {
    Q_OBJECT
public:
    GraphScene(QObject *parent = nullptr) : QGraphicsScene(parent), mode(NormalMode), firstNode(nullptr) {
        cursorPos = QPointF(100, 100);
        cursorItem = addRect(cursorPos.x() - 5, cursorPos.y() - 5, 10, 10, QPen(Qt::black), QBrush(Qt::black));
    }

protected:
    void keyPressEvent(QKeyEvent *event) override {
        if (event->key() == Qt::Key_Left) moveCursor(-100, 0);
        else if (event->key() == Qt::Key_Right) moveCursor(100, 0);
        else if (event->key() == Qt::Key_Up) moveCursor(0, -100);
        else if (event->key() == Qt::Key_Down) moveCursor(0, 100);
        else if (mode == NormalMode) {
            if (event->key() == Qt::Key_N) mode = NodeMode;
            else if (event->key() == Qt::Key_E) mode = EdgeMode;
        } else if (mode == NodeMode) {
            if (event->key() == Qt::Key_Escape) mode = NormalMode;
            else if (event->key() == Qt::Key_Backspace) deleteNode();
            else if (event->key() == Qt::Key_Return) addNode();
            else inputText += event->text();
        } else if (mode == EdgeMode) {
            if (event->key() == Qt::Key_Escape) mode = NormalMode;
            else if (event->key() == Qt::Key_Return) selectNodeForEdge();
            else inputText += event->text();
        }
    }

public:
    void addNode() {
        if (inputText.isEmpty()) return;
        NodeItem *node = new NodeItem(inputText, cursorPos);
        addItem(node);
        nodes.append(node);
        inputText.clear();
    }

    void deleteNode() {
        if (!selectedNode) return;
        removeItem(selectedNode);
        nodes.removeOne(selectedNode);
        delete selectedNode;
        selectedNode = nullptr;
    }

    void selectNodeForEdge() {
        for (NodeItem *node : nodes) {
            if (node->getLabel().startsWith(inputText)) {
                if (!firstNode) {
                    firstNode = node;
                } else {
                    EdgeItem *edge = new EdgeItem(firstNode, node);
                    addItem(edge);
                    edges.append(edge);
                    firstNode = nullptr;
                }
                inputText.clear();
                return;
            }
        }
    }

    void saveGraph(QString filename) {
        QJsonArray nodeArray, edgeArray;

        for (NodeItem *node : nodes) {
            QJsonObject nodeObj;
            nodeObj["label"] = node->getLabel();
            nodeObj["x"] = node->x();
            nodeObj["y"] = node->y();
            nodeArray.append(nodeObj);
        }

        for (EdgeItem *edge : edges) {
            QJsonObject edgeObj;
            edgeObj["start"] = edge->startNode->getLabel();
            edgeObj["end"] = edge->endNode->getLabel();
            edgeArray.append(edgeObj);
        }

        QJsonObject graphObj;
        graphObj["nodes"] = nodeArray;
        graphObj["edges"] = edgeArray;

        QFile file(filename);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(QJsonDocument(graphObj).toJson());
            file.close();
        }
    }

    void loadGraph(QString filename) {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)) return;

        QByteArray jsonData = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        QJsonObject graphObj = doc.object();
        QJsonArray nodeArray = graphObj["nodes"].toArray();
        QJsonArray edgeArray = graphObj["edges"].toArray();

        for (QGraphicsItem *item : items()) removeItem(item);

        nodes.clear();
        edges.clear();

        QMap<QString, NodeItem*> nodeMap;
        for (const QJsonValue &nodeVal : nodeArray) {
            QJsonObject nodeObj = nodeVal.toObject();
            NodeItem *node = new NodeItem(nodeObj["label"].toString(), QPointF(nodeObj["x"].toDouble(), nodeObj["y"].toDouble()));
            addItem(node);
            nodes.append(node);
            nodeMap[node->getLabel()] = node;
        }

        for (const QJsonValue &edgeVal : edgeArray) {
            QJsonObject edgeObj = edgeVal.toObject();
            NodeItem *start = nodeMap.value(edgeObj["start"].toString());
            NodeItem *end = nodeMap.value(edgeObj["end"].toString());
            if (start && end) {
                EdgeItem *edge = new EdgeItem(start, end);
                addItem(edge);
                edges.append(edge);
            }
        }
    }

    void moveCursor(int dx, int dy) {
        cursorPos += QPointF(dx, dy);
        cursorItem->setRect(cursorPos.x() - 5, cursorPos.y() - 5, 10, 10);
    }

private:
    Mode mode;
    QPointF cursorPos;
    QGraphicsRectItem *cursorItem; // Cursor representation
    QString inputText;
    QList<NodeItem*> nodes;
    QList<EdgeItem*> edges;
    NodeItem *selectedNode = nullptr;
    NodeItem *firstNode = nullptr;
};

class GraphEditor : public QMainWindow {
    Q_OBJECT
public:
    GraphEditor(QWidget *parent = nullptr) : QMainWindow(parent) {
        scene = new GraphScene(this);
        view = new QGraphicsView(scene, this);
        setCentralWidget(view);
        resize(800, 600);

        QMenu *fileMenu = menuBar()->addMenu("&File");
        QAction *saveAction = fileMenu->addAction("&Save");
        QAction *loadAction = fileMenu->addAction("&Load");

        connect(saveAction, &QAction::triggered, this, &GraphEditor::saveGraph);
        connect(loadAction, &QAction::triggered, this, &GraphEditor::loadGraph);
    }

private slots:
    void saveGraph() {
        QString filename = QFileDialog::getSaveFileName(this, "Save Graph", "", "Graph Files (*.json)");
        if (!filename.isEmpty()) scene->saveGraph(filename);
    }

    void loadGraph() {
        QString filename = QFileDialog::getOpenFileName(this, "Load Graph", "", "Graph Files (*.json)");
        if (!filename.isEmpty()) scene->loadGraph(filename);
    }

private:
    GraphScene *scene;
    QGraphicsView *view;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    GraphEditor editor;
    editor.show();
    return app.exec();
}

#include "main.moc"
