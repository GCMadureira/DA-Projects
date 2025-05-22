//
// Created by up202304978 on 19-05-2025.
//

#include "algorithms.h"


KnapsackResult app_approach(const ProblemInstance& instance) {
    KnapsackResult result1 = g_approach(instance);
    KnapsackResult result2;

    std::vector<Pallet> sortedPallets = instance.pallets;
    std::sort(sortedPallets.begin(), sortedPallets.end(), [](const Pallet &a, const Pallet &b) {return a.profit > b.profit;});

    result2.maxProfit = 0;
    int currentWeight = 0;

    for (const auto& pallet : sortedPallets) {
        if (currentWeight + pallet.weight <= instance.truckCapacity) {
            currentWeight += pallet.weight;
            result2.maxProfit += pallet.profit;
            result2.selectedPallets.push_back(pallet);
        }
    }

    result2.occupiedWeight = currentWeight;

    return (result1.maxProfit > result2.maxProfit ||
        (result1.maxProfit == result2.maxProfit && result1.selectedPallets.size() < result2.selectedPallets.size()) ||
        (result1.maxProfit == result2.maxProfit && result1.selectedPallets.size() == result2.selectedPallets.size() && result1.occupiedWeight < result2.occupiedWeight)
            ? result1 : result2);
}