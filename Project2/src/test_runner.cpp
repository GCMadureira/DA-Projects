//
// Created by Gabriel Sousa on 14/05/2025.
//

#include "algorithms.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <functional>

const int NUM_RUNS = 15;
const int BRUTE_FORCE_LIMIT = 25;

// Generic average timing wrapper
double averageTime(std::function<void()> func) {
    double totalTime = 0.0;
    for (int i = 0; i < NUM_RUNS; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        totalTime += std::chrono::duration<double>(end - start).count();
    }
    return totalTime / NUM_RUNS;
}

void runTestMode() {
    std::cout << "===== Automated Benchmark Mode =====\n";

    std::string basePath = "../data/Own/";
    std::ofstream output("../results/benchmark_results.csv");

    if (!output.is_open()) {
        std::cerr << "Error: Could not open results output file.\n";
        return;
    }

    output << "Dataset,BruteForce(s),BranchAndBound(s),DynamicProgramming(s),Greedy(s),GreedyOptimal\n";

    for (int i = 1; i <= 10; ++i) {
        std::string datasetId = (i < 10 ? "0" + std::to_string(i) : std::to_string(i));
        std::string truckFile = basePath + "TruckAndPallets_" + datasetId + ".csv";
        std::string palletFile = basePath + "Pallets_" + datasetId + ".csv";

        ProblemInstance instance = readData(truckFile, palletFile);
        std::cout << "\nRunning tests for dataset " << datasetId << "...\n";

        // --- Brute-Force ---
        double timeBrute = -1.0;
        KnapsackResult bruteRes;
        if (instance.pallets.size() <= BRUTE_FORCE_LIMIT) {
            timeBrute = averageTime([&]() {
                bruteRes = bf_approach(instance);
            });
            std::cout << "  Brute-Force Avg Time: " << timeBrute << "s\n";
        } else {
            std::cout << "  Brute-Force skipped (too many pallets: " << instance.pallets.size() << ")\n";
        }

        // --- Brute-Force ---
        double timeBranch = -1.0;
        KnapsackResult branchRes;
        if (instance.pallets.size() <= BRUTE_FORCE_LIMIT) {
            timeBranch = averageTime([&]() {
                branchRes = bb_approach(instance);
            });
            std::cout << "  Branch and Bounding Avg Time: " << timeBranch << "s\n";
        } else {
            std::cout << "  Branch and Bounding skipped (too many pallets: " << instance.pallets.size() << ")\n";
        }

        // --- Dynamic Programming ---
        KnapsackResult dpRes;
        double timeDP = averageTime([&]() {
            dpRes = dp_approach(instance);
        });
        std::cout << "  DP Avg Time: " << timeDP << "s\n";

        // --- Greedy ---
        KnapsackResult greedyRes;
        double timeGreedy = averageTime([&]() {
            greedyRes = g_approach(instance);
        });
        std::cout << "  Greedy Avg Time: " << timeGreedy << "s";

        bool isOptimal = (greedyRes.maxProfit == dpRes.maxProfit);
        std::cout << " -> " << (isOptimal ? "OPTIMAL" : "NOT OPTIMAL") << "\n";

        // Write results
        output << datasetId << ",";
        if (timeBrute < 0)
            output << "SKIPPED,";
        else
            output << timeBrute << "," << timeBranch << ",";
        output << timeDP << "," << timeGreedy << "," << (isOptimal ? "Yes" : "No") << "\n";
    }

    output.close();
    std::cout << "\nBenchmark completed. Results saved to ../results/benchmark_results.csv\n";
}
