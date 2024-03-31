#ifndef ALGO_H
#define ALGO_H

#include <QThread>
#include <QImage>
#include "map.h"
#include "graph.h"

class Algo
{
public:
    Algo(QSharedPointer<Map> map);
    const QSharedPointer<QImage> getImage() const;
    bool isFinished() const;
    virtual void step() = 0;

    bool vizualize_path();
    virtual void getInfo(int * open, int * close, int * path) const = 0;
protected:
    QSharedPointer<Map> map;
    QSharedPointer<Graph> graph;
    bool finished;

    QVector<QSharedPointer<Node>> path;
    QMap<QSharedPointer<Node>,QSharedPointer<Node>> prev;
};


// Algos:

class Bfs : public Algo
{
public:
    Bfs(QSharedPointer<Map> map);
    void step() override;
    void getInfo(int * open, int * close, int * path) const override;
private:
    void openNode(QSharedPointer<Node> node, bool draw = true);
    void closeNode(QSharedPointer<Node> node, bool draw = true);

    QQueue<QSharedPointer<Node>> open;
    QSet<QSharedPointer<Node>> closed;

};



class Dfs : public Algo
{
public:
    Dfs(QSharedPointer<Map> map);
    void step() override;
    void getInfo(int * open, int * close, int * path) const override;

private:
    void openNode(QSharedPointer<Node> node, bool draw = true);
    void closeNode(QSharedPointer<Node> node, bool draw = true);

    QStack<QSharedPointer<Node>> open;
    QSet<QSharedPointer<Node>> closed;

};




class Random : public Algo
{
public:
    Random(QSharedPointer<Map> map);
    void step() override;
    void getInfo(int * open, int * close, int * path) const override;

private:
    void openNode(QSharedPointer<Node> node, bool draw = true);
    void closeNode(QSharedPointer<Node> node, bool draw = true);

    QList<QSharedPointer<Node>> open;
    QSet<QSharedPointer<Node>> closed;

};

class AStar : public Algo
{
public:
    AStar(QSharedPointer<Map> map);
    void step() override;
    void getInfo(int * open, int * close, int * path) const override;

private:
    void openNode(QSharedPointer<Node> node, int d, bool draw = true);
    void closeNode(QSharedPointer<Node> node, bool draw = true);

    QMultiMap<int, QSharedPointer<Node>> open;
    QMap<QSharedPointer<Node>, int> dist;
    QSet<QSharedPointer<Node>> closed;

    int getDist(QSharedPointer<Node> node_a, QSharedPointer<Node> node_b) const;

};

class Greedy : public Algo
{
public:
    Greedy(QSharedPointer<Map> map);
    void step() override;
    void getInfo(int * open, int * close, int * path) const override;

private:
    void openNode(QSharedPointer<Node> node, bool draw = true);
    void closeNode(QSharedPointer<Node> node, bool draw = true);

    QMultiMap<int, QSharedPointer<Node>> open;
    QSet<QSharedPointer<Node>> closed;

    int getDist(QSharedPointer<Node> node_a, QSharedPointer<Node> node_b) const;

};


#endif // ALGO_H
