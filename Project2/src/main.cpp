//
// Created by Gabriel Sousa on 15/04/2025.
//

#include "../Parsing/PalletPacking.h"
#include "algorithms.h"
#include <iostream>
#include <vector>

void showMenu() {
    std::cout << "===== Pallet Packing Optimization Tool =====\n";
    std::cout << "1. Run Brute-Force\n";
    std::cout << "2. Run Dynamic Programming\n";
    std::cout << "3. Run Greedy Approximation\n";
    std::cout << "4. Run ILP / Advanced Algorithm\n";
    std::cout << "0. Exit\n";
    std::cout << "Choose an option: ";
}

int main() {
    ProblemInstance instance;
    int choice;
    instance = readData("../data/TruckAndPallets_07.csv", "../data/Pallets_07.csv");
    std::cout << "Dataset loaded. Pallets: " << instance.pallets.size() << ", Capacity: " << instance.truckCapacity << "\n";

    do {
        showMenu();
        std::cin >> choice;
        switch (choice) {
            case 1: {
                std::vector<Pallet> result = bruteForce(instance);

                // for testing purposes
                std::cout << "\n";
                int totalProfit = 0, totalWeight = 0;
                for (Pallet p : result) {
                    std::cout << "Pallet " << p.id << ": profit->" << p.profit << " weight->" << p.weight << "\n";
                    totalProfit += p.profit;
                    totalWeight += p.weight;
                }
                std::cout << "Total profit: " << totalProfit << "\n";
                std::cout << "Weight filled: " << totalWeight << "/" << instance.truckCapacity << "\n\n";

                break;
            }
            case 2:
                // Call dynamic programming function
                break;
            case 3:
                // Call greedy approximation function
                break;
            case 4:
                // Call ILP or advanced algorithm
                break;
            case 0:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
