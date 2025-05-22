#ifndef PROJECT2_ALGORITHMS_H
#define PROJECT2_ALGORITHMS_H

#include <vector>
#include <unordered_set>
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
    std::vector<Pallet> selectedPallets; ///< the ids of the select pallets
};

/**
 * @brief Solves the pallet packing problem using a brute-force backtracking approach.
 *
 * Explores all possible combinations of pallets to find the maximum profit without exceeding the truck's capacity.
 * Uses a backtracking approach without bounding, effectively exploring all the branches/combinations.
 * Has an O(2^n) time complexity since it always explores all possible combinations, and an O(n) space complexity since it only needs to store 2 vectors of size n and a constant number of variables.
 *
 * @param instance The problem instance containing the truck capacity and pallet list.
 * @return The result containing the maximum profit and selected pallets.
 */
KnapsackResult bf_approach(const ProblemInstance& instance);

/**
 * @brief Solves the pallet packing problem using a branch and bound approach.
 *
 * Explores a tree of possible combinations, and at each level chooses to add or not to add an item, exploring those two subtrees. Sorts the pallets by ascending order of weights first.
 * If the remaining profit of unselected items is not enough to reach the current best solution, it abandons the exploration of that subtree.
 * On the same manner, since the weights are sorted, if selecting the next weight exceeds the truck capacity, then abandon the exploration of that subtree.
 * Has an O(2^n) asymptotic time complexity since, in the worst case scenario it explores all possible combinations(generally better than that), and an O(n) space complexity since it only needs to store 2 vectors of size n and a constant number of variables.
 *
 * @param instance The problem instance containing the truck capacity and pallet list.
 * @return The result containing the maximum profit and selected pallets.
 */
KnapsackResult bb_approach(const ProblemInstance& instance);

/**
 * @brief Solves the pallet packing problem using a dynamic programming approach.
 *
 * Uses a bottom-up method to compute the optimal combination of pallets for maximum profit.
 * Depending on the truck capacity or dataset size, it dynamically chooses between using a
 * vector-based or unordered_map-based DP table for performance efficiency.
 * Using the vector based approach, it has an O(n*W) time complexity since it iterates through all n pallets, and for each processes (W - pallet.weight) weights, and an O(n*W) space complexity since it uses a vector of size W+1 containing other vectors with size of at most n.
 * Using the hash map based approach, it has an O(n*k) time complexity and an O(n*k) space complexity for the same reasons as the vector based approach, except k is the number of reachable unique weights, which is generally way smaller than C.
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
 * Has an O(n*log(n)) time complexity since it sorts all the pallets once, and an O(n) space complexity since it uses a vector to store the sorted pallets.
 *
 * @param instance The problem instance containing truck capacity and pallet list.
 * @return The result containing estimated profit and selected pallets (not guaranteed to be optimal).
 */
KnapsackResult g_approach(const ProblemInstance& instance);

/**
 * @brief Solves the pallet packing problem using a 2-approximation approach.
 *
 * Uses two greedy algorithms and then chooses the best option out of both results.
 * The first algorithm selects pallets based on increasing weight-to-profit ratio until the truck is full or no more pallets fit.
 * To do so it sorts all the pallets based on said ratio first and iterates through the sorted vector.
 * The second algorithm selects pallets based on profit until the truck is full or no more pallets fit.
 * To do so it sorts all the pallets in decreasing order of profit and then iterates through the sorted vector.
 * Has an O(n*log(n)) time complexity since it sorts all the pallets twice, once on each algorithm, and an O(n) space complexity since it uses two vectors to store the sorted pallets, also one in each algorithm.
 *
 * @param instance The problem instance containing truck capacity and pallet list.
 * @return The result containing estimated profit and selected pallets (not guaranteed to be optimal).
 */
KnapsackResult app_approach(const ProblemInstance& instance);

/**
 * @brief Solves the pallet packing problem using an integer linear programming approach.
 *
 * Each pallet is converted to a binary variable, that is, it only has values <=0 or >=1.
 * The objective function is the sum of the variables multiplied by the profit of the pallet that it originated from.
 * This way, if a variable is set to 1, it means the pallet is selected and is contributing to the objective function.
 * The only starting constraint is one where the sum of all the variables, each times the weight of the pallet of origin, is less or equal to the truck capacity.
 * Has an O(2^n) time complexity in the worst case, since it needs to explore an exponential number of nodes (it is an NP problem), but in the average case is much more efficient.
 * Has an O(n) space complexity where n is the number of variables (pallets) and m is the number of constraints needed.
 *
 * @param instance The problem instance containing truck capacity and pallet list.
 * @return The result containing estimated profit and selected pallets (not guaranteed to be optimal).
 */
KnapsackResult ilp_approach(const ProblemInstance& instance);


#endif //PROJECT2_ALGORITHMS_H
