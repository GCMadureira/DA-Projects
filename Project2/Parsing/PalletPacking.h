//
// Created by Gabriel Sousa on 15/04/2025.
//

#ifndef PROJECT2_PALLETPACKING_H
#define PROJECT2_PALLETPACKING_H

#include <vector>
#include <string>

struct Pallet {
    int id;
    int weight;
    int profit;
};

struct ProblemInstance {
    int truckCapacity;
    std::vector<Pallet> pallets;
};

ProblemInstance readData(const std::string& truckFile, const std::string& palletFile);

#endif //PROJECT2_PALLETPACKING_H
