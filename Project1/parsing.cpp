//
// Created by Gabriel Sousa on 23/02/2025.
//

#include "parsing.h"


// Convert the Parking field from string to bool.
bool stringToBool(const std::string &s) {
    return s == "1";
}

// Reads Locations.csv and returns a vector of LocationRecord
std::vector<LocationRecord> readLocations(const std::string &filename) {
    std::vector<LocationRecord> records;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        return records;
    }
    std::string line;
    std::getline(file, line); // Skip header line
    while (std::getline(file, line)) {
        //std::cout << "Reading line: " << line << std::endl;
        std::istringstream linestream(line);
        std::string token;
        LocationRecord rec;
        // Field 1: Location (name)
        std::getline(linestream, token, ',');
        rec.location = token;
        // Field 2: Id
        std::getline(linestream, token, ',');
        rec.id = std::stoi(token);
        // Field 3: Code
        std::getline(linestream, token, ',');
        rec.code = token;
        // Field 4: Parking (1 or 0)
        std::getline(linestream, token, ',');
        rec.parking = stringToBool(token);
        records.push_back(rec);
    }
    file.close();
    return records;
}

// Reads Distances.csv and returns a vector of DistanceRecord
std::vector<DistanceRecord> readDistances(const std::string &filename) {
    std::vector<DistanceRecord> records;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        return records;
    }
    std::string line;
    std::getline(file, line); // Skip header line
    while (std::getline(file, line)) {
        //std::cout << "Reading line: " << line << std::endl;
        std::istringstream linestream(line);
        std::string token;
        DistanceRecord rec;
        // Field 1: Location1 (start location code)
        std::getline(linestream, token, ',');
        rec.code1 = token;
        // Field 2: Location2 (end location code)
        std::getline(linestream, token, ',');
        rec.code2 = token;
        // Field 3: Driving (if "X", set drivingTime to -1)
        std::getline(linestream, token, ',');
        if (token == "X")
            rec.drivingTime = -1;  // Used -1 to denote an undrivable segment
        else
            rec.drivingTime = std::stoi(token);
        // Field 4: Walking
        std::getline(linestream, token, ',');
        rec.walkingTime = std::stoi(token);
        records.push_back(rec);
    }
    file.close();
    return records;
}

// Function to load data into the graph
void loadGraph(Graph<int>& graph) {
    std::vector<LocationRecord> locations = readLocations("DataSets/Locations2.csv");
    std::vector<DistanceRecord> distances = readDistances("DataSets/Distances2.csv");

    std::unordered_map<std::string, int> codeToId;


    // Add locations (nodes) to the graph
    for (const auto& loc : locations) {
        graph.addVertex(loc.id);
        codeToId[loc.code] = loc.id;
        if(loc.parking){
            graph.findVertex(loc.id)->setParking(true);
        }
    }

    // Add distances (edges) to the graph
    for (const auto& dist : distances) {
        if (codeToId.find(dist.code1) != codeToId.end() && codeToId.find(dist.code2) != codeToId.end()) {
            int id1 = codeToId[dist.code1];
            int id2 = codeToId[dist.code2];

            graph.addBidirectionalEdge(id1, id2, dist.drivingTime, dist.walkingTime);
        } else {
            std::cerr << "Warning: Skipping edge (" << dist.code1 << ", " << dist.code2 << ") due to missing location.\n";
        }
    }

    std::cout << "Graph successfully loaded with " << locations.size() << " locations and " << distances.size() << " connections.\n";
}
/*2
Debugging purposes
int main() {
    std::cout << "Reading locations...\n";
    auto locations = readLocations("DataSets/Locations.csv");

    if (locations.empty()) {
        std::cerr << "Failed to load locations.\n";
        return 1;
    }

    std::cout << "Reading distances...\n";
    auto distances = readDistances("DataSets/Distances.csv");

    if (distances.empty()) {
        std::cerr << "Failed to load distances.\n";
        return 1;
    }

    std::cout << "All data loaded successfully!\n";
    return 0;
}
*/



int parseSource(const Graph<int>& graph, const std::string& sourceLine) {
    int startNode;
    size_t pos = sourceLine.find(':');
    if (pos != std::string::npos) {
        std::string startNodeStr = sourceLine.substr(pos + 1);
        try {startNode = std::stoi(startNodeStr);}
        catch (const std::exception& e) {
            std::cout << "Invalid node format. Source node must be a valid integer.\n";
            return -1;
        }
        if (graph.findVertex(startNode) == nullptr) {
            std::cout << "The source node " << startNode << " does not exist.\n";
            return -1;
        }
    } else {
        std::cout << "Invalid input format. Expected 'Source:<id>'\n";
        return -1;
    }
    return startNode;
}

