#ifndef PROJECT2_ALGORITHMS_H
#define PROJECT2_ALGORITHMS_H

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include "PalletPacking.h"
#include "functional"

/**
 * @struct KnapsackResult
 * @brief Represents the result of a knapsack algorithm, including total profit and selected pallet IDs.
 */
struct KnapsackResult {
    int maxProfit;
    int occupiedWeight;
    std::vector<int> selectedPalletIds;
};

/**
 * @brief Solves the pallet packing problem using a brute-force backtracking.
 *
 * Explores all possible combinations of pallets to find the maximum profit without exceeding the truck's capacity.
 *
 * @param instance The problem instance containing truck capacity and pallet list.
 * @return The result containing maximum profit and selected pallets.
 */
KnapsackResult bruteForce(const ProblemInstance& instance);

/**
 * @brief Solves the pallet packing problem using a dynamic programming approach.
 *
 * Uses a bottom-up method to compute the optimal combination of pallets for maximum profit.
 * Depending on the truck capacity or dataset size, it dynamically chooses between using a
 * vector-based or unordered_map-based DP table for performance efficiency.
 *
 * @param instance The problem instance containing truck capacity and pallet list.
 * @return The result containing maximum profit and selected pallet IDs.
 */
KnapsackResult dp_approach(const ProblemInstance& instance);

/**
 * @brief Solves the pallet packing problem using a greedy approach.
 *
 * Selects pallets based on increasing weight-to-profit ratio until the truck is full or no more pallets fit.
 *
 * @param instance The problem instance containing truck capacity and pallet list.
 * @return The result containing estimated profit and selected pallets (not guaranteed to be optimal).
 */
KnapsackResult g_approach(const ProblemInstance& instance);

#endif //PROJECT2_ALGORITHMS_H
