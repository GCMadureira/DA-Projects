//
// Created by GustavoCM on 16/04/2025.
//

#include "algorithms.h"

static ProblemInstance problemInstance;
static std::vector<bool> currentPacking;
static std::vector<bool> bestPacking;
static int bestValue, bestWeight, bestSize; // information about the current best packing

static void exploreLevel(int level, int currentValue, int currentWeight, int currentSize) {
    // reached the end of the tree
    if(level == problemInstance.pallets.size()) {
      if(currentValue > bestValue || (currentValue == bestValue && currentSize < bestSize)
        || (currentValue == bestValue && currentSize == bestSize && currentWeight < bestWeight)) {
        //choose the solution based on profit, number of pallets and weight, in this order of priority
        bestPacking = currentPacking;
        bestValue = currentValue;
        bestWeight = currentWeight;
        bestSize = currentSize;
      }
      return;
    }

    const Pallet currentPallet = problemInstance.pallets[level];

    currentPacking[level] = true; // choose to add the current pallet if it fits
    if(currentWeight + currentPallet.weight <= problemInstance.truckCapacity)
      exploreLevel(level + 1, currentValue + currentPallet.profit, currentWeight + currentPallet.weight, currentSize + 1);

    currentPacking[level] = false; --currentSize; // choose not to add the current pallet
    exploreLevel(level + 1, currentValue, currentWeight, currentSize);
}

// explores every combination of pallets through backtracking, in each node, can decide to add the current level's pallet or not
KnapsackResult bf_approach(const ProblemInstance& instance) {
    // set the static variables for this iteration of the algorithm
    problemInstance = instance;
    currentPacking = std::vector<bool>(instance.pallets.size(), false);
    bestPacking = std::vector<bool>(instance.pallets.size(), false);
    bestValue = 0; bestWeight = INT_MAX; bestSize = 0;

    exploreLevel(0, 0, 0, 0);

    std::vector<Pallet> result;
    int profit = 0, weight = 0;
    for(int i = 0; i < instance.pallets.size(); i++) {
      if(bestPacking[i]) {
        result.push_back(instance.pallets[i]);
        profit += instance.pallets[i].profit;
        weight += instance.pallets[i].weight;
      }
    }

    return {profit, weight, result};
}