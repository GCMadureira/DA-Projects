//
// Created by Gabriel Sousa on 19/03/2025.
//


void independentRoutePlanning(Graph<int>& urbanGraph) {
    // Open the input and output files
    std::ifstream inFile("inputs/input.txt");
    if (!inFile.is_open()) {
        std::cout << "Could not open input.txt. Process aborted.\n";
        return;
    }
    std::ofstream outFile("inputs/output.txt");
    if (!outFile.is_open()) {
        std::cout << "Could not open output.txt. Process aborted.\n";
    }


    // Setup
    for (auto v : urbanGraph.getVertexSet()) {
        v->setIgnoreFlag(false);
        for (auto e : v->getAdj()) e->setIgnoreFlag(false);
    }
    int startNode, endNode;
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
        return;
    }

    // Parse source
    pos = sourceLine.find(':');
    if (pos != std::string::npos) {
        startNodeStr = sourceLine.substr(pos + 1);
        try {startNode = std::stoi(startNodeStr);}
        catch (const std::exception& e) {
            outFile << "Invalid node format. Source node must be a valid integer.\n";
            return;
        }
    } else {
        outFile << "Invalid input format. Expected 'Source:<id>'\n";
        return;
    }

    // Parse destination
    pos = destinationLine.find(':');
    if (pos != std::string::npos) {
        endNodeStr = destinationLine.substr(pos + 1);
        try {endNode = std::stoi(endNodeStr);}
        catch (const std::exception& e) {
            outFile << "Invalid node format. Destination node must be a valid integer.\n";
            return;
        }
    } else {
        outFile << "Invalid input format. Expected 'Destination:<id>'\n";
        return;
    }


    // Computations
    int firstPathLength = 0, secondPathLength = 0;
    Vertex<int>* endVertex = urbanGraph.findVertex(endNode);
    std::vector<int> firstPath, secondPath;
    dijkstra(&urbanGraph, startNode);

    if (endVertex != nullptr) firstPathLength = endVertex->getDist();
    firstPath = getPath(&urbanGraph, startNode, endNode);

    // Ignore all used Nodes to compute the Alternate Driving Route (except start and end nodes)
    for (int i = 1; i < firstPath.size() - 1; ++i) {
        auto vertex = urbanGraph.findVertex(firstPath[i]);
        vertex->setIgnoreFlag(true);
    }

    dijkstra(&urbanGraph, startNode);
    if (endVertex != nullptr) secondPathLength = endVertex->getDist();
    secondPath = getPath(&urbanGraph, startNode, endNode);


    // Result Output
    outFile << "Source: " << startNode << "\n"
            << "Destination: " << endNode << "\n";

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

