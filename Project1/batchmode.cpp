//
// Created by Gabriel Sousa on 19/03/2025.
//



/*
    Parses the input file and applies all restrictions; sets the start, end and include nodes on the arguments
    Return values:
        -1: error
         0: driving and alternate driving routes
         1: restricted driving route
         (can add more later)
*/
int parseInputFile(std::ifstream& inFile, std::ofstream& outFile, Graph<int>& urbanGraph,
                    int& startNode, int& endNode, int& includeNode) {
    std::string modeLine, sourceLine, destinationLine, avoidNodesLine, avoidSegmentsLine, includeNodeLine;
    std::string modeStr, startNodeStr, endNodeStr, avoidNodesStr, avoidSegmentsStr, includeNodeStr;
    std::vector<int> avoidNodes;
    std::vector<std::pair<int, int>> avoidSegments;

    // Extract values after ':'
    size_t pos;

     // Read expected lines
    std::getline(inFile, modeLine);
    std::getline(inFile, sourceLine);
    std::getline(inFile, destinationLine);
    std::getline(inFile, avoidNodesLine);
    std::getline(inFile, avoidSegmentsLine);
    std::getline(inFile, includeNodeLine);

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
        try {startNode = std::stoi(startNodeStr);}
        catch (const std::exception& e) {
            outFile << "Invalid node format. Source node must be a valid integer.\n";
            return -1;
        }
    } else {
        outFile << "Invalid input format. Expected 'Source:<id>'\n";
        return -1;
    }

    // Parse destination
    pos = destinationLine.find(':');
    if (pos != std::string::npos) {
        endNodeStr = destinationLine.substr(pos + 1);
        try {endNode = std::stoi(endNodeStr);}
        catch (const std::exception& e) {
            outFile << "Invalid node format. Destination node must be a valid integer.\n";
            return -1;
        }
    } else {
        outFile << "Invalid input format. Expected 'Destination:<id>'\n";
        return -1;
    }

    //Parse avoidNodes
    pos=avoidNodesLine.find(':');
    if (pos != std::string::npos) {
        avoidNodesStr = avoidNodesLine.substr(pos + 1);
    }
    std::istringstream avoidNodesStream(avoidNodesStr);
    std::string node;
    while (std::getline(avoidNodesStream, node, ',')) {
        avoidNodes.push_back(std::stoi(node));
    }
    if (std::getline(avoidNodesStream, node)) // read the last node
        avoidNodes.push_back(std::stoi(node));

    //Parse avoidSegments
    pos = avoidSegmentsLine.find(':');
    if (pos != std::string::npos) {
        avoidSegmentsStr = avoidSegmentsLine.substr(pos + 1);
    }
    std::istringstream avoidSegmentsStream(avoidSegmentsStr);
    std::string segment;
    while (std::getline(avoidSegmentsStream, segment, ',')) {
        int u, v;
        if (sscanf(segment.c_str(), "(%d,%d)", &u, &v) == 2) {
            avoidSegments.emplace_back(u, v);
        }
    }
    if (std::getline(avoidNodesStream, node)) { // read the last segment
        int u, v;
        if (sscanf(segment.c_str(), "(%d,%d)", &u, &v) == 2) {
            avoidSegments.emplace_back(u, v);
        }
    }

    //Parse includeNode
    pos = includeNodeLine.find(':');
    if (pos != std::string::npos) {
        includeNodeStr = includeNodeLine.substr(pos + 1);
        try {includeNode = std::stoi(includeNodeStr);}
        catch (const std::exception& e) {
            outFile << "Invalid node format. Include node must be a valid integer.\n";
        }
    }

    // Apply restrictions
    for (int node : avoidNodes) {
        auto v = urbanGraph.findVertex(node);
        if (v) v->setIgnoreFlag(true);
    }
    for (auto& seg : avoidSegments) {
        auto e = urbanGraph.findEdge(seg.first, seg.second);
        if (e) e->setIgnoreFlag(true);
    }

    return (!avoidNodes.empty() || !avoidSegments.empty() || includeNode != -1);
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
    int startNode, endNode, includeNode = -1;
    int mode = parseInputFile(inFile, outFile, urbanGraph, startNode, endNode, includeNode);
    if (mode == -1) return; // error


    // Computations
    int firstPathLength = 0, secondPathLength = 0;
    std::vector<int> firstPath, secondPath;
    dijkstra(&urbanGraph, startNode);
    if (includeNode != -1) { // Two-step path calculation via includeNode
        firstPath = getPath(&urbanGraph, startNode, includeNode, firstPathLength);
        dijkstra(&urbanGraph, includeNode);
        secondPath = getPath(&urbanGraph, includeNode, endNode, secondPathLength);
    }
    else { // Best and Alternate Driving Routes
        firstPath = getPath(&urbanGraph, startNode, endNode, firstPathLength);
        // Ignore all used Nodes to compute the Alternate Driving Route (except start and end nodes)
        for (int i = 1; i < firstPath.size() - 1; ++i) {
            auto vertex = urbanGraph.findVertex(firstPath[i]);
            vertex->setIgnoreFlag(true);
        }
        secondPath = getPath(&urbanGraph, startNode, endNode, secondPathLength);
    }


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
    else if (mode == 1) { // Restricted Driving Route
        outFile << "RestrictedDrivingRoute: ";
        if (firstPath.empty() || secondPath.empty()) {
            outFile << "none\n";
        } else {
            outFile << firstPath[0];
            for (size_t i = 1; i < firstPath.size(); ++i) {
                outFile << ", " << firstPath[i];
            }
            for (size_t i = 1; i < secondPath.size(); ++i) {
                outFile << ", " << secondPath[i];
            }
            outFile << " (" << (firstPathLength + secondPathLength) << ")\n";
        }
    }
    outFile << "Batch processing completed successfully.\n";
}