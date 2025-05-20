#ifndef PROJECT2_ALGORITHMS_H
#define PROJECT2_ALGORITHMS_H

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include "PalletPacking.h"
#include "functional"

/**
 * @struct KnapsackResult
 * @brief Represents the result of a knapsack algorithm, including total profit and selected pallet IDs.
 */
struct KnapsackResult {
    int maxProfit; ///< the maximum profit obtained in the solution
    int occupiedWeight; ///< the weight occupied by the selected items in the solution
    std::vector<int> selectedPalletIds; ///< the ids of the select pallets
};

/**
 * @brief Solves the pallet packing problem using a brute-force backtracking approach.
 *
 * Explores all possible combinations of pallets to find the maximum profit without exceeding the truck's capacity.
 * Uses a backtracking approach without bounding, effectively exploring all the branches/combinations.
 *
 * @param instance The problem instance containing the truck capacity and pallet list.
 * @return The result containing the maximum profit and selected pallets.
 */
KnapsackResult bf_approach(const ProblemInstance& instance);

KnapsackResult bb_approach(const ProblemInstance& instance);

/**
 * @brief Solves the pallet packing problem using a dynamic programming approach.
 *
 * Uses a bottom-up method to compute the optimal combination of pallets for maximum profit.
 * Depending on the truck capacity or dataset size, it dynamically chooses between using a
 * vector-based or unordered_map-based DP table for performance efficiency.
 *
 * @param instance The problem instance containing the truck capacity and pallet list.
 * @return The result containing the maximum profit and selected pallet IDs.
 */
KnapsackResult dp_approach(const ProblemInstance& instance);
KnapsackResult dp_array_approach(const ProblemInstance& instance);
KnapsackResult dp_hashmap_approach(const ProblemInstance& instance);

/**
 * @brief Solves the pallet packing problem using a greedy approach.
 *
 * Selects pallets based on increasing weight-to-profit ratio until the truck is full or no more pallets fit.
 * To do so it sorts all the pallets based on said ratio first and iterates through the sorted vector.
 *
 * @param instance The problem instance containing truck capacity and pallet list.
 * @return The result containing estimated profit and selected pallets (not guaranteed to be optimal).
 */
KnapsackResult g_approach(const ProblemInstance& instance);


KnapsackResult app_approach(const ProblemInstance& instance);

KnapsackResult ilp_approach(const ProblemInstance& instance);


#endif //PROJECT2_ALGORITHMS_H
