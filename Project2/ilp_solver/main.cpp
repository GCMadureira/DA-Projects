#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>

int main() {
    // Run Python script (must be .py)
    int ret = system("python knapsack_solver.py input.txt output.txt");
    if (ret != 0) {
        std::cerr << "Failed to run knapsack_solver.py" << std::endl;
        return 1;
    }

    // Open output.txt
    std::ifstream infile("output.txt");
    if (!infile.is_open()) {
        std::cerr << "Error opening output.txt" << std::endl;
        return 1;
    }

    int totalProfit, totalWeight, totalCapacity;
    std::string line;

    std::getline(infile, line);
    totalProfit = std::stoi(line);

    std::getline(infile, line);
    totalWeight = std::stoi(line);

    std::getline(infile, line);
    totalCapacity = std::stoi(line);

    std::getline(infile, line);
    std::istringstream iss(line);
    std::vector<int> selectedIndices;
    int index;
    while (iss >> index) {
        selectedIndices.push_back(index);
    }

    infile.close();

    // Print results
    std::cout << "=== Knapsack Results (Python ILP) ===" << std::endl;
    std::cout << "Total Profit: " << totalProfit << std::endl;
    std::cout << "Total Weight: " << totalWeight << " / " << totalCapacity << std::endl;

    std::cout << std::endl;

    return 0;
}