void restrictedRoutePlanning(Graph<int>& urbanGraph) {
    std::ifstream inFile("inputs/input.txt");
    if (!inFile.is_open()) {
        std::cout << "Could not open input.txt. Process aborted.\n";
        return;
    }
    std::ofstream outFile("inputs/output.txt");
    if (!outFile.is_open()) {
        std::cout << "Could not open output.txt. Process aborted.\n";
        return;
    }

    std::string modeLine, sourceLine, destinationLine, avoidNodesLine, avoidSegmentsLine, includeNodeLine;
    std::string mode, startNodeStr, endNodeStr, avoidNodesStr, avoidSegmentsStr, includeNodeStr;
    int startNode, endNode, includeNode = -1;

    size_t pos;

    std::getline(inFile, modeLine);
    std::getline(inFile, sourceLine);
    std::getline(inFile, destinationLine);
    std::getline(inFile, avoidNodesLine);
    std::getline(inFile, avoidSegmentsLine);
    std::getline(inFile, includeNodeLine);

    pos = modeLine.find(':');
    if (pos != std::string::npos) mode = modeLine.substr(pos + 1);

    pos = sourceLine.find(':');
    if (pos != std::string::npos) startNodeStr = sourceLine.substr(pos + 1);

    pos = destinationLine.find(':');
    if (pos != std::string::npos) endNodeStr = destinationLine.substr(pos + 1);

    pos = avoidNodesLine.find(':');
    if (pos != std::string::npos) avoidNodesStr = avoidNodesLine.substr(pos + 1);

    pos = avoidSegmentsLine.find(':');
    if (pos != std::string::npos) avoidSegmentsStr = avoidSegmentsLine.substr(pos + 1);

    pos = includeNodeLine.find(':');
    if (pos != std::string::npos) includeNodeStr = includeNodeLine.substr(pos + 1);

    try {
        startNode = std::stoi(startNodeStr);
        endNode = std::stoi(endNodeStr);
        if (!includeNodeStr.empty()) {
            includeNode = std::stoi(includeNodeStr);
        }
    } catch (const std::exception &) {
        outFile << "Invalid node format. Source, Destination, and IncludeNode must be integers.\n";
        return;
    }

    std::stringstream ss(avoidNodesStr);
    std::string node;
    while (std::getline(ss, node, ',')) {
        try {
            int nodeId = std::stoi(node);
            auto vertex = urbanGraph.findVertex(nodeId);
            if (vertex) {  // Check if vertex is not nullptr
                vertex->setIgnoreFlag(true);
            } else {
                std::cout << "Warning: Node " << nodeId << " not found in graph.\n";
            }
        } catch (const std::exception&) {
            std::cout << "Warning: Invalid node format in AvoidNodes.\n";
        }
    }

    std::stringstream segSS(avoidSegmentsStr);
    std::string segment;
    while (std::getline(segSS, segment, ')')) {
        size_t openParen = segment.find('(');
        size_t comma = segment.find(',');
        if (openParen != std::string::npos && comma != std::string::npos) {
            try {
                int from = std::stoi(segment.substr(openParen + 1, comma - openParen - 1));
                int to = std::stoi(segment.substr(comma + 1));
                auto edge = urbanGraph.findEdge(from, to);
                if (edge) {  // Check if edge is not nullptr
                    edge->setIgnoreFlag(true);
                } else {
                    std::cout << "Warning: Edge (" << from << ", " << to << ") not found in graph.\n";
                }
            } catch (const std::exception&) {
                std::cout << "Warning: Invalid edge format in AvoidSegments.\n";
            }
        }
    }

    std::vector<int> firstPath, secondPath;
    Vertex<int>* endVertex = urbanGraph.findVertex(endNode);
    Vertex<int>* includeVertex = urbanGraph.findVertex(includeNode);
    int firstPathLength = 0, secondPathLength = 0;

    if (includeNode != -1) {
        auto vertex = urbanGraph.findVertex(includeNode);
        if (vertex) vertex->setIgnoreFlag(false);
    }

    dijkstra(&urbanGraph, startNode);
    if (includeNode == -1 || startNode == includeNode) {
        // No IncludeNode or it's the same as startNode to Direct Route
        if (endVertex != nullptr) firstPathLength = endVertex->getDist();
        firstPath = getPath(&urbanGraph, startNode, endNode);
    } else {
        // Path from startNode to includeNode
        if (includeVertex != nullptr) firstPathLength = includeVertex->getDist();
        firstPath = getPath(&urbanGraph, startNode, includeNode);

        if (firstPath.empty()) {
            std::cout << "Error: No path found from startNode " << startNode << " to includeNode " << includeNode << ".\n";
            outFile << "Source: " << startNode << "\n"
                    << "Destination: " << endNode << "\n"
                    << "RestrictedDrivingRoute: none\n";
            return;
        }

        dijkstra(&urbanGraph, includeNode);
        if (endVertex != nullptr) secondPathLength = endVertex->getDist();
        secondPath = getPath(&urbanGraph, includeNode, endNode, secondPathLength);

        if (secondPath.empty()) {
            std::cout << "Error: No path found from includeNode " << includeNode << " to endNode " << endNode << ".\n";
            outFile << "Source: " << startNode << "\n"
                    << "Destination: " << endNode << "\n"
                    << "RestrictedDrivingRoute: none\n";
            return;
        }
    }

    // Merge paths if includeNode exists
    if (!secondPath.empty()) {
        firstPath.insert(firstPath.end(), secondPath.begin() + 1, secondPath.end());
    }

    outFile << "Source: " << startNode << "\n"
            << "Destination: " << endNode << "\n"
            << "RestrictedDrivingRoute: " << firstPath[0];
    for (int i = 1; i < firstPath.size(); i++) {
        outFile << ", " << firstPath[i];
    }
    outFile << " (" << firstPathLength + secondPathLength << ")\n\n";
}

