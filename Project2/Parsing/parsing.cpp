//
// Created by Gabriel Sousa on 15/04/2025.
//

#include "PalletPacking.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool parseInt(const std::string& str, int& value) {
    std::stringstream ss(str);
    ss >> value;
    return !ss.fail() && ss.eof(); // eof ensures full conversion
}

ProblemInstance readData(const std::string& truckFile, const std::string& palletFile) {
    ProblemInstance instance;
    std::ifstream truck(truckFile);
    std::ifstream pallets(palletFile);

    std::string line;

    // Read truck capacity
    getline(truck, line); // Skip header
    if (!getline(truck, line)) {
        std::cerr << "Failed to read truck data line.\n";
        return instance;
    }

    // Remove carriage return if present 
    if (!line.empty() && line.back() == '\r')
        line.pop_back();

    std::stringstream ss(line);
    std::string token;

    // Capacity
    getline(ss, token, ',');
    token.erase(0, token.find_first_not_of(" \t")); // trim left
    token.erase(token.find_last_not_of(" \t") + 1); // trim right
    if (!parseInt(token, instance.truckCapacity)) {
        std::cerr << "Failed to parse truck Capacity: " << token << "\n";
    }

    // Read Pallets
    getline(pallets, line); // Skip header
    while (getline(pallets, line)) {
        if (line.empty()) continue;

        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        std::stringstream ps(line);
        std::string token;
        Pallet p;

        // Pallet ID
        getline(ps, token, ',');
        if (!parseInt(token, p.id)) {
            std::cerr << "Failed to parse ID: " << token << "\n";
            continue;
        }

        // Weight
        getline(ps, token, ',');
        if (!parseInt(token, p.weight)) {
            std::cerr << "Failed to parse weight: " << token << "\n";
            continue;
        }

        // Profit
        getline(ps, token, ',');
        if (!parseInt(token, p.profit)) {
            std::cerr << "Failed to parse profit: " << token << "\n";
            continue;
        }

        instance.pallets.push_back(p);
    }

    return instance;
}