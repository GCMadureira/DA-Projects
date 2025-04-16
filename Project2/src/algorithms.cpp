//
// Created by GustavoCM on 16/04/2025.
//

#include "algorithms.h"

void exploreLevel(const ProblemInstance& instance, int level, std::vector<bool>& currentPacking, int currentValue, int currentWeight) {
    static std::vector<bool> bestPacking(instance.pallets.size(), false);
    static int bestValue = 0;

    // reached the end of the tree
    if(level == instance.pallets.size()) {
      if(currentValue > bestValue) {
        bestPacking = currentPacking;
        bestValue = currentValue;
      }
      return;
    }

    Pallet currentPallet = instance.pallets[level];

    currentPacking[level] = true; // choose to add the current pallet if it fits
    if(currentWeight + currentPallet.weight <= instance.truckCapacity)
      exploreLevel(instance, level + 1, currentPacking, currentValue + currentPallet.profit, currentWeight + currentPallet.weight);

    currentPacking[level] = false; // choose not to add the current pallet
    exploreLevel(instance, level + 1, currentPacking, currentValue, currentWeight);

    if(level == 0) { // pass the answer on the currentPacking parameter
      currentPacking = bestPacking;
    }
}

// explores every combination of pallets through backtracking, in each node, can decide to add the current level's pallet or not
std::vector<Pallet> bruteForce(const ProblemInstance& instance) {
    std::vector<Pallet> result;
    std::vector<bool> currentPacking(instance.pallets.size(), false);

    exploreLevel(instance, 0, currentPacking, 0, 0);

    for(int i = 0; i < instance.pallets.size(); i++) {
      if(currentPacking[i]) {
        result.push_back(instance.pallets[i]);
      }
    }

    return result;
}