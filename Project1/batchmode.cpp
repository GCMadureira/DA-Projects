//
// Created by Gabriel Sousa on 19/03/2025.
//


#include "parsing.h"

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
        return;
    }


    // Setup
    urbanGraph.setInitialState();

    // Read expected lines
    std::string modeLine, sourceLine, destinationLine;
    std::getline(inFile, modeLine);
    std::getline(inFile, sourceLine);
    std::getline(inFile, destinationLine);

    // Parse source
    int startNode = parseSource(urbanGraph, sourceLine);
    if (startNode == -1) return;

    // Parse destination
    int endNode = parseDestination(urbanGraph, destinationLine);
    if (endNode == -1) return;


    // Computations
    double firstPathLength = 0, secondPathLength = 0;
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
    urbanGraph.setInitialState();

    // Read expected lines
    std::string modeLine, sourceLine, destinationLine, avoidNodesLine, avoidSegmentsLine, includeNodeLine;
    std::getline(inFile, modeLine);
    std::getline(inFile, sourceLine);
    std::getline(inFile, destinationLine);
    std::getline(inFile, avoidNodesLine);
    std::getline(inFile, avoidSegmentsLine);
    std::getline(inFile, includeNodeLine);

    // Parse source
    int startNode = parseSource(urbanGraph, sourceLine);
    if (startNode == -1) return;

    // Parse destination
    int endNode = parseDestination(urbanGraph, destinationLine);
    if (endNode == -1) return;

    // Parse include node
    int includeNode = parseIncludeNode(urbanGraph, includeNodeLine);

    // Parse avoid nodes and segments
    parseAvoidNodes(urbanGraph, avoidNodesLine, startNode, endNode);
    parseAvoidSegments(urbanGraph, avoidSegmentsLine);


    // Computations
    std::vector<int> firstPath, secondPath;
    Vertex<int>* endVertex = urbanGraph.findVertex(endNode);
    Vertex<int>* includeVertex = urbanGraph.findVertex(includeNode);
    double firstPathLength = 0, secondPathLength = 0;

    dijkstra(&urbanGraph, startNode);
    if (includeNode == -1 || startNode == includeNode || endNode == includeNode) {
        // No IncludeNode or it's the same as startNode/endNode
        firstPathLength = endVertex->getDist();
        firstPath = getPath(&urbanGraph, startNode, endNode);

        if (firstPath.empty()) {
            std::cout << "Error: No path found from startNode " << startNode << " to endNode " << endNode << ".\n";
            outFile << "Source: " << startNode << "\n"
                    << "Destination: " << endNode << "\n"
                    << "RestrictedDrivingRoute: none\n";
            return;
        }
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
    std::getline(inFile, modeLine);
    std::getline(inFile, sourceLine);
    std::getline(inFile, destinationLine);
    std::getline(inFile, maxWalkTimeLine);
    std::getline(inFile, avoidNodesLine);
    std::getline(inFile, avoidSegmentsLine);


    // Parse source
    int startNode = parseSource(urbanGraph, sourceLine);
    if (startNode == -1) return;

    // Parse destination
    int endNode = parseDestination(urbanGraph, destinationLine);
    if (endNode == -1) return;

    // Parse max walk time
    int maxWalkTime = parseMaxWalkTime(urbanGraph, maxWalkTimeLine);

    // Parse avoid nodes and segments
    parseAvoidNodes(urbanGraph, avoidNodesLine, startNode, endNode);
    parseAvoidSegments(urbanGraph, avoidSegmentsLine);

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


    // Computations
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

