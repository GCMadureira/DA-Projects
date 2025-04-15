#ifndef BATCHMODE_H
#define BATCHMODE_H

#include "data_structures/Graph.h"

/**
 * @brief Finds the best path on a normal driving route plan with the data on input.txt
 *
 * Parses the input file to load route input data and then calculates the best route with dijkstra(). Finally, it saves the output on the output.txt file. <br>
 * The route is calculated by calling dijkstra() once to get the best path from the startNode to the endNode, then it sets the ignore flag to true on every used node.
 * It then calculates the second best route by calling dijkstra() again. <br>
 * Stops execution midway if:
 * - it cannot open the input or the output file
 * - the start or end nodes are invalid
 * Since dijkstra() is always called twice, the time complexity of this algorithm is O((E+V)logV) and the other parts such as input parsing or displaying output do not affect it
 *
 * @param urbanGraph - reference to a graph object containing loaded data
 */
void independentRoutePlanning(Graph<int>& graph);

/**
 * @brief Finds the best path on a restricted driving route plan with the data on input.txt
 *
 * Parses the input file to load route input data and then calculates the best route with dijkstra(). Finally it saves the output on the output.txt file. <br>
 * The route is calculated by calling dijkstra() once to get the best path from the startNode to the endNode if there is no include node. <br>
 * If the input file contains a valid include node then the route is calculated by calling dijkstra() once to get the best path from the start node to the include node.
 * Then dijkstra() is called again to get the best path from the include node to the end node.
 * At the end both paths are merged to obtain the best route from the start node to the end node passing through the include node. <br>
 * Stops execution midway if:
 * - it cannot open the input or the output file
 * - the start or end nodes are invalid
 * Since dijkstra() is always called at most twice, the time complexity of this algorithm is O((E+V)logV) and the other parts such as input parsing or displaying output do not affect it
 *
 * @param urbanGraph - reference to a graph object containing loaded data
 */
void restrictedRoutePlanning(Graph<int>& graph);

/**
 * @brief Finds the best path on a driving and walking route plan with the data on input.txt
 *
 * Parses the input file to load route input data, checks the constraints on the nodes and then calculates the best route with dijkstra().
 * If there is no route obeying the defined input parameters then it calculates the two best approximate solutions. Finally it saves the output on the output.txt file. <br>
 * The route is calculated by calling dijkstra() once to get the best path from the startNode to every other node.
 * It then saves the path calculated on each vertex with Vertex::saveDist() and Vertex::savePath() and calls dijkstra() again to calculate the best path from the end node to all the other nodes.
 * Afterwards it iterates through every parking node in the graph and sums the distance to the start node and the distance to the end node, saving the results from the best one.
 * If no node obeys the max walk time restriction then it saves the two closest parking nodes in terms of max walk time and presents them as suggestions. <br>
 * Stops execution midway if:
 * - it cannot open the input or the output file
 * - the start or end nodes are invalid, adjacent to each other or a parking node
 * Since dijkstra() is always called twice, the time complexity of this algorithm is O((E+V)logV) and the other parts such as input parsing or displaying output do not affect it
 *
 * @param urbanGraph - reference to a graph object containing loaded data
 */
void environmentallyFriendlyRoutePlanning(Graph<int>& graph);

#endif // BATCHMODE_H
