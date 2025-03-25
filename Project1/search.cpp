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
    }

    g->findVertex(origin)->setDist(0);
    pq.insert(g->findVertex(origin));

    while(!pq.empty()){
        Vertex<T>* v = pq.extractMin();
        v->setVisited(true);
        for(Edge<T>* e : v->getAdj()){
            if (e->getDest()->isIgnored() || e->isIgnored() || e->getDest()->isVisited()) continue; // Restricted Route or already visited
            if (e->getDest()->getDist() == INF) pq.insert(e->getDest());
            if (e->getDrivingTime() != -1 && relax(e)){ //only driving route for now
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

template <class T>
static std::vector<T> getParkingNodes(Graph<T> * g) {
    std::vector<T> parkingNodes;

    for(Vertex<T>* v : g->getVertexSet()){
        if(v->hasParking()){
            parkingNodes.push_back(v->getInfo());
        }
    }
    return parkingNodes;
}

template <class T>
bool areAdjacent(Graph<T> * g,const int &origin, const int &dest){
    for(Edge<T>* e : g->findVertex(origin)->getAdj()){
        if(e->getDest()== g->findVertex(dest)){
            return true;
        }
    }
    return false;
}


