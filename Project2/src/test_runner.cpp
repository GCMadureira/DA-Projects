#include "algorithms.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <functional>
#include <filesystem>

const int NUM_RUNS = 5;
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

void runBenchmark(const std::string& datasetType, int datasetCount) {
    std::cout << "\n===== Benchmarking " << datasetType << " Datasets =====\n";

    std::string basePath = "../data/" + datasetType + "/";
    std::string outputPath = "../results/results_" + datasetType + "_Datasets.csv";

    std::ofstream output(outputPath);
    if (!output.is_open()) {
        std::cerr << "Error: Could not open results output file for " << datasetType << "\n";
        return;
    }

    output << "Dataset,BruteForce(s),BranchAndBound(s),DynamicProgramming(s),Greedy(s),GreedyOptimal,Approximation(s),ApproximationOptimal,ILP(s)\n";

    for (int i = 1; i <= datasetCount; ++i) {
        std::string datasetId = (i < 10 ? "0" + std::to_string(i) : std::to_string(i));
        std::string truckFile = basePath + "TruckAndPallets_" + datasetId + ".csv";
        std::string palletFile = basePath + "Pallets_" + datasetId + ".csv";

        if (!std::filesystem::exists(truckFile) || !std::filesystem::exists(palletFile)) {
            std::cout << "  Skipping dataset " << datasetId << " (files not found)\n";
            continue;
        }

        ProblemInstance instance = readData(truckFile, palletFile);
        std::cout << "\nRunning tests for dataset " << datasetId << "...\n";

        // --- Brute Force ---
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

        // --- Branch & Bound ---
        double timeBranch = -1.0;
        KnapsackResult branchRes;
        if (instance.pallets.size() <= BRUTE_FORCE_LIMIT) {
            timeBranch = averageTime([&]() {
                branchRes = bb_approach(instance);
            });
            std::cout << "  Branch and Bound Avg Time: " << timeBranch << "s\n";
        } else {
            std::cout << "  Branch and Bound skipped (too many pallets: " << instance.pallets.size() << ")\n";
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
        bool isOptimalGreedy = (greedyRes.maxProfit == dpRes.maxProfit);
        std::cout << "  Greedy Avg Time: " << timeGreedy << "s -> " << (isOptimalGreedy ? "OPTIMAL" : "NOT OPTIMAL") << "\n";

        // --- Approximation ---
        KnapsackResult appRes;
        double timeApp = averageTime([&]() {
            appRes = app_approach(instance);
        });
        bool isOptimalApp = (appRes.maxProfit == dpRes.maxProfit);
        std::cout << "  Approximation Avg Time: " << timeApp << "s -> " << (isOptimalApp ? "OPTIMAL" : "NOT OPTIMAL") << "\n";

        // --- ILP ---
        KnapsackResult ilpRes;
        double timeILP = averageTime([&]() {
            ilpRes = ilp_approach(instance);
        });
        std::cout << "  ILP Avg Time: " << timeILP << "s\n";

        // --- Write to CSV ---
        output << datasetId << ",";
        if (timeBrute < 0)
            output << "SKIPPED,SKIPPED,";
        else
            output << timeBrute << "," << timeBranch << ",";
        output << timeDP << "," << timeGreedy << "," << (isOptimalGreedy ? "Yes" : "No") << ",";
        output << timeApp << "," << (isOptimalApp ? "Yes" : "No") << ",";
        output << timeILP << "\n";
    }

    output.close();
    std::cout << "\nResults saved to " << outputPath << "\n";
}

void runTestMode() {
    runBenchmark("Provided", 10);
    runBenchmark("Own", 10);
}
