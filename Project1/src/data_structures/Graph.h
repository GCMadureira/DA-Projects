//
// Created by Gabriel Sousa on 23/02/2025.
//

#ifndef PROJECT1_GRAPH_H
#define PROJECT1_GRAPH_H

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include "MutablePriorityQueue.h"

template <class T>
class Edge;

#define INF std::numeric_limits<int>::max()

/************************* Vertex  **************************/

template <class T>
class Vertex {
public:
    Vertex(T in);
    Vertex(T in, const std::string& name);
    bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue

    T getInfo() const;
    std::vector<Edge<T> *> getAdj() const;
    bool isVisited() const;
    bool isIgnored() const;
    bool hasParking() const;
    double getDist() const;
    Edge<T> *getPath() const;
    double getSavedDist() const;
    Edge<T> *getSavedPath() const;
    std::string getName() const;

    void setInfo(T info);
    void setVisited(bool visited);
    void setIgnoreFlag(bool ignore);
    void setParking(bool parking);

    void setDist(double dist);
    void setPath(Edge<T> *path);
    void saveDist();
    void savePath();
    void setName(const std::string &name);
    Edge<T> * addEdge(Vertex<T> *d, int drivingTime, int walkingTime);
    bool removeEdge(T in);
    void removeOutgoingEdges();

    friend class MutablePriorityQueue<Vertex>;
protected:
    T info;                ///< info node
    std::string name;  ///< name of the node, optional
    std::vector<Edge<T> *> adj;  ///< outgoing edges

    // auxiliary fields
    bool visited = false; ///< used by dijkstra() to tell what nodes were already visisted
    double dist = 0; ///< used by dijkstra(), represents the smallest distance from the source node
    Edge<T> *path = nullptr; ///< used by dijkstra() and getPath(), represents the edge used get to the node on the best path
    double savedDist = 0; ///< save current dist obtained with dijkstra()
    Edge<T> *savedPath = nullptr; ///< save current path obtained with dijkstra()
    bool ignoreFlag = false; ///< if true then ignore Vertex when searching for path
    bool parking = false; ///< true if the node has parking

    std::vector<Edge<T> *> incoming; ///< incoming edges

    int queueIndex = 0; 		///< required by MutablePriorityQueue and UFDS
};

/********************** Edge  ****************************/

template <class T>
class Edge {
public:
    Edge(Vertex<T> *orig, Vertex<T> *dest, int drivingTime, int walkingTime);

    Vertex<T> * getDest() const;
    bool isIgnored() const;
    Vertex<T> * getOrig() const;
    Edge<T> *getReverse() const;
    int getDrivingTime() const;
    int getWalkingTime() const;

    void setIgnoreFlag(bool ignore);
    void setReverse(Edge<T> *reverse);
    void setDrivingTime(int time);
    void setWalkingTime(int time);
protected:
    Vertex<T> * dest; ///< destination vertex
    int drivingTime; ///< segment driving time
    int walkingTime; ///< segment walking time


    // auxiliary fields
    bool ignoreFlag = false; ///< if true then ignore Edge when searching for path

    // used for bidirectional edges
    Vertex<T> *orig; ///< origin vertex
    Edge<T> *reverse = nullptr; ///< reversed edge
};

/********************** Graph  ****************************/

template <class T>
class Graph {
public:
    /*
    * Auxiliary function to find a vertex with a given the content.
    */
    Vertex<T> *findVertex(const T &in) const;
    /*
     *  Adds a vertex with a given content or info (in) to a graph (this).
     *  Returns true if successful, and false if a vertex with that content already exists.
     */
    bool addVertex(const T &in);
    bool addVertex(const T &in, const std::string &name);
    bool removeVertex(const T &in);

    bool addBidirectionalEdge(const T &sourc, const T &dest, int drivingTime, int walkingTime);

    /*
     * Adds an edge to a graph (this), given the contents of the source and
     * destination vertices and the edge weight (w).
     * Returns true if successful, and false if the source or destination vertex does not exist.
     */
    bool addEdge(const T &sourc, const T &dest, int drivingTime, int walkingTime);
    bool removeEdge(const T &source, const T &dest);
    Edge<T> *findEdge(const T &source, const T &dest) const;

    int getNumVertex() const;
    std::vector<Vertex<T> *> getVertexSet() const;

    bool areAdjacent(const int &origin, const int &dest);
    void setInitialState();

protected:
    std::vector<Vertex<T> *> vertexSet;    ///< vertex set
};


/************************* Vertex  **************************/

/**
 * @brief Vertex constructor from the id
 *
 * @param in - id of the vertex
 */
