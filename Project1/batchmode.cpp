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

    // Parse avoidNodes
    pos = avoidNodesLine.find(':');
    if (pos != std::string::npos) {
        avoidNodesStr = avoidNodesLine.substr(pos + 1);
    }
    std::istringstream avoidNodesStream(avoidNodesStr);
    std::string node;
    while (std::getline(avoidNodesStream, node, ',') || std::getline(avoidNodesStream, node)) { // if the first condition is false then check for the last node without the comma
        try {
            int nodeValue = std::stoi(node);
            // Avoid nodes cannot coincide with the start, end or include nodes
            if (nodeValue == startNode || nodeValue == endNode || nodeValue == includeNode) continue;
            avoidNodes.push_back(nodeValue);
        }
        catch (const std::exception& e) {
            outFile << "Invalid node format. Avoid nodes must be a valid integers.\n";
        }
    }

    //Parse avoidSegments
    pos = avoidSegmentsLine.find(':');
    if (pos != std::string::npos) {
        avoidSegmentsStr = avoidSegmentsLine.substr(pos + 1);
    }
    std::istringstream avoidSegmentsStream(avoidSegmentsStr);
    std::string segment;
    while (std::getline(avoidSegmentsStream, segment, ')') || std::getline(avoidSegmentsStream, segment)) { // if the first condition is false then check for the last segment without the comma
        int u, v;
        if (sscanf(segment.c_str(), "(%d,%d)", &u, &v) == 2) { // first segment
            avoidSegments.emplace_back(u, v);
        }
        else if (sscanf(segment.c_str(), ",(%d,%d)", &u, &v) == 2) { // other segments
            avoidSegments.emplace_back(u, v);
        }
        else {
            outFile << "Invalid segment format. Avoid segments must be in (a,b) format.\n";
        }
    }

    //Parse includeNode
    pos = includeNodeLine.find(':');
    if (pos != std::string::npos) {
        includeNodeStr = includeNodeLine.substr(pos + 1);
        if (!includeNodeStr.empty()) { // Avoid the error message when there is no include node
            try {includeNode = std::stoi(includeNodeStr);}
            catch (const std::exception& e) {
                outFile << "Invalid node format. Include node must be a valid integer.\n";
            }
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
        std::cout << "Could not open input.txt. Batch mode aborted.\n";
        return;
    }

    std::ofstream outFile("inputs/output.txt");
    if (!outFile.is_open()) {
        std::cout << "Could not open output.txt. Batch mode aborted.\n";
        return;
    }

    // Setup
    for (auto v : urbanGraph.getVertexSet()) {
        v->setIgnoreFlag(false);
        for (auto e : v->getAdj()) e->setIgnoreFlag(false);
    }
    int startNode, endNode, includeNode = -1;
    int mode = parseInputFile(inFile, outFile, urbanGraph, startNode, endNode, includeNode);
    if (mode == -1) return; // error


    // Computations
    int firstPathLength = 0, secondPathLength = 0;
    std::vector<int> firstPath, secondPath;
    dijkstra(&urbanGraph, startNode);
    if (mode == 0) { // Driving and Alternate Driving Route
        firstPath = getPath(&urbanGraph, startNode, endNode, firstPathLength);
        // Ignore all used Nodes to compute the Alternate Driving Route (except start and end nodes)
        for (int i = 1; i < firstPath.size() - 1; ++i) {
            auto vertex = urbanGraph.findVertex(firstPath[i]);
            vertex->setIgnoreFlag(true);
        }
        dijkstra(&urbanGraph, startNode);
        secondPath = getPath(&urbanGraph, startNode, endNode, secondPathLength);
    }
    else if (mode == 1) { // Restricted Route
        if (includeNode == -1) firstPath = getPath(&urbanGraph, startNode, endNode, firstPathLength);
        else { // Two-step path calculation via includeNode
            firstPath = getPath(&urbanGraph, startNode, includeNode, firstPathLength);
            dijkstra(&urbanGraph, includeNode);
            secondPath = getPath(&urbanGraph, includeNode, endNode, secondPathLength);
        }
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
        if (firstPath.empty()) {
            outFile << "none\n";
        } else {
            outFile << firstPath[0];
            for (size_t i = 1; i < firstPath.size(); ++i) {
                outFile << ", " << firstPath[i];
            }
            if (includeNode != -1 && !secondPath.empty()) { // Has include node and a valid route through it
                for (size_t i = 1; i < secondPath.size(); ++i) {
                    outFile << ", " << secondPath[i];
                }
                outFile << " (" << (firstPathLength + secondPathLength) << ")\n\n";
            }
            else outFile << " (" << firstPathLength << ")\n\n";
        }
    }
    outFile << "Batch processing completed successfully.\n";
}

