//
// Created by GustavoCM on 18/05/2025.
//


#include "algorithms.h"

static std::vector<Pallet> sortedPallets;
static std::vector<bool> currentPacking;
static std::vector<bool> bestPacking;
static int bestValue, maxWeight;

static void exploreLevel(int level, int currentValue, int currentWeight, int remainingValue) {
    // reached the end of the tree OR the current pallet does not fit, so the following also do not
    if(level == sortedPallets.size() || currentWeight + sortedPallets[level].weight > maxWeight) {
        if(currentValue > bestValue) {
            bestPacking = currentPacking;
            bestValue = currentValue;
        }
        return;
    }

    const Pallet currentPallet = sortedPallets[level];

    currentPacking[level] = true; // choose to add the current pallet if it fits
    exploreLevel(level + 1, currentValue + currentPallet.profit, currentWeight + currentPallet.weight, remainingValue - currentPallet.profit);

    currentPacking[level] = false; // choose not to add the current pallet
    // if you choose not to add the pallet and it cannot lead to an optimal solution
    if (currentValue + remainingValue - currentPallet.profit <= bestValue) return;
    exploreLevel(level + 1, currentValue, currentWeight, remainingValue - currentPallet.profit);
}

// explores the combinations of pallets through backtracking, in each node, can decide to add the current level's pallet or not, uses bounding/branch pruning
KnapsackResult bb_approach(const ProblemInstance& instance) {
    // set the static variables for this iteration of the algorithm
    sortedPallets = instance.pallets;
    currentPacking = std::vector<bool>(instance.pallets.size(), false);
    bestPacking = std::vector<bool>(instance.pallets.size(), false);
    bestValue = 0; maxWeight = instance.truckCapacity;

    int remainingValue = 0;
    for (Pallet pallet : instance.pallets) remainingValue += pallet.profit;

    // sort the pallets according to their weights (asc)
    std::sort(sortedPallets.begin(), sortedPallets.end(), [](const Pallet& left, const Pallet& right) {return left.weight < right.weight;});

    exploreLevel(0, 0, 0, remainingValue);

    std::vector<Pallet> result;
    int profit = 0, weight = 0;
    for(int i = 0; i < sortedPallets.size(); i++) {
        if(bestPacking[i]) {
            result.push_back(sortedPallets[i]);
            profit += sortedPallets[i].profit;
            weight += sortedPallets[i].weight;
        }
    }

    return {profit, weight, result};
}