template <class T>
Vertex<T>::Vertex(T in): info(in) {}

/**
 * @brief Vertex constructor from the id and name
 *
 * @param in - id of the vertex
 * @param name - name of the vertex
 */
template<class T>
Vertex<T>::Vertex(T in, const std::string &name): info(in), name(name) {}

/**
 * @brief Auxiliary function to add an outgoing edge to a vertex (this), with a given destination vertex (d) and edge weight (w)
 *
 * @param d - pointer to destination vertex
 * @param drivingTime - driving time of the new edge
 * @param walkingTime - walking time of the new edge
 *
 * @return pointer to the new edge created
 */
template <class T>
Edge<T> * Vertex<T>::addEdge(Vertex<T> *d, int drivingTime, int walkingTime) {
    auto newEdge = new Edge<T>(this, d, drivingTime,walkingTime);
    adj.push_back(newEdge);
    d->incoming.push_back(newEdge);
    return newEdge;
}

/**
 * @brief Auxiliary function to remove an outgoing edge (with a given destination (d)) from a vertex (this)
 *
 * @param in - id of the destination vertex of the edge to be removed
 *
 * @return true if successful, and false if such edge does not exist
 */
template <class T>
bool Vertex<T>::removeEdge(T in) {
    bool removedEdge = false;
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        Vertex<T> *dest = edge->getDest();
        if (dest->getInfo() == in) {
            it = adj.erase(it);
            deleteEdge(edge);
            removedEdge = true; // allows for multiple edges to connect the same pair of vertices (multigraph)
        }
        else {
            it++;
        }
    }
    return removedEdge;
}


/**
 * @brief Auxiliary function to remove an outgoing edge of a vertex
 */
template <class T>
void Vertex<T>::removeOutgoingEdges() {
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        it = adj.erase(it);
        deleteEdge(edge);
    }
}

/**
 * @brief Compares this vertex with another based on the Vertex::dist member
 *
 * @param vertex - reference to the other vertex object to compare with
 *
 * @return true if this vertex has less dist and false otherwise
 */
template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
    return this->dist < vertex.dist;
}

/**
 * @brief Getter for the Vertex::info private member
 *
 * @return value of Vertex::info member
 */
template <class T>
T Vertex<T>::getInfo() const {
    return this->info;
}

/**
 * @brief Getter for the Vertex::adj private member
 *
 * @return value of Vertex::adj member
 */
template <class T>
std::vector<Edge<T>*> Vertex<T>::getAdj() const {
    return this->adj;
}

/**
 * @brief Getter for the Vertex::visited private member
 *
 * @return value of Vertex::visited member
 */
template <class T>
bool Vertex<T>::isVisited() const {
    return this->visited;
}

/**
 * @brief Getter for the Vertex::dist private member
 *
 * @return value of Vertex::dist member
 */
template <class T>
double Vertex<T>::getDist() const {
    return this->dist;
}

/**
 * @brief Getter for the Vertex::path private member
 *
 * @return value of Vertex::path member
 */
template <class T>
Edge<T> *Vertex<T>::getPath() const {
    return this->path;
}

/**
 * @brief Getter for the Vertex::savedDist private member
 *
 * @return value of Vertex::savedDist member
 */
template <class T>
double Vertex<T>::getSavedDist() const{
    return this->savedDist;
}

/**
 * @brief Getter for the Vertex::savedPath private member
 *
 * @return value of Vertex::savedPath member
 */
template <class T>
Edge<T> *Vertex<T>::getSavedPath() const{
    return this->savedPath;
}

/**
 * @brief Getter for the Vertex::name private member
 *
 * @return value of Vertex::name member
 */
template<class T>
std::string Vertex<T>::getName() const {
    return this->name;
}


/**
 * @brief Setter for the Vertex::ignoreFlag private member
 *
 * @param ignore - new value to be set
 */
template <class T>
void Vertex<T>::setIgnoreFlag(bool ignore) {
    this->ignoreFlag = ignore;
}

/**
 * @brief Setter for the Vertex::parking private member
 *
 * @param parking - new value to be set
 */
template <class T>
void Vertex<T>::setParking(bool parking) {
    this->parking = parking;
}

/**
 * @brief Getter for the Vertex::ignoreFlag private member
 *
 * @return value of Vertex::ignoreFlag member
 */
template <class T>
bool Vertex<T>::isIgnored() const{
    return this->ignoreFlag;
}

/**
 * @brief Getter for the Vertex::parking private member
 *
 * @return value of Vertex::parking member
 */
template <class T>
bool Vertex<T>::hasParking() const{
    return this->parking;
}

/**
 * @brief Setter for the Vertex::info private member
 *
 * @param in - new value to be set
 */