void restrictedRoutePlanning(Graph<int>& urbanGraph) {
    std::ifstream inFile("inputs/input.txt");
    if (!inFile.is_open()) {
        std::cout << "Could not open input.txt. Batch mode aborted.\n";
        return;
    }

    std::ofstream outFile("inputs/output.txt");
    if (!outFile.is_open()) {
        std::cout << "Could not open output.txt. Batch mode aborted.\n";
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

    std::vector<int> path1, path2;
    int pathLength1 = 0, pathLength2 = 0;

    if (includeNode != -1) {
        auto vertex = urbanGraph.findVertex(includeNode);
        if (vertex) vertex->setIgnoreFlag(false);
    }

    if (includeNode == -1 || startNode == includeNode) {
        // No IncludeNode or it's the same as startNode to Direct Route
        std::cout << "Debug: No IncludeNode. Finding direct path.\n";
        dijkstra(&urbanGraph, startNode);
        path1 = getPath(&urbanGraph, startNode, endNode, pathLength1);
    } else {
        // Path from startNode to IncludeNode
        dijkstra(&urbanGraph, startNode);
        path1 = getPath(&urbanGraph, startNode, includeNode, pathLength1);
        if (path1.empty()) {
            std::cout << "Error: No path found from " << startNode << " to IncludeNode " << includeNode << ".\n";
            outFile << "Source: " << startNode << "\n"
                    << "Destination: " << endNode << "\n"
                    << "RestrictedDrivingRoute: none\n";
            return;
        }

        // Path from IncludeNode to endNode
        dijkstra(&urbanGraph, includeNode);
        path2 = getPath(&urbanGraph, includeNode, endNode, pathLength2);
        if (path2.empty()) {
            outFile << "Source: " << startNode << "\n"
                    << "Destination: " << endNode << "\n"
                    << "RestrictedDrivingRoute: none\n";
            return;
        }
    }

// Merge paths if IncludeNode exists
    std::vector<int> finalPath = path1;
    if (!path2.empty()) {
        finalPath.insert(finalPath.end(), path2.begin() + 1, path2.end());
    }
    if (finalPath.empty()) {
        outFile << "Source: " << startNode << "\n"
                << "Destination: " << endNode << "\n"
                << "RestrictedDrivingRoute: none\n";
        return;
    }

    outFile << "Source: " << startNode << "\n"
            << "Destination: " << endNode << "\n"
            << "RestrictedDrivingRoute: " << finalPath[0];
    for (int i = 1; i < finalPath.size(); i++) {
        outFile << ", " << finalPath[i];
    }
    outFile << " (" << pathLength1 + pathLength2 << ")\n";

    outFile << "Batch processing completed successfully.\n";
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
    /*
    if (urbanGraph.areAdjacent(startNode, endNode)) {
        outFile << "Source: " << startNode << "\nDestination: " << endNode << "\n";
        outFile << "DrivingRoute:none\nParkingNode:none\nWalkingRoute:none\n";
        outFile << "Message: Origin and destination are adjacent, violating constraints.\n";
        return;
    }*/
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

    // Find all possible parking nodes
    std::vector<int> parkingNodes = getParkingNodes(&urbanGraph);
    std::cout << "Found " << parkingNodes.size() << " parking nodes.\n";

    std::vector<int> bestDrivingPath, bestWalkingPath;
    int bestTotalTime = INF;
    int bestParkingNode = -1;


    for (int parkingNode : parkingNodes) {
        if (urbanGraph.findVertex(parkingNode)->isIgnored()) continue;

        cout<<"Processing Parking Node:"<<parkingNode<< "\n";
        // Step 1: Find best driving path to parking node
        dijkstra(&urbanGraph, startNode);
        int drivingTime=0;
        std::vector<int> drivingPath = getPath(&urbanGraph, startNode, parkingNode, drivingTime);
        if (drivingPath.empty()) {
            continue;
        }

        // Step 2: Find best walking path from parking node to destination
        dijkstra(&urbanGraph, parkingNode);
        int walkingTime = 0;
        std::vector<int> walkingPath = getPath(&urbanGraph, parkingNode, endNode, walkingTime);

        if (walkingPath.empty() || walkingTime > maxWalkTime) {
            continue;
        }

        cout<<"Driving Time:"<<drivingTime<<"\n"<<"WalkingTime:"<< walkingTime<<"\n";
        // Step 3: Check for optimality
        int totalTime = drivingTime + walkingTime;
        if (totalTime < bestTotalTime || (totalTime == bestTotalTime && walkingTime > bestWalkingPath.size())) {
            bestTotalTime = totalTime;
            bestDrivingPath = drivingPath;
            bestWalkingPath = walkingPath;
            bestParkingNode = parkingNode;
            cout<<"BestTotalTime:"<<totalTime<<"\n";
        }
    }

    // Output results
    outFile << "Source: " << startNode << "\nDestination: " << endNode << "\n";
    if (bestParkingNode == -1) {
        outFile << "DrivingRoute:none\nParkingNode:none\nWalkingRoute:none\n";
        outFile << "Message: No valid route found. Possible reasons: no suitable parking or max walking time exceeded.\n";
    } else {
        outFile << "DrivingRoute:";
        for (size_t i = 0; i < bestDrivingPath.size(); i++) {
            if (i > 0) outFile << ", ";
            outFile << bestDrivingPath[i];
        }
        outFile << " (" << bestTotalTime - bestWalkingPath.size() << ")\n";

        outFile << "ParkingNode:" << bestParkingNode << "\n";

        outFile << "WalkingRoute:";
        for (size_t i = 0; i < bestWalkingPath.size(); i++) {
            if (i > 0) outFile << ", ";
            outFile << bestWalkingPath[i];
        }
        outFile << " (" << bestWalkingPath.size() << ")\n";

        outFile << "TotalTime:" << bestTotalTime << "\n";
    }
}

