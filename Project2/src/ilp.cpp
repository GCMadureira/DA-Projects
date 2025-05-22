//
// Created by GustavoCM on 15/05/2025.
//

#include "algorithms.h"



KnapsackResult ilp_approach(const ProblemInstance& instance) {
    std::ofstream input("../ilp_solver/input.txt");
    std::ifstream output("../ilp_solver/output.txt");
    if (!input.is_open() || !output.is_open()) {
        std::cerr << "Error: Could not open the input or/and the output files.\n";
        return {};
    }

    //the first line is the number of items, the second is the max weight
    input << instance.pallets.size() << std::endl;
    input << instance.truckCapacity << std::endl;

    //the third line is the weights of the pallets separated by spaces
    for (Pallet pallet : instance.pallets)
        input << pallet.weight << " ";
    input << std::endl;

    //the fourth and last line is the profits of the pallets separated by spaces
    for (Pallet pallet : instance.pallets)
        input << pallet.profit << " ";
    input << std::endl;

    //call the solver
    int ret = system("python ../ilp_solver/knapsack_solver.py ../ilp_solver/input.txt ../ilp_solver/output.txt");
    if (ret != 0) {
        std::cerr << "Failed to run knapsack_solver.py" << std::endl;
        return {};
    }

    int index;
    KnapsackResult result;
    std::unordered_set<int> selectedPallets;

    std::string line;

    //get the total profit on the first line
    std::getline(output, line);
    result.maxProfit = std::stoi(line);

    //get the total weight on the second line
    std::getline(output, line);
    result.occupiedWeight = std::stoi(line);

    //get the selected items' ids
    std::getline(output, line);
    std::istringstream iss(line);
    while (iss >> index) {
        selectedPallets.insert(index + 1);
    }

    // create the solution
    for (const Pallet& pallet : instance.pallets) {
        if (selectedPallets.contains(pallet.id)) {
            result.selectedPallets.push_back(pallet);
        }
    }



    return result;
}