template <class T>
void Vertex<T>::setInfo(T in) {
    this->info = in;
}

/**
 * @brief Setter for the Vertex::visited private member
 *
 * @param visited - new value to be set
 */
template <class T>
void Vertex<T>::setVisited(bool visited) {
    this->visited = visited;
}

/**
 * @brief Setter for the Vertex::dist private member
 *
 * @param dist - new value to be set
 */
template <class T>
void Vertex<T>::setDist(double dist) {
    this->dist = dist;
}

/**
 * @brief Setter for the Vertex::path private member
 *
 * @param path - new value to be set
 */
template <class T>
void Vertex<T>::setPath(Edge<T> *path) {
    this->path = path;
}

/**
 * @brief Saves the current value of the Vertex::dist member on the Vertex::savedDist member
 */
template <class T>
void Vertex<T>::saveDist() {
    this->savedDist = dist;
}

/**
 * @brief Saves the current value of the Vertex::path member on the Vertex::savedPath member
 */
template <class T>
void Vertex<T>::savePath() {
    this->savedPath = path;
}

/**
 * @brief Setter for the Vertex::name private member
 *
 * @param name - new value to be set
 */
template <class T>
void Vertex<T>::setName(const std::string& name) {
    this->name = name;
}

/********************** Edge  ****************************/

/**
 * @brief Edge constructor
 *
 * @param orig - pointer to the origin vertex of the edge
 * @param dest - pointer to the destination vertex of the edge
 * @param drivingTime - driving time of the edge
 * @param walkingTime - walking time of the edge
 */
template <class T>
Edge<T>::Edge(Vertex<T> *orig, Vertex<T> *dest, int drivingTime, int walkingTime)
        : orig(orig), dest(dest), drivingTime(drivingTime), walkingTime(walkingTime) {}

/**
 * @brief Getter for the Edge::dest private member
 *
 * @return value of Edge::dest member
 */
template <class T>
Vertex<T> * Edge<T>::getDest() const {
    return this->dest;
}

/**
 * @brief Getter for the Edge::orig private member
 *
 * @return value of Edge:orig member
 */
template <class T>
Vertex<T> * Edge<T>::getOrig() const {
    return this->orig;
}

/**
 * @brief Getter for the Edge::reverse private member
 *
 * @return value of Edge::reverse member
 */
template <class T>
Edge<T> *Edge<T>::getReverse() const {
    return this->reverse;
}

/**
 * @brief Setter for the Edge::ignoreFlag private member
 *
 * @param ignore - new value to be set
 */
template <class T>
void Edge<T>::setIgnoreFlag(bool ignore) {
    this->ignoreFlag = ignore;
}

/**
 * @brief Getter for the Edge::ignoreFlag private member
 *
 * @return value of Edge::ignoreFlag member
 */
template <class T>
bool Edge<T>::isIgnored() const{
    return this->ignoreFlag;
}

/**
 * @brief Setter for the Edge::reverse private member
 *
 * @param reverse - new value to be set
 */
template <class T>
void Edge<T>::setReverse(Edge<T> *reverse) {
    this->reverse = reverse;
}

/**
 * @brief Getter for the Edge::drivingTime private member
 *
 * @return value of Edge::drivingTime member
 */
template <class T>
int Edge<T>::getDrivingTime() const{
    return this->drivingTime;
}

/**
 * @brief Getter for the Edge::walkingTime private member
 *
 * @return value of Edge::walkingTime member
 */
template <class T>
int Edge<T>::getWalkingTime() const {
    return this->walkingTime;
}

/**
 * @brief Setter for the Edge::drivingTime private member
 *
 * @param time - new value to be set
 */
template <class T>
void Edge<T>::setDrivingTime(int time) {
    this->drivingTime = time;
}

/**
 * @brief Setter for the Edge::walkingTime private member
 *
 * @param time - new value to be set
 */
template <class T>
void Edge<T>::setWalkingTime(int time) {
    this->walkingTime = time;
}

/********************** Graph  ****************************/

/**
 * @brief Getter for the number of nodes of the graph
 *
 * @return number of nodes in the graph
 */
template <class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}

/**
 * @brief Getter for the Graph::vertexSet private member
 *
 * @return value of Graph::vertexSet member
 */
template <class T>
std::vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
}

