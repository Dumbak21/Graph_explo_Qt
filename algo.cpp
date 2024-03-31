#include "algo.h"

Algo::Algo(QSharedPointer<Map> map) : map(map), finished(false){
    graph = map->getGraph();
}

const QSharedPointer<QImage> Algo::getImage() const{
    return map->getImage();
}

bool Algo::isFinished() const{
    return finished;
}

bool Algo::vizualize_path(){
    path.clear();
    if(!finished){
        throw "Vizualizing unfinished algo";
    }
    auto res = prev.find(graph->getEnd());

    if(!prev.contains(graph->getEnd())){
        return false;
    }


    QSharedPointer<Node> curr_node = graph->getEnd();
    while(true){
        path.push_back(curr_node);
        if(!prev.contains(curr_node)){
            break;
        }
        curr_node = prev.find(curr_node).value();
    }
    if(path.length() > 2){
        path.removeFirst();
        path.removeLast();
    }
    map->drawPath(path);

    return true;
}


void Bfs::getInfo(int * open_cnt, int * close_cnt, int * path_cnt) const{
    *open_cnt = open.size();
    *close_cnt = closed.size();
    *path_cnt = path.size();
}
void Dfs::getInfo(int * open_cnt, int * close_cnt, int * path_cnt) const{
    *open_cnt = open.size();
    *close_cnt = closed.size();
    *path_cnt = path.size();
}
void Random::getInfo(int * open_cnt, int * close_cnt, int * path_cnt) const{
    *open_cnt = open.size();
    *close_cnt = closed.size();
    *path_cnt = path.size();
}
void Greedy::getInfo(int * open_cnt, int * close_cnt, int * path_cnt) const{
    *open_cnt = open.size();
    *close_cnt = closed.size();
    *path_cnt = path.size();
}
void AStar::getInfo(int * open_cnt, int * close_cnt, int * path_cnt) const{
    *open_cnt = open.size();
    *close_cnt = closed.size();
    *path_cnt = path.size();
}



// BFS

Bfs::Bfs(QSharedPointer<Map> map) : Algo(map){
    openNode(graph->getRoot(), false);
    if(graph->getRoot() == graph->getEnd()){
        finished = true;
    }
}
void Bfs::openNode(QSharedPointer<Node> node, bool draw){
    open.enqueue(node);
    if(draw){
        map->drawOpen(node->getX(), node->getY());
    }
}
void Bfs::closeNode(QSharedPointer<Node> node, bool draw){
    closed.insert(node);
    if(draw){
        map->drawClose(node->getX(), node->getY());
    }
}

void Bfs::step(){
    if(open.isEmpty()){
        finished = true;
        return; // reconstruct
    }
    auto curr_node = open.dequeue();

    auto adjNodes = curr_node->getAdj();
    foreach (auto & node, adjNodes) {
        if (!open.contains(node) && !closed.contains(node)){
            prev.insert(node, curr_node);

            if(node == graph->getEnd()){
                // FINISH
                finished = true;
            }
            else{
                openNode(node, curr_node==graph->getEnd() || curr_node==graph->getRoot() ? false : true);
            }
        }
    }

    closeNode(curr_node, curr_node==graph->getEnd() || curr_node==graph->getRoot() ? false : true);

}






// DFS

Dfs::Dfs(QSharedPointer<Map> map) : Algo(map){
    openNode(graph->getRoot(), false);
    if(graph->getRoot() == graph->getEnd()){
        finished = true;
    }
}
void Dfs::openNode(QSharedPointer<Node> node, bool draw){
    open.push(node);
    if(draw){
        map->drawOpen(node->getX(), node->getY());
    }
}
void Dfs::closeNode(QSharedPointer<Node> node, bool draw){
    closed.insert(node);
    if(draw){
        map->drawClose(node->getX(), node->getY());
    }
}

void Dfs::step(){

    if(open.isEmpty()){
        finished = true;
        return; // reconstruct
    }
    auto curr_node = open.pop();

    auto adjNodes = curr_node->getAdj();
    foreach (auto & node, adjNodes) {
        if (!open.contains(node) && !closed.contains(node)){
            prev.insert(node, curr_node);

            if(node == graph->getEnd()){
                // FINISH
                finished = true;
            }
            else{
                openNode(node, curr_node==graph->getEnd() || curr_node==graph->getRoot() ? false : true);
            }
        }
    }

    closeNode(curr_node, curr_node==graph->getEnd() || curr_node==graph->getRoot() ? false : true);
}


// RAND



Random::Random(QSharedPointer<Map> map) : Algo(map){
    openNode(graph->getRoot(), false);
    if(graph->getRoot() == graph->getEnd()){
        finished = true;
    }
}
void Random::openNode(QSharedPointer<Node> node, bool draw){
    open.push_back(node);
    if(draw){
        map->drawOpen(node->getX(), node->getY());
    }
}
void Random::closeNode(QSharedPointer<Node> node, bool draw){
    closed.insert(node);
    if(draw){
        map->drawClose(node->getX(), node->getY());
    }
}

