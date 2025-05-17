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
    int id; ///< the pallet id
    int weight; ///< the weight occupied by the pallet
    int profit; ///< the profit the pallet adds when selected
};

/**
 * @struct ProblemInstance
 * @brief Contains data for a packing problem instance, including truck capacity and a list of pallets.
 */
struct ProblemInstance {
    int truckCapacity; ///< the maximum capacity the truck can carry
    std::vector<Pallet> pallets; ///< a vector containing all the existing pallets for this problem instance
};

/**
 * @brief Reads the truck capacity and pallet data from CSV files.
 *
 * @param truckFile Path to the CSV file containing the truck capacity (expects a single line of data).
 * @param palletFile Path to the CSV file containing the pallet list (expects lines with ID, weight, and profit).
 *
 * @return A populated ProblemInstance object.
 */
ProblemInstance readData(const std::string& truckFile, const std::string& palletFile);

#endif //PROJECT2_PALLETPACKING_H