/**
 * @brief Auxiliary function to find a vertex with a given content
 *
 * @param in - id of the vertex to find
 *
 * @return pointer to vertex if found, nullptr otherwise
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
    for (auto v : vertexSet)
        if (v->getInfo() == in)
            return v;
    return nullptr;
}

/**
 * @brief Adds a vertex with a given content or info (in) to a graph (this)
 *
 * @param in - id of the vertex to add
 *
 * @return true if successful, and false if a vertex with that content already exists
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
    if (findVertex(in) != nullptr)
        return false;
    vertexSet.push_back(new Vertex<T>(in));
    return true;
}

/**
 * @brief Adds a vertex with a given content or info (in) and name (name) to a graph (this)
 *
 * @param in - id of the vertex to add
 * @param name - name of the vertex to add
 *
 * @return true if successful, and false if a vertex with that content already exists
 */
template <class T>
bool Graph<T>::addVertex(const T &in, const std::string &name) {
    if (findVertex(in) != nullptr)
        return false;
    vertexSet.push_back(new Vertex<T>(in, name));
    return true;
}

/**
 * @brief Removes a vertex with a given content (in) from a graph (this), and all outgoing and incoming edges
 *
 * @param in - id of the vertex to remove
 *
 * @return true if successful, and false if such vertex does not exist
 */
template <class T>
bool Graph<T>::removeVertex(const T &in) {
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if ((*it)->getInfo() == in) {
            auto v = *it;
            v->removeOutgoingEdges();
            for (auto u : vertexSet) {
                u->removeEdge(v->getInfo());
            }
            vertexSet.erase(it);
            delete v;
            return true;
        }
    }
    return false;
}

/**
 * @brief Adds an edge to a graph (this), given the contents of the source and destination vertices and the edge driving and walking times
 *
 * @param sourc - id of the source vertex of the new edge
 * @param dest - id of the destination vertex of the new edge
 * @param drivingTime - driving time of the new edge
 * @param walkingTime - walking time of the new edge
 *
 * @return true if successful, and false if the source or destination vertex does not exist
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, int drivingTime, int walkingTime) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, drivingTime, walkingTime);
    return true;
}

/**
 * @brief Removes an edge from a graph (this). The edge is identified by the source (sourc) and destination (dest) contents
 *
 * @param sourc - id of the source vertex of edge to remove
 * @param dest - id of the destination vertex of edge to remove
 *
 * @return true if successful, and false if such edge does not exist
 */
template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
    Vertex<T> * srcVertex = findVertex(sourc);
    if (srcVertex == nullptr) {
        return false;
    }
    return srcVertex->removeEdge(dest);
}

/**
 * @brief Finds an edge identified by the source (sourc) and destination (dest) contents
 *
 * @param sourc - id of the source vertex of edge to find
 * @param dest - id of the destination vertex of edge to find
 *
 * @return pointer to the edge if found, nullptr otherwise
 */
template <class T>
Edge<T>* Graph<T>::findEdge(const T &sourc, const T &dest) const{
    Vertex<T>* srcVertex = findVertex(sourc);
    if (srcVertex == nullptr) return nullptr;
    for (auto e : srcVertex->getAdj()) {
        if (e->getDest()->getInfo() == dest)
            return e;
    }
    return nullptr;
}

/**
 * @brief Adds a bidirectional edge to a graph (this), given the contents of the source and destination vertices and the edge driving and walking times
 *
 * @param sourc - id of the source vertex of the new edge
 * @param dest - id of the destination vertex of the new edge
 * @param drivingTime - driving time of the new edge
 * @param walkingTime - walking time of the new edge
 *
 * @return true if successful, and false if the source or destination vertex does not exist
 */
template <class T>
bool Graph<T>::addBidirectionalEdge(const T &sourc, const T &dest, int drivingTime, int walkingTime) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;

    // Add edges in both directions
    auto e1 = v1->addEdge(v2, drivingTime, walkingTime);
    auto e2 = v2->addEdge(v1, drivingTime, walkingTime);

    e1->setReverse(e2);
    e2->setReverse(e1);

    return true;
}

/**
 * @brief Checks if two vertices identified by the id are adjacent
 *
 * @param origin - id of the source vertex
 * @param dest - id of the destination vertex
 *
 * @return true if both vertices are adjacent, false otherwise
 */
template <class T>
bool Graph<T>::areAdjacent(const int &origin, const int &dest){
    for(Edge<T>* e : findVertex(origin)->getAdj()){
        if(e->getDest() == findVertex(dest)){
            return true;
        }
    }
    return false;
}

/**
 * @brief Sets the ignoreFlag of all the vertices and edges of the graph to false
 */
template<class T>
void Graph<T>::setInitialState() {
    for (auto v : getVertexSet()) {
        v->setIgnoreFlag(false);
        for (auto e : v->getAdj()) e->setIgnoreFlag(false);
    }
}


#endif //PROJECT1_GRAPH_H
