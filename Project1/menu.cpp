#include "menu.h"

// Function to write inputs to input.txt
void writeInputToFile(int choice, int source, int destination, int maxWalkTime,
                      const std::string& avoidNodes, const std::string& avoidSegments, int includeNode) {
    std::ofstream inputFile("inputs/input.txt");

    if (!inputFile) {
        std::cerr << "Error opening input.txt for writing.\n";
        return;
    }

    if (choice == 1 || choice == 2) {
        inputFile << "Mode:driving\n";
    }
    else if (choice == 3){
        inputFile << "Mode:driving-walking\n";
    }

    inputFile << "Source:" << source << "\n";
    inputFile << "Destination:" << destination << "\n";

    if (maxWalkTime != -1)
        inputFile << "MaxWalkTime:" << maxWalkTime << "\n";

    inputFile << "AvoidNodes:" << avoidNodes << "\n";
    inputFile << "AvoidSegments:" << avoidSegments << "\n";
    inputFile << "IncludeNode:" << (includeNode == -1 ? "" : std::to_string(includeNode)) << "\n";

    inputFile.close();
    std::cout << "\nData successfully saved in input.txt!\n";
}

// Function to get a valid integer (positive)
int getValidInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;


        if (std::cin.fail() || value < 1) {
            std::cin.clear();  // Clear error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Ignore invalid input
            std::cout << "Invalid input. Please enter a positive integer.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear buffer before input
            return value;
        }
    }
}

// Function to get a valid list of nodes/segments
std::string getValidList(const std::string& prompt, const std::string& pattern) {
    std::string input;
    std::regex regexPattern(pattern);

    std::cout << prompt;

    while (true) {
        std::getline(std::cin, input);

        if (input.empty())  // Allow skipping
            return "";

        if (std::regex_match(input, regexPattern))
            return input;

        std::cout << "Invalid format. Please try again.\n" << prompt;
    }
}

void receiveInput(const int choice){
    int source = -1, destination = -1, maxWalkTime = -1, includeNode = -1;
    std::string avoidNodes, avoidSegments;

    source = getValidInt("Source: ");
    destination = getValidInt("Destination: ");

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

int selectMode(const int choice){
    int mode = -1;
    do {
        std::cout << "\n===== Mode Selection =====\n";
        std::cout << "1. Manual Input\n";
        std::cout << "2. Automatic Input\n";
        std::cout << "3. Back\n";
        std::cout << "Enter your choice: ";
        std::cin >> mode;

        switch (mode) {
            case 1:
                receiveInput(choice);
            break;
            case 2:
                // automatic input so do nothing
                break;
            case 3:
                return 1;
            default:
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid choice. Please try again.\n";
            break;
        }
    } while (mode > 3 || mode < 1);
    return 0;
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
                if (selectMode(choice)) break; // if user goes back from the other menu do not execute the algorithm
                independentRoutePlanning(urbanGraph);
            break;
            case 2:
                if (selectMode(choice)) break;
                restrictedRoutePlanning(urbanGraph);
            break;
            case 3:
                if (selectMode(choice)) break;
                environmentallyFriendlyRoutePlanning(urbanGraph);
            break;
            case 4:
                std::cout << "Exiting program.\n";
            break;
            default:
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid choice. Please try again.\n";
            break;
        }
    } while (choice != 4);

    std::cout << "Program exited.\n";
    return 0;
}