void Random::step(){
    if(open.isEmpty()){
        finished = true;
        return; // reconstruct
    }

    QRandomGenerator rand;
    int rand_i = rand.bounded(0, open.size());
    auto curr_node = open.at(rand_i);


    auto adjNodes = curr_node->getAdj();
    foreach (auto & node, adjNodes) {
        if (!open.contains(node) && !closed.contains(node)){
            prev.insert(node, curr_node);

            if(node == graph->getEnd()){
                // FINISH
                finished = true;
            }
            else{
                openNode(node, curr_node==graph->getEnd() || curr_node==graph->getRoot() ? false : true);
            }
        }
    }

    open.removeAt(rand_i);
    closeNode(curr_node, curr_node==graph->getEnd() || curr_node==graph->getRoot() ? false : true);
}







// Greedy
int Greedy::getDist(QSharedPointer<Node> node_a, QSharedPointer<Node> node_b) const{
    return abs(node_a->getX() - node_b->getX()) + abs(node_a->getY() - node_b->getY());
}

Greedy::Greedy(QSharedPointer<Map> map) : Algo(map){
    openNode(graph->getRoot(), false);
    if(graph->getRoot() == graph->getEnd()){
        finished = true;
    }
}


void Greedy::openNode(QSharedPointer<Node> node, bool draw){
    int dist = getDist(node, graph->getEnd());
    open.insert(dist,node);
    if(draw){
        map->drawOpen(node->getX(), node->getY());
    }
}
void Greedy::closeNode(QSharedPointer<Node> node, bool draw){
    closed.insert(node);
    if(draw){
        map->drawClose(node->getX(), node->getY());
    }
}

void Greedy::step(){
    if(open.isEmpty()){
        finished = true;
        return; // reconstruct
    }
    auto curr_node = open.first();
    int min_i = open.firstKey();
    open.remove(min_i, curr_node);


    auto adjNodes = curr_node->getAdj();
    foreach (auto & node, adjNodes) {
        if (!open.values().contains(node) && !closed.contains(node)){
            prev.insert(node, curr_node);

            if(node == graph->getEnd()){
                // FINISH
                finished = true;
            }
            else{
                openNode(node, curr_node==graph->getEnd() || curr_node==graph->getRoot() ? false : true);
            }
        }
    }

    closeNode(curr_node, curr_node==graph->getEnd() || curr_node==graph->getRoot() ? false : true);
}




// AStar

int AStar::getDist(QSharedPointer<Node> node_a, QSharedPointer<Node> node_b) const{
    return abs(node_a->getX() - node_b->getX()) + abs(node_a->getY() - node_b->getY());
}

AStar::AStar(QSharedPointer<Map> map) : Algo(map){
    dist.insert(graph->getRoot(), 0);
    openNode(graph->getRoot(), 0, false);
    if(graph->getRoot() == graph->getEnd()){
        finished = true;
    }
}


void AStar::openNode(QSharedPointer<Node> node, int d,  bool draw){
    int dist = getDist(node, graph->getEnd());
    open.insert(dist + d,node);
    if(draw){
        map->drawOpen(node->getX(), node->getY());
    }
}
void AStar::closeNode(QSharedPointer<Node> node, bool draw){
    closed.insert(node);
    if(draw){
        map->drawClose(node->getX(), node->getY());
    }
}

void AStar::step(){
    if(open.isEmpty()){
        finished = true;
        return; // reconstruct
    }

    auto curr_node = open.first();
    int min_i = open.firstKey();
    open.remove(min_i, curr_node);


    auto adjNodes = curr_node->getAdj();
    foreach (auto & node, adjNodes) {
        if (!closed.contains(node)){

            if(node == graph->getEnd()){
                prev.insert(node, curr_node);
                dist.insert(node, *dist.find(curr_node)+1);

                // FINISH
                finished = true;
                break;
            }
            else{

                if(open.values().contains(node)){
                    int temp_i = -1;
                    for (int i = 0; i < open.values().size(); ++i) {
                        if(open.values().at(i) == node){
                            temp_i = i;
                            break;
                        }

                    }
                    int key = open.keys().at(temp_i);
                    open.remove(key, node);

                    prev.remove(node);
                    dist.remove(node);
                }
                openNode(node, *dist.find(curr_node)+1 , curr_node==graph->getEnd() || curr_node==graph->getRoot() ? false : true);

                prev.insert(node, curr_node);
                dist.insert(node, *dist.find(curr_node)+1);
            }
        }
    }

    closeNode(curr_node, curr_node==graph->getEnd() || curr_node==graph->getRoot() ? false : true);
}








