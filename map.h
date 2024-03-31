#ifndef MAP_H
#define MAP_H

#include <QtCore>
#include <QPainter>
#include "graph.h"
#include <QSharedPointer>

#include <QVector>
#include <QQueue>
#include <QMap>
#include <QPair>

#define PSIZE   2

class Map
{
public:
    Map(QString fileName);
    int getX() const;
    int getY() const;
    int getSx() const;
    int getSy() const;
    int getEx() const;
    int getEy() const;
    const QSharedPointer<QImage> getImage() const;
    const QSharedPointer<Graph> getGraph() const;
    bool wall( int x, int y) const;

    void drawOpen(int x, int y);
    void drawClose(int x, int y);
    void drawPath(QVector<QSharedPointer<Node>> & path);

private:
    int X, Y;                   // rozměry bludiště
    int Sx, Sy;                 // souřadnice startu
    int Ex, Ey;                 // souřadnice cíle

    QStringList bludisteStr;    // seznam řádek (stringů) s bludištěm
    QSharedPointer<QImage> image;              // generovaný obrázek
    QSharedPointer<QImage> curr_image;

    QSharedPointer<Graph> graph;

    QSharedPointer<QPainter> painter;
    void draw_pixel(QSharedPointer<QPainter> p, int x, int y, QString color);
    void draw_pixel( QPainter &p, int x, int y, QString color);

    void draw_pixel(QSharedPointer<QPainter> p, int x, int y, int r, int g, int b);
    void draw_pixel( QPainter &p, int x, int y, int r, int g, int b);


    void draw_wall( QPainter &p, int x, int y);
    void draw_space( QPainter &p, int x, int y);

    void draw_start( QPainter &p);
    void draw_end( QPainter &p);

    void generateGraph();
    void generateImage();
};

#endif // MAP_H
