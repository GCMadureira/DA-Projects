//
// Created by Gabriel Sousa on 17/04/2025.
//

#include "algorithms.h"


KnapsackResult g_approach(const ProblemInstance& instance) {
    std::vector<Pallet> sortedPallets = instance.pallets;

    std::sort(sortedPallets.begin(), sortedPallets.end(), [](const Pallet &a, const Pallet &b) {
        double ra = static_cast<double>(a.weight) / a.profit;
        double rb = static_cast<double>(b.weight) / b.profit;
        return ra < rb;
    });

    KnapsackResult result;
    result.maxProfit = 0;
    int currentWeight = 0;

    for (const auto& pallet : sortedPallets) {
        if (currentWeight + pallet.weight <= instance.truckCapacity) {
            currentWeight += pallet.weight;
            result.maxProfit += pallet.profit;
            result.selectedPallets.push_back(pallet);
        }
    }

    result.occupiedWeight = currentWeight;

    return result;
}