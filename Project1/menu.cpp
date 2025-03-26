#include <iostream>
#include <fstream>
#include <limits>
#include <regex>
#include "data_structures/Graph.h"
#include "data_structures/MutablePriorityQueue.h"
#include "parsing.cpp"
#include "search.cpp"
#include "parsing.h"
#include "batchmode.cpp"
#include "search.h"
#include <climits>

// Function to write inputs to input.txt
void writeInputToFile(int choice, int source, int destination, int maxWalkTime,
                      const std::string& avoidNodes, const std::string& avoidSegments, int includeNode) {
    std::ofstream inputFile("inputs/input.txt");

    if (!inputFile) {
        std::cerr << "Error opening input.txt for writing.\n";
        return;
    }

    if (choice == 1 || choice ==2) {
        inputFile << "Mode:driving\n";
    }
    else if (choice ==3 ){
        inputFile << "Mode:driving-walking\n";
    }

    inputFile << "Source:" << source << "\n";
    inputFile << "Destination:" << destination << "\n";

    if (maxWalkTime != -1)
        inputFile << "MaxWalkTime:" << maxWalkTime << "\n";

    inputFile << "AvoidNodes:" << (avoidNodes.empty() ? "none" : avoidNodes) << "\n";
    inputFile << "AvoidSegments:" << (avoidSegments.empty() ? "none" : avoidSegments) << "\n";
    inputFile << "IncludeNode:" << (includeNode == -1 ? "none" : std::to_string(includeNode)) << "\n";

    inputFile.close();
    std::cout << "Data successfully saved in input.txt!\n";
}

// Function to get a valid integer (positive)
int getValidInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;


        if (std::cin.fail() || value < 0) {
            std::cin.clear();  // Clear error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Ignore invalid input
            std::cout << "Invalid input. Please enter a positive integer.\n";
        } else {
            return value;
        }
    }
}

// Function to get a valid list of nodes/segments
std::string getValidList(const std::string& prompt, const std::string& pattern) {
    std::string input;
    std::regex regexPattern(pattern);

    std::cout << prompt;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear buffer before input

    while (true) {
        std::getline(std::cin, input);

        if (input.empty())  // Allow skipping
            return "none";

        if (std::regex_match(input, regexPattern))
            return input;

        std::cout << "Invalid format. Please try again.\n" << prompt;
    }
}

void receiveInput(int choice){

    int source = -1, destination = -1, maxWalkTime = -1, includeNode = -1;
    std::string avoidNodes, avoidSegments;

    if (choice == 1 || choice == 2 || choice == 3) {
        source = getValidInt("Source: ");
        destination = getValidInt("Destination: ");
    }

    if (choice == 2) {
        avoidNodes = getValidList("AvoidNodes (e.g., 1,2,3 or empty): ", R"(^\d+(,\d+)*$|^$)");
        avoidSegments = getValidList("AvoidSegments (e.g., (1,2),(3,4) or empty): ", R"(^\(\d+,\d+\)(,\(\d+,\d+\))*$|^$)");
        includeNode = getValidInt("IncludeNode: ");
    } else if (choice == 3) {
        maxWalkTime = getValidInt("MaxWalkTime: ");
        avoidNodes = getValidList("AvoidNodes (e.g., 1,2,3 or empty): ", R"(^\d+(,\d+)*$|^$)");
        avoidSegments = getValidList("AvoidSegments (e.g., (1,2),(3,4) or empty): ", R"(^\(\d+,\d+\)(,\(\d+,\d+\))*$|^$)");
    }

    writeInputToFile(choice, source, destination, maxWalkTime, avoidNodes, avoidSegments, includeNode);
}

void selectMode(int choice){
    int mode;
    std::cout << "\n===== Mode Selection =====\n";
    std::cout << "1. Manual Input\n";
    std::cout << "2. Automatic Input\n";
    std::cout << "Enter your choice: ";
    std::cin >> mode;

    if(mode==1){
        receiveInput(choice);
    }
    if(mode>2){
        std::cout<< "\nInvalid Choice\n";
        std::cout << "Enter a valid choice: ";
    }
}

int main() {
    // Create the graph
    Graph<int> urbanGraph;
    loadGraph(urbanGraph);

    int choice = -1;


    do {
        std::cout << "\n===== Route Planning =====\n";
        std::cout << "1. Independent Route Planning\n";
        std::cout << "2. Restricted Route Planning\n";
        std::cout << "3. Environmentally Friendly Route Planning\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                selectMode(choice);
                independentRoutePlanning(urbanGraph);
            break;
            case 2:
                selectMode(choice);
                restrictedRoutePlanning(urbanGraph);
            break;
            case 3:
                selectMode(choice);
                environmentallyFriendlyRoutePlanning(urbanGraph);
            break;
            case 4:
                std::cout << "Exiting program.\n";
            break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
            break;
        }
    } while (choice != 4);

    std::cout << "Program exited.\n";
    return 0;
}
