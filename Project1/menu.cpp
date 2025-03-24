//
// Created by Gabriel Sousa on 24/02/2025.
//


#include <iostream>
#include "data_structures/Graph.h"
#include "data_structures/MutablePriorityQueue.h"
#include "parsing.cpp"
#include "search.cpp"
#include "parsing.h"
#include "batchmode.cpp"


int main() {
    // Build the graph from CSV files (located in the DataSets folder)
    Graph<int> urbanGraph;
    loadGraph(urbanGraph);

    int choice;
    do {
        std::cout << "\n===== Route Planning =====\n";
        std::cout << "1. Independent Route Planning\n";
        std::cout << "2. Restricted Route Planning\n";
        std::cout << "3. Environmental Friendly Route Planning\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1:
                independentRoutePlanning(urbanGraph);
                break;
            case 2:
                restrictedRoutePlanning(urbanGraph);
                break;
            case 3:
                environmentallyFriendlyRoutePlanning(urbanGraph);
                break;
            case 4:
                std::cout << "Exiting program.\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while(choice!=4);
        /*
        if (choice == 1) {
            // TODO: This function reads input.txt and writes the route results to output.txt.
            independentRoutePlanning(urbanGraph);
        } else if (choice == 2) {
            std::cout << "Exiting program.\n";
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 2);
*/
    return 0;
}

