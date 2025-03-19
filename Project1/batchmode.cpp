//
// Created by Gabriel Sousa on 19/03/2025.
//


// Parses the input file and applies all restrictions; sets the start and end nodes on the arguments
int parseInputFile(std::ifstream& inFile, std::ofstream& outFile, Graph<int>& urbanGraph,
                    int& startNode, int& endNode) {
    std::string modeLine, sourceLine, destinationLine;
    std::string modeStr, startNodeStr, endNodeStr;

    // Extract values after ':'
    size_t pos;

     // Read expected lines
    std::getline(inFile, modeLine);
    std::getline(inFile, sourceLine);
    std::getline(inFile, destinationLine);

    // Parse mode
    pos = modeLine.find(':');
    if (pos != std::string::npos) {
        modeStr = modeLine.substr(pos + 1);
    } else {
        outFile << "Invalid input format. Expected 'Mode:<value>'\n";
        return -1;
    }

    // Parse source
    pos = sourceLine.find(':');
    if (pos != std::string::npos) {
        startNodeStr = sourceLine.substr(pos + 1);
    } else {
        outFile << "Invalid input format. Expected 'Source:<id>'\n";
        return -1;
    }

    // Parse destination
    pos = destinationLine.find(':');
    if (pos != std::string::npos) {
        endNodeStr = destinationLine.substr(pos + 1);
    } else {
        outFile << "Invalid input format. Expected 'Destination:<id>'\n";
        return -1;
    }

  	// Convert startNode and endNode to integers
    try {
        startNode = std::stoi(startNodeStr);
        endNode = std::stoi(endNodeStr);
    } catch (const std::exception& e) {
        outFile << "Invalid node format. Source and Destination must be integers.\n";
        return -1;
    }

    // Allow every vertex to be taken
    for (auto v : urbanGraph.getVertexSet()) {
        v->setIgnoreFlag(false);
    }

    return 0;
}




// Batch mode function that reads from input.txt and writes to output.txt.
void independentRoutePlanning(Graph<int>& urbanGraph) {
    // Open the input and output files
    std::ifstream inFile("inputs/input.txt");
    if (!inFile.is_open()) {
        std::cerr << "Could not open input.txt. Batch mode aborted.\n";
        return;
    }

    std::ofstream outFile("inputs/output.txt");
    if (!outFile.is_open()) {
        std::cerr << "Could not open output.txt. Batch mode aborted.\n";
        return;
    }

    // Setup
    for (auto v : urbanGraph.getVertexSet()) {
        v->setIgnoreFlag(false);
    }
    int startNode = 1, endNode = 5;
    int mode = parseInputFile(inFile, outFile, urbanGraph, startNode, endNode);
    if (mode == -1) return; // error


    // Computations
    int firstPathLength = 0, secondPathLength = 0;
    std::vector<int> firstPath, secondPath;
    dijkstra(&urbanGraph, startNode);
    firstPath = getPath(&urbanGraph, startNode, endNode, firstPathLength);

    // Ignore all used Nodes to compute the Alternate Driving Route (except start and end nodes)
    for (int i = 1; i < firstPath.size() - 1; ++i) {
        auto vertex = urbanGraph.findVertex(firstPath[i]);
        vertex->setIgnoreFlag(true);
    }

    secondPath = getPath(&urbanGraph, startNode, endNode, secondPathLength);


    // Result Output
    outFile << "Source: " << startNode << "\n"
            << "Destination: " << endNode << "\n";
    if (mode == 0) { // Normal Driving Route
        if (firstPath.empty()) {
            outFile << "BestDrivingRoute: none\n"
                    << "AlternateDrivingRoute: none\n";
            return;
        }

        outFile << "BestDrivingRoute: " << firstPath[0];
        for (int i = 1; i < firstPath.size(); i++) {
            outFile << ", " << firstPath[i];
        }
        outFile << " (" << firstPathLength << ")" << "\n";


        if (secondPath.empty()) {
            outFile << "AlternateDrivingRoute: none\n";
            return;
        }

        outFile << "AlternateDrivingRoute: " << secondPath[0];
        for (int i = 1; i < secondPath.size(); i++) {
            outFile << ", " << secondPath[i];
        }
        outFile << " (" << secondPathLength << ")" << "\n\n";
    }
    outFile << "Batch processing completed successfully.\n";
}