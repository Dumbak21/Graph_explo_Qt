#include "graph.h"
#include <QSharedPointer>

Node::Node(int x, int y, const char type) : x(x), y(y), type(type){}

const QSet<QSharedPointer<Node>> Node::getAdj() const{
    return adjNodes;
}

const void Node::addAdj( QSharedPointer<Node> node){
    adjNodes.insert(node);
}


const int Node::getX() const{
    return x;
}
const int Node::getY() const{
    return y;
}

void Graph::setEnd(QSharedPointer<Node> node){
    endNode = node;
}



Graph::Graph(QSharedPointer<Node> root) {
    rootNode = root;
}

const QSharedPointer<Node> Graph::getRoot() const{
    return rootNode;
}

const QSharedPointer<Node> Graph::getEnd() const{
    return endNode;
}
