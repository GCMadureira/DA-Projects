//
// Created by Gabriel Sousa on 17/04/2025.
//

#include "PalletPacking.h"
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iostream>

struct KnapsackResult {
    int maxProfit;
    std::vector<int> selectedPalletIds;
};


KnapsackResult dp_approach(const ProblemInstance& instance) {
    // map: weight -> { profit, list of pallet IDs used }
    std::unordered_map<int, std::pair<int, std::vector<int>>> dp;
    dp[0] = {0, {}};

    for (const auto& pallet : instance.pallets) {
        std::unordered_map<int, std::pair<int, std::vector<int>>> next = dp;

        for (const auto& [weight, value] : dp) {
            int newWeight = weight + pallet.weight;
            if (newWeight > instance.truckCapacity) continue;

            int newProfit = value.first + pallet.profit;

            // If not present or better profit, update
            if (next.find(newWeight) == next.end() || next[newWeight].first < newProfit) {
                std::vector<int> newSelection = value.second;
                newSelection.push_back(pallet.id);
                next[newWeight] = {newProfit, std::move(newSelection)};
            }
        }

        dp = std::move(next);
    }

    // Find best solution
    int maxProfit = 0;
    std::vector<int> bestSelection;
    for (const auto& [weight, value] : dp) {
        if (value.first > maxProfit) {
            maxProfit = value.first;
            bestSelection = value.second;
        }
    }

    return {maxProfit, bestSelection};
}
