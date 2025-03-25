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
void writeInputToFile(const std::string& mode, int source, int destination, int maxWalkTime,
                      const std::string& avoidNodes, const std::string& avoidSegments, int includeNode) {
    std::ofstream inputFile("inputs/input.txt");

    if (!inputFile) {
        std::cerr << "Error opening input.txt for writing.\n";
        return;
    }

    inputFile << "Mode:" << mode << "\n";
    inputFile << "Source:" << source << "\n";
    inputFile << "Destination:" << destination << "\n";

    if (maxWalkTime != -1)
        inputFile << "MaxWalkTime:" << maxWalkTime << "\n";

    if (!avoidNodes.empty())
        inputFile << "AvoidNodes:" << avoidNodes << "\n";

    if (!avoidSegments.empty())
        inputFile << "AvoidSegments:" << avoidSegments << "\n";

    if (includeNode != -1)
        inputFile << "IncludeNode:" << includeNode << "\n";

    inputFile.close();
    std::cout << "✅ Data successfully saved in input.txt!\n";
}

// Function to get a valid mode from user
std::string getValidMode(int choice) {
    std::string mode;
    while (true) {
      if (choice == 1 || choice ==2) {
          std::cout << "Mode (driving): ";
          std::cin >> mode;

          if (mode == "driving")
              return mode;

          std::cout << "❌ Invalid mode. Please enter 'driving'.\n";
      } else if (choice ==3 ){
          std::cout << "Mode (driving-walking): ";
          std::cin >> mode;

          if (mode == "driving-walking")
              return mode;

          std::cout << "❌ Invalid mode. Please enter 'driving-walking'.\n";
      }
    }
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
            std::cout << "❌ Invalid input. Please enter a positive integer.\n";
        } else {
            return value;
        }
    }
}

// Function to get a valid list of nodes/segments
std::string getValidList(const std::string& prompt, const std::string& pattern) {
    std::string input;
    std::regex regexPattern(pattern);

    while (true) {
        std::cout << prompt;
        std::cin.ignore();
        std::getline(std::cin, input);

        if (input.empty() || std::regex_match(input, regexPattern))
            return input;

        std::cout << "❌ Invalid format. Please try again.\n";
    }
}

int main() {
    // Create the graph
    Graph<int> urbanGraph;
    loadGraph(urbanGraph);

    int choice = -1, source = -1, destination = -1, maxWalkTime = -1, includeNode = -1;
    std::string mode, avoidNodes, avoidSegments;

    do {
        std::cout << "\n===== Route Planning =====\n";
        std::cout << "1. Independent Route Planning\n";
        std::cout << "2. Restricted Route Planning\n";
        std::cout << "3. Environmentally Friendly Route Planning\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (choice == 1 || choice == 2 || choice == 3) {
            mode = getValidMode(choice);
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

        writeInputToFile(mode, source, destination, maxWalkTime, avoidNodes, avoidSegments, includeNode);
        switch (choice) {
            case 1:
                independentRoutePlanning(urbanGraph);
            break;
            case 2:
                restrictedRoutePlanning(urbanGraph);
            break;
            case 3:
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

    std::cout << "🚪 Program exited.\n";
    return 0;
}
