//
// Created by Gabriel Sousa on 24/02/2025.
//

#ifndef PROJECT1_SEARCH_H
#define PROJECT1_SEARCH_H

#include "data_structures/Graph.h"

/**
 * @brief Relaxes an edge if it is part of the new best route to the destination node
 *
 * If the best distance to vertex u plus the distance of the edge (u,v) is smaller than the current best distance to vertex v
 * then relax the edge by replacing the current best distance to v with the best distance to u plus the distance of edge (u,v). <br>
 * By default, the edge is relaxed using the driving time, however if the boolean driving is set to false then only the walking time is considered instead.
 *
 * @param edge - pointer to edge to be relaxed
 * @param driving - boolean that defines whether the driving or walking distance are considered, true by default
 *
 * @return true if the edge was relaxed, false otherwise
 */
template <class T>
bool relax(Edge<T> *edge, bool driving = true);

/**
 * @brief Executes the dijkstra algorithm on graph g from source node origin
 *
 * Starts by setting an initial state of the graph by resetting the vertex class members: dist = INF, path = nullptr, visited = false. <br>
 * It then keeps extracting the closest unprocessed edge to the source from a mutable priority queue, iterating through all the edges adjacent to it.
 * If the edge satisfies every restriction then call relax(), and if the call returns true, decrease the key of the node on the priority queue. <br>
 * By default, only the driving time is considered on the edges, however if the boolean driving is set to false, the walking time is considered instead.
 *
 * @param g - pointer to the graph where the algorithm will be executed
 * @param origin - id of the source node
 * @param driving - boolean that defines whether the driving or walking distance are considered, true by default
 */
template <class T>
void dijkstra(Graph<T> * g, const int &origin, bool driving = true);

/**
 * @brief Generates the path obtained from executing dijkstra()
 *
 * Loops over every edge of the path from the dest node to the origin node.
 * This is done by adding the current node to a vector and then using Vertex::getPath() (or Vertex::getSavedPath() if savedPath = true) to obtain the next node.
 * The vector is then reversed to obtain the correct order. <br>
 * By default, only the normal Vertex#path and Vertex#dist members are used, however if the savedPath boolean is set to true then consider Vertex#savedPath and Vertex#savedDist instead.
 *
 * @param g - pointer to the graph from which the path will be generated
 * @param origin - id of the path's starting node
 * @param dest - id of the path's destination node
 * @param savedPath - boolean that defines whether the saved or the normal path and dist members are considered, false by default
 *
 * @return vector containing the ids of the nodes present in the path, from origin to dest, or an empty vector in case of error
 */
template <class T>
std::vector<T> getPath(Graph<T> * g, const int &origin, const int &dest, bool savedPath = false);

#endif //PROJECT1_SEARCH_H
