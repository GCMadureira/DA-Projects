//
// Created by Gabriel Sousa on 15/04/2025.
//

#ifndef PROJECT2_PALLETPACKING_H
#define PROJECT2_PALLETPACKING_H

#include <vector>
#include <string>

/**
 * @struct Pallet
 * @brief Represents a pallet with an ID, weight, and profit.
 */
struct Pallet {
    int id;
    int weight;
    int profit;
};

/**
 * @struct ProblemInstance
 * @brief Contains data for a packing problem instance, including truck capacity and a list of pallets.
 */
struct ProblemInstance {
    int truckCapacity;
    std::vector<Pallet> pallets;
};

/**
 * @brief Reads truck capacity and pallet data from CSV files.
 *
 * @param truckFile Path to the CSV file containing the truck capacity (expects a single line of data).
 * @param palletFile Path to the CSV file containing the pallet list (expects lines with ID, weight, and profit).
 * @return A populated ProblemInstance object.
 */
ProblemInstance readData(const std::string& truckFile, const std::string& palletFile);

#endif //PROJECT2_PALLETPACKING_H
