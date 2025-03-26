#ifndef BATCHMODE_H
#define BATCHMODE_H

#include "data_structures/Graph.h"

void parseInputFile(std::ifstream& inputFile, std::ofstream& outputFile, Graph<int>& graph, int& startNode, int& endNode, int& maxWalkTime);
void independentRoutePlanning(Graph<int>& graph);
void restrictedRoutePlanning(Graph<int>& graph);
void environmentallyFriendlyRoutePlanning(Graph<int>& graph);

#endif // BATCHMODE_H
