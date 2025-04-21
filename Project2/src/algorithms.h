//
// Created by GustavoCM on 21/04/2025.
//

#ifndef PROJECT2_ALGORITHMS_H
#define PROJECT2_ALGORITHMS_H

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include "PalletPacking.h"

struct KnapsackResult {
    int maxProfit;
    std::vector<int> selectedPalletIds;
};


KnapsackResult dp_approach(const ProblemInstance& instance);
KnapsackResult g_approach(const ProblemInstance& instance);

#endif //PROJECT2_ALGORITHMS_H
