//
// Created by Gabriel Sousa on 15/04/2025.
//

#include "algorithms.h"
#include "test_runner.cpp"
#include <iostream>
#include <string>

void showMainMenu() {
    std::cout << "\n===== Pallet Packing Optimization Tool =====\n";
    std::cout << "1. Run Brute-Force\n";
    std::cout << "2. Run Branch and Bound\n";
    std::cout << "3. Run Dynamic Programming\n";
    std::cout << "4. Run Greedy Approximation\n";
    std::cout << "5. Run ILP / Advanced Algorithm (Not implemented)\n";
    std::cout << "0. Back\n";
    std::cout << "Choose an option: ";
}

std::string selectDatasetPath() {
    int sourceChoice, datasetNumber;

    std::cout << "\nSelect dataset source:\n";
    std::cout << "1. Provided datasets\n";
    std::cout << "2. Your own datasets\n";
    std::cout << "Choice: ";
    std::cin >> sourceChoice;

    std::string basePath;
    if (sourceChoice == 1)
        basePath = "../data/Provided/";
    else if (sourceChoice == 2)
        basePath = "../data/Own/";
    else {
        std::cerr << "Invalid source choice. Defaulting to Provided.\n";
        basePath = "../data/Provided/";
    }

    std::cout << "Choose dataset number (1 to 10): ";
    std::cin >> datasetNumber;
    if (datasetNumber < 1 || datasetNumber > 10) {
        std::cerr << "Invalid dataset number. Defaulting to 1.\n";
        datasetNumber = 1;
    }

    std::string datasetId = (datasetNumber < 10 ? "0" + std::to_string(datasetNumber) : std::to_string(datasetNumber));
    std::string truckFile = basePath + "TruckAndPallets_" + datasetId + ".csv";
    std::string palletFile = basePath + "Pallets_" + datasetId + ".csv";

    return truckFile + ";" + palletFile;
}

void runInteractiveMode() {
    std::string datasetPath = selectDatasetPath();
    size_t sep = datasetPath.find(';');
    std::string truckFile = datasetPath.substr(0, sep);
    std::string palletFile = datasetPath.substr(sep + 1);

    ProblemInstance instance = readData(truckFile, palletFile);

    std::cout << "Dataset loaded. Pallets: " << instance.pallets.size()
              << ", Capacity: " << instance.truckCapacity << "\n\n";

    int choice;
    do {
        showMainMenu();
        std::cin >> choice;

        KnapsackResult result;

        switch (choice) {
            case 1:
                result = bf_approach(instance);
                break;
            case 2:
                result = bb_approach(instance);
            break;
            case 3:
                result = dp_approach(instance);
                break;
            case 4: {
                result = g_approach(instance);
                // Compare greedy with optimal (DP)
                int dpResult = dp_approach(instance).maxProfit;
                if (result.maxProfit == dpResult)
                    std::cout << "\nGreedy solution is OPTIMAL.\n";
                else
                    std::cout << "\nGreedy solution is NOT optimal -> " << result.maxProfit << "/" << dpResult << ", " << 100*result.maxProfit/(double)dpResult << "% of the optimal solution" << "\n";
                break;
            }
            case 5:
                std::cout << "ILP / Advanced algorithm is not implemented.\n";
                continue;
            case 0:
                return;
            default:
                std::cout << "Invalid choice.\n";
                continue;
        }

        std::sort(result.selectedPalletIds.begin(), result.selectedPalletIds.end());

        std::cout << "\nMaximum profit: " << result.maxProfit << "\n";
        std::cout << "Weight occupied: " << result.occupiedWeight << "/" << instance.truckCapacity << "\n";
        std::cout << "Selected pallet IDs:\n";
        for (int id : result.selectedPalletIds) {
            std::cout << id << " ";
        }
        std::cout << "\n\n";

    } while (choice != 0);
}

int main() {
    int modeChoice;
    do {
        std::cout << "\n=== Select Mode ===\n";
        std::cout << "1. Normal Use (Interactive)\n";
        std::cout << "2. Run Automated Benchmark Tests\n";
        std::cout << "0. Exit\n";
        std::cout << "Choice: ";
        std::cin >> modeChoice;

        KnapsackResult result;

        switch (modeChoice) {
            case 1:
                runInteractiveMode();
            break;
            case 2:
                runTestMode();
            break;
            case 0:
                std::cout << "Exiting...\n";
            return 0;
            default:
                std::cout << "Invalid choice.\n";
        }
    } while (modeChoice != 0);

    return 0;
}
