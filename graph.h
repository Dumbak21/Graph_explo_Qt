#ifndef GRAPH_H
#define GRAPH_H

#include <QVector>
#include <QSet>
#include <QSharedPointer>

class Node
{
public:
    Node(int x, int y, char type=' ');
    const QSet<QSharedPointer<Node>> getAdj() const;
    const int getX() const;
    const int getY() const;

    const void addAdj( QSharedPointer<Node> node);
private:
    int x,y;
    char type;

    QSet<QSharedPointer<Node>> adjNodes;
};

class Graph
{
public:
    Graph(QSharedPointer<Node> root);
    const QSharedPointer<Node> getRoot() const;
    const QSharedPointer<Node> getEnd() const;
    void setEnd(QSharedPointer<Node> node);

private:
    QSharedPointer<Node> rootNode;
    QSharedPointer<Node> endNode;
};

#endif // GRAPH_H