int parseDestination(const Graph<int>& graph, const std::string& destinationLine) {
    int endNode;
    size_t pos = destinationLine.find(':');
    if (pos != std::string::npos) {
        std::string endNodeStr = destinationLine.substr(pos + 1);
        try {endNode = std::stoi(endNodeStr);}
        catch (const std::exception& e) {
            std::cout << "Invalid node format. Destination node must be a valid integer.\n";
            return -1;
        }
        if (graph.findVertex(endNode) == nullptr) {
            std::cout << "The destination node " << endNode << " does not exist.\n";
            return -1;
        }
    } else {
        std::cout << "Invalid input format. Expected 'Destination:<id>'\n";
        return -1;
    }
    return endNode;
}

int parseIncludeNode(const Graph<int>& graph, const std::string& includeNodeLine) {
    int includeNode;
    size_t pos = includeNodeLine.find(':');
    if (pos != std::string::npos) {
        std::string includeNodeStr = includeNodeLine.substr(pos + 1);
        if (!includeNodeStr.empty()) {
            try {includeNode = std::stoi(includeNodeStr);}
            catch (const std::exception& e) {
                std::cout << "Warning: Invalid node format. Include node must be a valid integer. Ignoring include node.\n";
                return -1;
            }
            if (graph.findVertex(includeNode) == nullptr) {
                std::cout << "Warning: The include node " << includeNode << " does not exist. Ignoring include node.\n";
                return -1;
            }
        }
    } else {
        std::cout << "Invalid input format. Expected 'IncludeNode:<id>'\n";
        return -1;
    }
    return includeNode;
}

void parseAvoidNodes(const Graph<int>& graph, const std::string& avoidNodesLine, const int startNode, const int endNode) {
    std::string avoidNodesStr;
    size_t pos = avoidNodesLine.find(':');
    if (pos != std::string::npos) avoidNodesStr = avoidNodesLine.substr(pos + 1);
    std::stringstream ss(avoidNodesStr);
    std::string nodeStr;
    while (std::getline(ss, nodeStr, ',')) {
        try {
            int nodeId = std::stoi(nodeStr);
            if (nodeId == startNode || nodeId == endNode) {
                std::cout << "Warning: Avoid nodes cannot be the same as the source or destination nodes.\n";
            }
            else if (auto vertex = graph.findVertex(nodeId)) {  // Check if vertex is not nullptr
                vertex->setIgnoreFlag(true);
            } else {
                std::cout << "Warning: Node " << nodeId << " not found in graph.\n";
            }
        } catch (const std::exception&) {
            std::cout << "Warning: Invalid node format in AvoidNodes.\n";
        }
    }
}

void parseAvoidSegments(const Graph<int>& graph, const std::string& avoidSegmentsLine) {
    std::string avoidSegmentsStr;
    size_t pos = avoidSegmentsLine.find(':');
    if (pos != std::string::npos) avoidSegmentsStr = avoidSegmentsLine.substr(pos + 1);
    std::stringstream segSS(avoidSegmentsStr);
    std::string segmentStr;
    while (std::getline(segSS, segmentStr, ')')) {
        size_t openParen = segmentStr.find('(');
        size_t comma = segmentStr.find(',', 1);
        if (openParen != std::string::npos && comma != std::string::npos) {
            try {
                int from = std::stoi(segmentStr.substr(openParen + 1, comma - openParen - 1));
                int to = std::stoi(segmentStr.substr(comma + 1));
                if (auto edge = graph.findEdge(from, to)) {  // Check if edge is not nullptr
                    edge->setIgnoreFlag(true);
                } else {
                    std::cout << "Warning: Edge (" << from << ", " << to << ") not found in graph.\n";
                }
            } catch (const std::exception&) {
                std::cout << "Warning: Invalid edge format in AvoidSegments.\n";
            }
        }
    }
}

int parseMaxWalkTime(const Graph<int>& graph, const std::string& maxWalkTimeLine) {
    int maxWalkTime;
    size_t pos = maxWalkTimeLine.find(':');
    if (pos != std::string::npos) {
        std::string maxWalkTimeStr = maxWalkTimeLine.substr(pos + 1);
        try {maxWalkTime = std::stoi(maxWalkTimeStr);}
        catch (const std::exception& e) {
            std::cout << "Invalid input format. Max Walk Time must be a valid integer. Ignoring Max Walk Time.\n";
            return INT_MAX;
        }
        if (maxWalkTime <= 0) {
            std::cout << "Invalid input format. Max Walk Time must be a valid integer. Ignoring Max Walk Time.\n";
            return INT_MAX;
        }
    } else {
        std::cout << "Invalid input format. Expected 'MaxWalkTime:<int>'\n";
        return INT_MAX;
    }
    return maxWalkTime;
}