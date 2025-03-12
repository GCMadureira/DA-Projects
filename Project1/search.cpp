//
// Created by Gabriel Sousa on 24/02/2025.
//

#include "search.h"


using namespace std;

template <class T>
bool relax(Edge<T> *edge) { // d[u] + w(u,v) < d[v]
    Vertex<T>* neighbor = edge->getDest();
    Vertex<T>* v = edge->getOrig();
    if((v->getDist() + edge->getDrivingTime()) < neighbor->getDist()){
        neighbor->setDist(v->getDist() + edge->getDrivingTime());
        neighbor->setPath(edge);
        return true;
    }
    return false;
}

template <class T>
void dijkstra(Graph<T> * g, const int &origin) {
    if (!g->findVertex(origin)) return;

    std::vector<Vertex<T>*> vertices = g->getVertexSet();
    MutablePriorityQueue<Vertex<T>> pq;
    for(Vertex<T>* v : vertices){
        v->setDist(INF);
        v->setPath(nullptr);
        v->setVisited(false);
        if (!v->isIgnored()) pq.insert(v);
    }

    g->findVertex(origin)->setDist(0);
    pq.decreaseKey(g->findVertex(origin));

    while(!pq.empty()){
        Vertex<T>* v = pq.extractMin();
        for(Edge<T>* e : v->getAdj()){
            if (e->getDest()->isIgnored()) continue;
            if(e->getDrivingTime() != -1 && relax(e)){ //only driving route for now
                pq.decreaseKey(e->getDest());
            }
        }
    }
}

template <class T>
static std::vector<T> getPath(Graph<T> * g, const int &origin, const int &dest, int& pathLength) {
    std::vector<T> path;
    pathLength = 0;
    Vertex<T>* v = g->findVertex(dest);

    if (!v || v->getDist() == INT_MAX) {
        return path;
    }

    while (v) {
        path.push_back(v->getInfo());
        if (v->getInfo() == origin) break;
        pathLength += v->getPath()->getDrivingTime();
        v = v->getPath()->getOrig();
    }

    reverse(path.begin(), path.end());
    return path;
}


