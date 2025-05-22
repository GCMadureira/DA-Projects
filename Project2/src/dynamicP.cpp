//
// Created by Gabriel Sousa on 17/04/2025.
//

#include "algorithms.h"

KnapsackResult dp_array_approach(const ProblemInstance& instance) {
    int capacity = instance.truckCapacity;
    using State = std::pair<int, std::vector<int>>;

    std::vector<State> dp(capacity + 1, {-1, {}});
    dp[0] = {0, {}};

    for (const auto& pallet : instance.pallets) {
        for (int w = capacity - pallet.weight; w >= 0; --w) {
            if (dp[w].first != -1) {
                int newW = w + pallet.weight;
                int newProfit = dp[w].first + pallet.profit;

                if (dp[newW].first < newProfit) {
                    dp[newW].first = newProfit;
                    dp[newW].second = dp[w].second;
                    dp[newW].second.push_back(pallet.id);
                }
            }
        }
    }

    int maxProfit = 0;
    std::vector<int> bestSelection;
    for (int w = 0; w <= capacity; ++w) {
        if (dp[w].first > maxProfit) {
            maxProfit = dp[w].first;
            bestSelection = dp[w].second;
        }
    }

    std::unordered_set<int> selectedPallets(bestSelection.begin(), bestSelection.end());
    std::vector<Pallet> result;
    int weight = 0;
    for (const Pallet& pallet : instance.pallets) {
        if (selectedPallets.contains(pallet.id)) {
            result.push_back(pallet);
            weight += pallet.weight;
        }
    }

    return KnapsackResult{maxProfit, weight, result};
}

KnapsackResult dp_hashmap_approach(const ProblemInstance& instance) {
    int capacity = instance.truckCapacity;
    using State = std::pair<int, std::vector<int>>;

    std::unordered_map<int, State> dp;
    dp[0] = {0, {}};

    for (const auto& pallet : instance.pallets) {
        std::unordered_map<int, State> next = dp;

        for (const auto& [weight, state] : dp) {
            int newWeight = weight + pallet.weight;
            if (newWeight > capacity) continue;

            int newProfit = state.first + pallet.profit;

            if (next.find(newWeight) == next.end() || next[newWeight].first < newProfit) {
                std::vector<int> newSelection = state.second;
                newSelection.push_back(pallet.id);
                next[newWeight] = {newProfit, std::move(newSelection)};
            }
        }

        dp = std::move(next);
    }

    int maxProfit = 0;
    std::vector<int> bestSelection;
    for (const auto& [weight, state] : dp) {
        if (state.first > maxProfit) {
            maxProfit = state.first;
            bestSelection = state.second;
        }
    }

    std::unordered_set<int> selectedPallets(bestSelection.begin(), bestSelection.end());
    std::vector<Pallet> result;
    int weight = 0;
    for (const Pallet& pallet : instance.pallets) {
        if (selectedPallets.contains(pallet.id)) {
            result.push_back(pallet);
            weight += pallet.weight;
        }
    }

    return KnapsackResult{maxProfit, weight, result};
}

KnapsackResult dp_approach(const ProblemInstance& instance) {
    if (instance.truckCapacity <= 10000) {
        return dp_array_approach(instance);
    } else {
        return dp_hashmap_approach(instance);
    }
}

