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

KnapsackResult bruteForce(const ProblemInstance& instance);
KnapsackResult dp_approach(const ProblemInstance& instance);
KnapsackResult g_approach(const ProblemInstance& instance);

#endif //PROJECT2_ALGORITHMS_H
