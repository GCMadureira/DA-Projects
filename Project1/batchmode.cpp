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
    std::string startNodeStr, endNodeStr;

    // Extract values after ':'
    size_t pos;

    // Read expected lines
    std::getline(inFile, modeLine);
    std::getline(inFile, sourceLine);
    std::getline(inFile, destinationLine);

    // Parse source
    pos = sourceLine.find(':');
    if (pos != std::string::npos) {
        startNodeStr = sourceLine.substr(pos + 1);
        try {startNode = std::stoi(startNodeStr);}
        catch (const std::exception& e) {
            outFile << "Invalid node format. Source node must be a valid integer.\n";
            return;
        }
        if (urbanGraph.findVertex(startNode) == nullptr) {
            outFile << "The source node " << startNode << " does not exist.\n";
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
        if (urbanGraph.findVertex(endNode) == nullptr) {
            outFile << "The destination node " << endNode << " does not exist.\n";
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

    firstPathLength = endVertex->getDist();
    firstPath = getPath(&urbanGraph, startNode, endNode);

    // Ignore all used Nodes to compute the Alternate Driving Route (except start and end nodes)
    for (int i = 1; i < firstPath.size() - 1; ++i) {
        auto vertex = urbanGraph.findVertex(firstPath[i]);
        vertex->setIgnoreFlag(true);
    }

    dijkstra(&urbanGraph, startNode);
    secondPathLength = endVertex->getDist();
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
    // Open the input and output files
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

    // Setup
    for (auto v : urbanGraph.getVertexSet()) {
        v->setIgnoreFlag(false);
        for (auto e : v->getAdj()) e->setIgnoreFlag(false);
    }

    std::string modeLine, sourceLine, destinationLine, avoidNodesLine, avoidSegmentsLine, includeNodeLine;
    std::string startNodeStr, endNodeStr, avoidNodesStr, avoidSegmentsStr, includeNodeStr;
    int startNode, endNode, includeNode = -1;

    // Read expected lines
    std::getline(inFile, modeLine);
    std::getline(inFile, sourceLine);
    std::getline(inFile, destinationLine);
    std::getline(inFile, avoidNodesLine);
    std::getline(inFile, avoidSegmentsLine);
    std::getline(inFile, includeNodeLine);

    // Extract values after ':'
    size_t pos;

    // Parse source
    pos = sourceLine.find(':');
    if (pos != std::string::npos) {
        startNodeStr = sourceLine.substr(pos + 1);
        try {startNode = std::stoi(startNodeStr);}
        catch (const std::exception& e) {
            outFile << "Invalid node format. Source node must be a valid integer.\n";
            return;
        }
        if (urbanGraph.findVertex(startNode) == nullptr) {
            outFile << "The source node " << startNode << " does not exist.\n";
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
        if (urbanGraph.findVertex(endNode) == nullptr) {
            outFile << "The destination node " << endNode << " does not exist.\n";
            return;
        }
    } else {
        outFile << "Invalid input format. Expected 'Destination:<id>'\n";
        return;
    }

    // Parse include node
    pos = includeNodeLine.find(':');
    if (pos != std::string::npos) {
        includeNodeStr = includeNodeLine.substr(pos + 1);
        if (!includeNodeStr.empty()) {
            try {includeNode = std::stoi(includeNodeStr);}
            catch (const std::exception& e) {
                outFile << "Warning: Invalid node format. Include node must be a valid integer. Ignoring include node.\n";
                includeNode = -1;
            }
            if (urbanGraph.findVertex(includeNode) == nullptr) {
                outFile << "Warning: The include node " << includeNode << " does not exist. Ignoring include node.\n";
                includeNode = -1;
            }
        }
    } else {
        outFile << "Invalid input format. Expected 'IncludeNode:<id>'\n";
        return;
    }

    // Parse avoid nodes
    pos = avoidNodesLine.find(':');
    if (pos != std::string::npos) avoidNodesStr = avoidNodesLine.substr(pos + 1);
    std::stringstream ss(avoidNodesStr);
    std::string nodeStr;
    while (std::getline(ss, nodeStr, ',')) {
        std::cout << "\n\n" << nodeStr << "\n\n";
        try {
            int nodeId = std::stoi(nodeStr);
            if (nodeId == startNode || nodeId == endNode) {
                std::cout << "Warning: Avoid nodes cannot be the same as the source or destination nodes.\n";
            }
            else if (auto vertex = urbanGraph.findVertex(nodeId)) {  // Check if vertex is not nullptr
                vertex->setIgnoreFlag(true);
            } else {
                std::cout << "Warning: Node " << nodeId << " not found in graph.\n";
            }
        } catch (const std::exception&) {
            std::cout << "Warning: Invalid node format in AvoidNodes.\n";
        }
    }

    // Parse avoid segments
    pos = avoidSegmentsLine.find(':');
    if (pos != std::string::npos) avoidSegmentsStr = avoidSegmentsLine.substr(pos + 1);
    std::stringstream segSS(avoidSegmentsStr);
    std::string segmentStr;
    while (std::getline(segSS, segmentStr, ')')) {
        std::cout << "\n\n" << segmentStr << "\n\n";
        size_t openParen = segmentStr.find('(');
        size_t comma = segmentStr.find(',', 1);
        if (openParen != std::string::npos && comma != std::string::npos) {
            try {
                int from = std::stoi(segmentStr.substr(openParen + 1, comma - openParen - 1));
                int to = std::stoi(segmentStr.substr(comma + 1));
                if (auto edge = urbanGraph.findEdge(from, to)) {  // Check if edge is not nullptr
                    edge->setIgnoreFlag(true);
                } else {
                    std::cout << "Warning: Edge (" << from << ", " << to << ") not found in graph.\n";
                }
            } catch (const std::exception&) {
                std::cout << "Warning: Invalid edge format in AvoidSegments.\n";
            }
        }
    }

    // Computations
    std::vector<int> firstPath, secondPath;
    Vertex<int>* endVertex = urbanGraph.findVertex(endNode);
    Vertex<int>* includeVertex = urbanGraph.findVertex(includeNode);
    int firstPathLength = 0, secondPathLength = 0;

    dijkstra(&urbanGraph, startNode);
    if (includeNode == -1 || startNode == includeNode || endNode == includeNode) {
        // No IncludeNode or it's the same as startNode/endNode
        firstPathLength = endVertex->getDist();
        firstPath = getPath(&urbanGraph, startNode, endNode);
    } else {
        // Path from startNode to includeNode
        firstPathLength = includeVertex->getDist();
        firstPath = getPath(&urbanGraph, startNode, includeNode);

        if (firstPath.empty()) {
            std::cout << "Error: No path found from startNode " << startNode << " to includeNode " << includeNode << ".\n";
            outFile << "Source: " << startNode << "\n"
                    << "Destination: " << endNode << "\n"
                    << "RestrictedDrivingRoute: none\n";
            return;
        }

        dijkstra(&urbanGraph, includeNode);
        secondPathLength = endVertex->getDist();
        secondPath = getPath(&urbanGraph, includeNode, endNode);

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
    // Open the input and output files
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

    // Extract values after ':'
    size_t pos;

    // Parse source
    pos = sourceLine.find(':');
    if (pos != std::string::npos) {
        startNodeStr = sourceLine.substr(pos + 1);
        try {startNode = std::stoi(startNodeStr);}
        catch (const std::exception& e) {
            outFile << "Invalid node format. Source node must be a valid integer.\n";
            return;
        }
        if (urbanGraph.findVertex(startNode) == nullptr) {
            outFile << "The source node " << startNode << " does not exist.\n";
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
        if (urbanGraph.findVertex(endNode) == nullptr) {
            outFile << "The destination node " << endNode << " does not exist.\n";
            return;
        }
    } else {
        outFile << "Invalid input format. Expected 'Destination:<id>'\n";
        return;
    }

    // Parse max walk time
    pos = maxWalkTimeLine.find(':');
    if (pos != std::string::npos) {
        maxWalkTimeStr = maxWalkTimeLine.substr(pos + 1);
        try {maxWalkTime = std::stoi(maxWalkTimeStr);}
        catch (const std::exception& e) {
            outFile << "Invalid input format. Max Walk Time must be a valid integer.\n";
            return;
        }
    } else {
        outFile << "Invalid input format. Expected 'MaxWalkTime:<int>'\n";
        return;
    }

    // Parse avoid nodes
    pos = avoidNodesLine.find(':');
    if (pos != std::string::npos) avoidNodesStr = avoidNodesLine.substr(pos + 1);
    std::stringstream ss(avoidNodesStr);
    std::string nodeStr;
    while (std::getline(ss, nodeStr, ',')) {
        try {
            int nodeId = std::stoi(nodeStr);
            if (nodeId == startNode || nodeId == endNode) {
                std::cout << "Warning: Avoid nodes cannot be the same as the source or destination nodes.\n";
            }
            else if (auto vertex = urbanGraph.findVertex(nodeId)) {  // Check if vertex is not nullptr
                vertex->setIgnoreFlag(true);
            } else {
                std::cout << "Warning: Node " << nodeId << " not found in graph.\n";
            }
        } catch (const std::exception&) {
            std::cout << "Warning: Invalid node format in AvoidNodes.\n";
        }
    }

    // Parse avoid segments
    pos = avoidSegmentsLine.find(':');
    if (pos != std::string::npos) avoidSegmentsStr = avoidSegmentsLine.substr(pos + 1);
    std::stringstream segSS(avoidSegmentsStr);
    std::string segmentStr;
    while (std::getline(segSS, segmentStr, ')')) {
        size_t openParen = segmentStr.find('(');
        size_t comma = segmentStr.find(',');
        if (openParen != std::string::npos && comma != std::string::npos) {
            try {
                int from = std::stoi(segmentStr.substr(openParen + 1, comma - openParen - 1));
                int to = std::stoi(segmentStr.substr(comma + 1));
                if (auto edge = urbanGraph.findEdge(from, to)) {  // Check if edge is not nullptr
                    edge->setIgnoreFlag(true);
                } else {
                    std::cout << "Warning: Edge (" << from << ", " << to << ") not found in graph.\n";
                }
            } catch (const std::exception&) {
                std::cout << "Warning: Invalid edge format in AvoidSegments.\n";
            }
        }
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

