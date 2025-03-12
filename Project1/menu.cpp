//
// Created by Gabriel Sousa on 24/02/2025.
//


#include <iostream>
#include "data_structures/Graph.h"
#include "data_structures/MutablePriorityQueue.h"
#include "parsing.cpp"
#include "search.cpp"
#include "parsing.h"

// Batch mode function that reads from input.txt and writes to output.txt.
void runBatchMode(Graph<int>& urbanGraph) {
    std::ifstream inFile("inputs/input.txt");
    if (!inFile.is_open()) {
        std::cerr << "Could not open input.txt. Batch mode aborted.\n";
        return;
    }

    std::ofstream outFile("inputs/output.txt");
    if (!outFile.is_open()) {
        std::cerr << "Could not open output.txt. Batch mode aborted.\n";
        return;
    }

    // Example: Expecting the command "ROUTE" followed by two integers.
    std::string command;
    inFile >> command;
    if (command != "ROUTE") {
        outFile << "Invalid input format. Expected command 'ROUTE'.\n";
        return;
    }
    /*
        std::string line;
        std::getline(inFile, line);
        std::string value;
        while (std::getline(inFile, line)){
            std::getline(linestream, token, ',');
        }

     */

    int startNode, endNode;
    if (!(inFile >> startNode >> endNode)) {
        outFile << "Invalid or missing node data.\n";
        return;
    }

    //  TODO: Call algorithm.
    // For demonstration,just output the information.
    outFile << "Computing route from " << startNode << " to " << endNode << ".\n";

    // Allow every vertex to be taken
    for (auto v : urbanGraph.getVertexSet()) {
        v->setIgnoreFlag(false);
    }

    // Compute Best Driving Route
    int pathLength;
    dijkstra(&urbanGraph, startNode);
    std::vector<int> path = getPath(&urbanGraph, startNode, endNode, pathLength);
    if (path.empty()) {
        outFile << "BestDrivingRoute: none\n"
                << "AlternateDrivingRoute: none\n";
        return;
    }

    outFile << "\n";
    outFile << "Source: " << startNode << "\n"
            << "Destination: " << endNode << "\n"
            << "BestDrivingRoute: " << path[0];
    for (int i = 1; i < path.size(); i++) {
        outFile << ", " << path[i];
    }
    outFile << " (" << pathLength << ")" << "\n";

    // Ignore all used Nodes to compute the Alternate Driving Route (except start and end nodes)
    for (int i = 1; i < path.size() - 1; ++i) {
        auto vertex = urbanGraph.findVertex(path[i]);
        vertex->setIgnoreFlag(true);
    }


    // Second iteration of the algorithm to compute the Alternative Driving Route
    dijkstra(&urbanGraph, startNode);
    path = getPath(&urbanGraph, startNode, endNode, pathLength);
    if (path.empty()) {
        outFile << "AlternateDrivingRoute: none\n";
        return;
    }

    outFile << "AlternateDrivingRoute: " << path[0];
    for (int i = 1; i < path.size(); i++) {
        outFile << ", " << path[i];
    }
    outFile << " (" << pathLength << ")" << "\n\n";


    outFile << "Batch processing completed successfully.\n";
}

int main() {
    // Build the graph from CSV files (located in the DataSets folder)
    Graph<int> urbanGraph;
    loadGraph(urbanGraph);

    int choice;
    do {
        std::cout << "\n===== Route Planning =====\n";
        std::cout << "1. Independent Route Planning\n";
        std::cout << "2. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore();

        //TODO: runbatchmode(urbanGraph,choice)
        // Depois no batchmode faz-se o switch
        //
        if (choice == 1) {
            // TODO: This function reads input.txt and writes the route results to output.txt.
            std::cout << "Not yet implemented.\n";
            runBatchMode(urbanGraph);
            // TODO: Call runBatchMode() or other functions as required.
        } else if (choice == 2) {
            std::cout << "Exiting program.\n";
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 2);

    return 0;
}