void environmentallyFriendlyRoutePlanning(Graph<int>& urbanGraph) {
    std::ifstream inFile("inputs/input.txt");
    if (!inFile.is_open()) {
        std::cout << "Could not open input.txt. Process aborted.\n";
        return;
    }

    std::ofstream outFile("inputs/output.txt");
    if (!outFile.is_open()) {
        std::cout << "Could not open output.txt. Process aborted.\n";
        return;
    }

    // Read input parameters
    std::string modeLine, sourceLine, destinationLine, maxWalkTimeLine, avoidNodesLine, avoidSegmentsLine;
    std::string startNodeStr, endNodeStr, maxWalkTimeStr, avoidNodesStr, avoidSegmentsStr;
    int startNode, endNode, maxWalkTime;

    std::getline(inFile, modeLine);
    std::getline(inFile, sourceLine);
    std::getline(inFile, destinationLine);
    std::getline(inFile, maxWalkTimeLine);
    std::getline(inFile, avoidNodesLine);
    std::getline(inFile, avoidSegmentsLine);

    // Extract values
    size_t pos;
    pos = sourceLine.find(':');
    if (pos != std::string::npos) startNodeStr = sourceLine.substr(pos + 1);
    pos = destinationLine.find(':');
    if (pos != std::string::npos) endNodeStr = destinationLine.substr(pos + 1);
    pos = maxWalkTimeLine.find(':');
    if (pos != std::string::npos) maxWalkTimeStr = maxWalkTimeLine.substr(pos + 1);
    pos = avoidNodesLine.find(':');
    if (pos != std::string::npos) avoidNodesStr = avoidNodesLine.substr(pos + 1);
    pos = avoidSegmentsLine.find(':');
    if (pos != std::string::npos) avoidSegmentsStr = avoidSegmentsLine.substr(pos + 1);

    // Convert string values to integers
    try {
        startNode = std::stoi(startNodeStr);
        endNode = std::stoi(endNodeStr);
        maxWalkTime = std::stoi(maxWalkTimeStr);
    } catch (const std::exception &) {
        outFile << "Invalid format. Ensure Source, Destination, and MaxWalkTime are valid integers.\n";
        return;
    }

    // Check constraints

    if (urbanGraph.areAdjacent(startNode, endNode)) {
        outFile << "Source: " << startNode << "\nDestination: " << endNode << "\n";
        outFile << "DrivingRoute:none\nParkingNode:none\nWalkingRoute:none\n";
        outFile << "Message: Origin and destination are adjacent, violating constraints.\n";
        return;
    }
    if(urbanGraph.findVertex(startNode)->hasParking() ||urbanGraph.findVertex(endNode)->hasParking()){
        outFile << "Source: " << startNode << "\nDestination: " << endNode << "\n";
        outFile << "DrivingRoute:none\nParkingNode:none\nWalkingRoute:none\n";
        outFile << "Message: Origin or destination is a parking node, violating constraints.\n";
        return;
    }

    std::stringstream ss(avoidNodesStr);
    std::string node;
    while (std::getline(ss, node, ',')) {
        try {
            int nodeId = std::stoi(node);
            auto vertex = urbanGraph.findVertex(nodeId);
            if (vertex) {  // Check if vertex exists
                vertex->setIgnoreFlag(true);
            } else {
                std::cout << "Warning: Node " << nodeId << " not found in graph.\n";
            }
        } catch (const std::exception&) {
            std::cout << "Warning: Invalid node format in AvoidNodes.\n";
        }
    }

    // Process AvoidSegments and set ignore flags for edges
    std::stringstream segSS(avoidSegmentsStr);
    std::string segment;
    while (std::getline(segSS, segment, ')')) {
        size_t openParen = segment.find('(');
        size_t comma = segment.find(',');
        if (openParen != std::string::npos && comma != std::string::npos) {
            try {
                int from = std::stoi(segment.substr(openParen + 1, comma - openParen - 1));
                int to = std::stoi(segment.substr(comma + 1));
                auto edge = urbanGraph.findEdge(from, to);
                if (edge) {  // Check if edge exists
                    edge->setIgnoreFlag(true);
                } else {
                    std::cout << "Warning: Edge (" << from << ", " << to << ") not found in graph.\n";
                }
            } catch (...) {
                std::cout << "Warning: Invalid edge format in AvoidSegments.\n";
            }
        }
    }

    dijkstra(&urbanGraph, startNode); // all driving routes from start node
    for (auto node : urbanGraph.getVertexSet()) {
        node->saveDist();
        node->savePath();
    }

    dijkstra(&urbanGraph, endNode, false); // all walking routes from end node

    int bestTotalTime = INT_MAX, bestParkingNode = -1;
    int firstSuggestedTotalTime = INT_MAX, firstSuggestedParkingNode = -1;
    int secondSuggestedTotalTime = INT_MAX, secondSuggestedParkingNode = -1;
    for (auto node : urbanGraph.getVertexSet()) {
        if (!node->hasParking()) continue;

        //                  driving time        walking time
        int totalTime = node->getSavedDist() + node->getDist();
        if (totalTime < bestTotalTime && node->getDist() <= maxWalkTime) { // obeys maxWalkTime restriction
            bestTotalTime = totalTime;
            bestParkingNode = node->getInfo();
        }
        else if (totalTime < firstSuggestedTotalTime) { // new best suggestion, push the old best suggestion to second place, remove the old second best suggestion
            secondSuggestedTotalTime = firstSuggestedTotalTime;
            secondSuggestedParkingNode = firstSuggestedParkingNode;
            firstSuggestedTotalTime = totalTime;
            firstSuggestedParkingNode = node->getInfo();
        }
        else if (totalTime < secondSuggestedTotalTime) { // new second best suggestion, remove the old second best suggestion
            secondSuggestedTotalTime = totalTime;
            secondSuggestedParkingNode = node->getInfo();
        }
    }


    // Output results
    outFile << "Source: " << startNode << "\nDestination: " << endNode << "\n";
    if (bestParkingNode == -1) {
        if (firstSuggestedParkingNode == -1 && secondSuggestedParkingNode == -1) { // no route obeying the restrictions or suggested route
            outFile << "DrivingRoute:none\nParkingNode:none\nWalkingRoute:none\nTotalTime:\n";
            outFile << "Message: No valid route found. Possible reasons: no suitable parking or max walking time exceeded.\n";
            return;
        }
        // no route obeying the restrictions but suggested route exists
        int drivingDistance = urbanGraph.findVertex(firstSuggestedParkingNode)->getSavedDist();
        int walkingDistance = urbanGraph.findVertex(firstSuggestedParkingNode)->getDist();
        vector<int> drivingPath = getPath(&urbanGraph, startNode, firstSuggestedParkingNode, true);
        vector<int> walkingPath = getPath(&urbanGraph,  endNode, firstSuggestedParkingNode);
        reverse(walkingPath.begin(), walkingPath.end());

        outFile << "DrivingRoute1:";
        for (size_t i = 0; i < drivingPath.size(); i++) {
            if (i > 0) outFile << ", ";
            outFile << drivingPath[i];
        }
        outFile << " (" << drivingDistance << ")\n";

        outFile << "ParkingNode1:" << firstSuggestedParkingNode << "\n";
        outFile << "WalkingRoute1:";
        for (size_t i = 0; i < walkingPath.size(); i++) {
            if (i > 0) outFile << ", ";
            outFile << walkingPath[i];
        }
        outFile << " (" << walkingDistance << ")\n";
        outFile << "TotalTime1:" << firstSuggestedTotalTime << "\n";

        if (secondSuggestedParkingNode == -1) { // second suggestion does not exist
            outFile << "DrivingRoute2:none\nParkingNode2:none\nWalkingRoute2:none\nTotalTime2:\n";
            return;
        }

        // second suggestion
        drivingDistance = urbanGraph.findVertex(secondSuggestedParkingNode)->getSavedDist();
        walkingDistance = urbanGraph.findVertex(secondSuggestedParkingNode)->getDist();
        drivingPath = getPath(&urbanGraph, startNode, secondSuggestedParkingNode, true);
        walkingPath = getPath(&urbanGraph,  endNode, secondSuggestedParkingNode);
        reverse(walkingPath.begin(), walkingPath.end());

        outFile << "DrivingRoute2:";
        for (size_t i = 0; i < drivingPath.size(); i++) {
            if (i > 0) outFile << ", ";
            outFile << drivingPath[i];
        }
        outFile << " (" << drivingDistance << ")\n";

        outFile << "ParkingNode2:" << secondSuggestedParkingNode << "\n";
        outFile << "WalkingRoute2:";
        for (size_t i = 0; i < walkingPath.size(); i++) {
            if (i > 0) outFile << ", ";
            outFile << walkingPath[i];
        }
        outFile << " (" << walkingDistance << ")\n";
        outFile << "TotalTime2:" << secondSuggestedTotalTime << "\n";

    } else {
        int drivingDistance = urbanGraph.findVertex(bestParkingNode)->getSavedDist();
        int walkingDistance = urbanGraph.findVertex(bestParkingNode)->getDist();
        vector<int> bestDrivingPath = getPath(&urbanGraph, startNode, bestParkingNode, true);
        vector<int> bestWalkingPath = getPath(&urbanGraph,  endNode, bestParkingNode);
        reverse(bestWalkingPath.begin(), bestWalkingPath.end());

        outFile << "DrivingRoute:";
        for (size_t i = 0; i < bestDrivingPath.size(); i++) {
            if (i > 0) outFile << ", ";
            outFile << bestDrivingPath[i];
        }
        outFile << " (" << drivingDistance << ")\n";

        outFile << "ParkingNode:" << bestParkingNode << "\n";
        outFile << "WalkingRoute:";
        for (size_t i = 0; i < bestWalkingPath.size(); i++) {
            if (i > 0) outFile << ", ";
            outFile << bestWalkingPath[i];
        }
        outFile << " (" << walkingDistance << ")\n";
        outFile << "TotalTime:" << bestTotalTime << "\n";
    }
}

