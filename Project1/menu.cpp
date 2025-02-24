//
// Created by Gabriel Sousa on 24/02/2025.
//


#include <iostream>
#include "Graph.h"
#include "parsing.cpp"
#include "search.cpp"
#include "parsing.h"

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
            // This function reads input.txt and writes the route results to output.txt.
            std::cout << "Not yet implemented.\n";
            // TODO
        } else if (choice == 2) {
            std::cout << "Exiting program.\n";
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 2);

    return 0;
}

