//
// Created by Gabriel Sousa on 19/03/2025.
//

// Batch mode function that reads from input.txt and writes to output.txt.
void IndependentRoutePlanning(Graph<int>& urbanGraph) {
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

    std::string modeLine, sourceLine, destinationLine;
    std::string mode, startNodeStr, endNodeStr;;
    int startNode, endNode;

    // Extract values after ':'
    size_t pos;

    // Read expected lines
    std::getline(inFile, modeLine);
    std::getline(inFile, sourceLine);
    std::getline(inFile, destinationLine);

    // Parse mode
    pos = modeLine.find(':');
    if (pos != std::string::npos) {
        mode = modeLine.substr(pos + 1);
    } else {
        outFile << "Invalid input format. Expected 'Mode:<value>'\n";
        return;
    }

    // Parse source
    pos = sourceLine.find(':');
    if (pos != std::string::npos) {
        startNodeStr = sourceLine.substr(pos + 1);
    } else {
        outFile << "Invalid input format. Expected 'Source:<id>'\n";
        return;
    }

    // Parse destination
    pos = destinationLine.find(':');
    if (pos != std::string::npos) {
        endNodeStr = destinationLine.substr(pos + 1);
    } else {
        outFile << "Invalid input format. Expected 'Destination:<id>'\n";
        return;
    }

    // Convert startNode and endNode to integers
    try {
        startNode = std::stoi(startNodeStr);
        endNode = std::stoi(endNodeStr);
    } catch (const std::exception& e) {
        outFile << "Invalid node format. Source and Destination must be integers.\n";
        return;
    }

    //  TODO: Call algorithm.
    // Allow every vertex to be taken
    for (auto v : urbanGraph.getVertexSet()) {
        v->setIgnoreFlag(false);
    }

    // Compute Best Driving Route
    int pathLength;
    dijkstra(&urbanGraph, startNode);
    std::vector<int> path = getPath(&urbanGraph, startNode, endNode, pathLength);
    if (path.empty()) {
        outFile << "BestDrivingRoute: none\n"
                << "AlternateDrivingRoute: none\n";
        return;
    }

    outFile << "Source: " << startNode << "\n"
            << "Destination: " << endNode << "\n"
            << "BestDrivingRoute: " << path[0];
    for (int i = 1; i < path.size(); i++) {
        outFile << ", " << path[i];
    }
    outFile << " (" << pathLength << ")" << "\n";

    // Ignore all used Nodes to compute the Alternate Driving Route (except start and end nodes)
    for (int i = 1; i < path.size() - 1; ++i) {
        auto vertex = urbanGraph.findVertex(path[i]);
        vertex->setIgnoreFlag(true);
    }


    // Second iteration of the algorithm to compute the Alternative Driving Route
    dijkstra(&urbanGraph, startNode);
    path = getPath(&urbanGraph, startNode, endNode, pathLength);
    if (path.empty()) {
        outFile << "AlternateDrivingRoute: none\n";
        return;
    }

    outFile << "AlternateDrivingRoute: " << path[0];
    for (int i = 1; i < path.size(); i++) {
        outFile << ", " << path[i];
    }
    outFile << " (" << pathLength << ")" << "\n\n";


    outFile << "Batch processing completed successfully.\n";
}