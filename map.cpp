#include "map.h"

#define coord               QPair<int,int>

#define WALL_COLOR          "black"
#define SPACE_COLOR         "white"

#define OPEN_COLOR          "orange"
#define CLOSE_COLOR         "yellow"

#define PATH_COLOR          "blue"

void Map::draw_pixel( QSharedPointer<QPainter> p, int x, int y, QString color){
    int r,g,b;
    QColor(color).getRgb(&r, &g, &b);

    draw_pixel(p, x, y, r, g, b);
}
void Map::draw_pixel( QPainter &p, int x, int y, QString color){
    int r,g,b;
    QColor(color).getRgb(&r, &g, &b);

    draw_pixel(p, x, y, r, g, b);
}


void Map::draw_pixel( QSharedPointer<QPainter> p, int x, int y, int r, int g, int b){
    p->setBrush(QBrush(QColor(r,g,b)));
    p->setPen(QPen(QColor(r,g,b)));

    QRect rect(x*PSIZE,y*PSIZE, PSIZE-1, PSIZE-1);
    p->drawRect(rect);
    p->fillRect(rect, p->brush());

}
void Map::draw_pixel( QPainter &p, int x, int y, int r, int g, int b){
    p.setBrush(QBrush(QColor(r,g,b)));
    p.setPen(QPen(QColor(r,g,b)));

    QRect rect(x*PSIZE,y*PSIZE, PSIZE-1, PSIZE-1);
    p.drawRect(rect);
    p.fillRect(rect, p.brush());

}


void Map::draw_wall( QPainter &p, int x, int y){
    draw_pixel(p, x, y, WALL_COLOR);
}
void Map::draw_space( QPainter &p, int x, int y){
    draw_pixel(p, x, y, SPACE_COLOR);
}

void Map::draw_start( QPainter &p){
    draw_pixel(p, Sx,Sy, "green");
}
void Map::draw_end( QPainter &p){
    draw_pixel(p, Ex,Ey, "red");
}

void Map::drawOpen(int x, int y){
    /*QPainter p;
    if (!p.begin( curr_image.get()))
        throw QString("QPainter init error.");*/

    draw_pixel(painter, x, y, OPEN_COLOR);
    //p.end();
}
void Map::drawClose(int x, int y){
    /*QPainter p;
    if (!p.begin( curr_image.get()))
        throw QString("QPainter init error.");*/
    draw_pixel(painter, x, y, CLOSE_COLOR);


    //p.end();
}
void Map::drawPath(QVector<QSharedPointer<Node>> & path){
    /*QPainter p;
    if (!p.begin( curr_image.get()))
        throw QString("QPainter init error.");*/
    foreach (auto & node, path) {
        draw_pixel(painter, node->getX(), node->getY(), PATH_COLOR);
    }
    //p.end();
}


void Map::generateGraph(){
    // Build graph
    QSharedPointer<Node> root = QSharedPointer<Node>(new Node(Sx,Sy));
    graph = QSharedPointer<Graph>(new Graph(root));

    QMap<coord,QSharedPointer<Node>> found = QMap<coord,QSharedPointer<Node>>();
    QQueue<QSharedPointer<Node>> qu = QQueue<QSharedPointer<Node>>();
    qu.push_back(root);
    found.insert({root->getX(), root->getY()}, root);
    // discovery loop
    while(!qu.empty()){
        QSharedPointer<Node> curr_node = qu.last();
        qu.pop_back();

        QVector<coord> surr{
            {curr_node->getX()-1, curr_node->getY()},
            {curr_node->getX(), curr_node->getY()-1},
            {curr_node->getX()+1, curr_node->getY()},
            {curr_node->getX(), curr_node->getY()+1}
        };
        foreach (auto &pnt, surr) {
            if(pnt.first >= 0 && pnt.second >= 0 && pnt.first < X && pnt.second < Y && !wall(pnt.first, pnt.second)){
                QSharedPointer<Node> node;
                if (found.contains({pnt.first, pnt.second})){
                    auto it = found.find(pnt);
                    node = it.value();
                }
                else{
                    node = QSharedPointer<Node>(new Node(pnt.first, pnt.second));
                    found.insert({pnt.first, pnt.second}, node);
                    qu.push_back(node);
                }
                // add adj
                curr_node->addAdj(node);
                node->addAdj(curr_node);

                if(node->getX() == Ex && node->getY() == Ey){
                    graph->setEnd(node);
                }
            }
        }
    }

    qDebug() << "Nodes: " << found.size();
}
void Map::generateImage(){
    // Render IMAGE
    qDebug() << "Img " <<  X*PSIZE;
    image = QSharedPointer<QImage>( new QImage( X*PSIZE, Y*PSIZE, QImage::Format_RGB888));


    QPainter p;
    if (!p.begin( image.get()))
        throw QString("QPainter init error.");

    // vytvoř mapu pixelů obrázku podle bludiště
    for (int y=0; y<Y; y++) {
        for (int x=0; x<X; x++) {
            if (bludisteStr[y][x] == 'X')
                draw_wall(  p, x, y);
            else
                draw_space(  p, x, y);
        }
    }

    // ještě označení startu a cíle
    draw_start(p);
    draw_end(p);

    p.end();
}

Map::Map(QString fileName) : curr_image(nullptr) {

    QFile textFile( fileName);
    if (!textFile.open( QIODevice::ReadOnly))
        throw QString("Soubor s bludištěm nelze otevřít.");

    // bludiště se načte do seznamu stringů
    QTextStream textStream(&textFile);
    QString line;
    while (true)
    {
        // načtení řádky ze souboru
        line = textStream.readLine();

        if (line.isNull() || line[0] != 'X') break;
        bludisteStr.append( line);
    }
    QString startLine = line;
    if (startLine.isNull() || startLine.length() == 0){
        throw QString("Neni start.");
    }
    QStringList lineSecs = startLine.split(' ');
    Sx = lineSecs[1].trimmed().removeLast().toInt();
    Sy = lineSecs[2].trimmed().toInt();

    QString endLine = textStream.readLine();
    if (endLine.isNull() || endLine.length() == 0){
        throw QString("Neni konec.");
    }
    lineSecs = endLine.split(' ');
    Ex = lineSecs[1].trimmed().removeLast().toInt();
    Ey = lineSecs[2].trimmed().toInt();

    // načtení rozměrů bludiště
    Y = bludisteStr.count();
    if (Y == 0)
        throw QString("Soubor s bludištěm je prázdný.");

    X = bludisteStr[0].length();
    if (X == 0)
        throw QString("Bludiště je prázdné.");

    qDebug() << "Size: " << X << " " << Y;

    // GRAPH
    generateGraph();

    // IMAGE
    generateImage();

    curr_image = QSharedPointer<QImage>(new QImage(*image));
    curr_image->save("testtest.png");

    painter = QSharedPointer<QPainter>(new QPainter());
    painter->begin(curr_image.get());
}


int Map::getX() const{
    return X;
}
int Map::getY() const{
    return Y;
}
int Map::getSx() const{
    return Sx;
}
int Map::getSy() const{
    return Sy;
}
int Map::getEx() const{
    return Ex;
}
int Map::getEy() const{
    return Ey;
}
const QSharedPointer<QImage> Map::getImage() const{
    return curr_image;
}

const QSharedPointer<Graph> Map::getGraph() const{
    return graph;
}

bool Map::wall( int x, int y) const{
    if (bludisteStr[y][x] == 'X')
        return true;
    return false;
}

