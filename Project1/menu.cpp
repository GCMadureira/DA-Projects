//
// Created by Gabriel Sousa on 24/02/2025.
//


#include <iostream>
#include "Graph.h"
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

    int startNode, endNode;
    if (!(inFile >> startNode >> endNode)) {
        outFile << "Invalid or missing node data.\n";
        return;
    }

    //  TODO: Call algorithm.
    // For demonstration,just output the information.
    outFile << "Computing route from " << startNode << " to " << endNode << ".\n";

    // TODO: Replace the following with your actual route planning function.
    // Example: auto route = computeRoute(urbanGraph, startNode, endNode);
    // outFile << "Route: " << route << "\n";

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

