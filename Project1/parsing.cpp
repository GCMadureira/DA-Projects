//
// Created by Gabriel Sousa on 23/02/2025.
//

#include "parsing.h"


/**
 * @brief Convert the Parking field from string to bool.
 *
 * @param s - input string reference
 *
 * @return true if the string is "1" or false otherwise
 */
bool stringToBool(const std::string &s) {
    return s == "1";
}

/**
 * @brief Reads and parses an input file and returns a vector of struct LocationRecord.
 *
 * If the input file cannot be opened, exits with an error to std::cerr. <br>
 * Input file must have the format <Location>,<Id>,<Code>,<Parking> and only one entry per line.
 *
 * @param filename - string reference containing the name of the input file
 *
 * @return vector of struct LocationRecord read from input file or empty vector if error opening file
 */
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

/**
 * @brief Reads and parses an input file and returns a vector of struct DistanceRecord.
 *
 * If the input file cannot be opened, exits with an error to std::cerr. <br>
 * Input file must have the format <Location1>,<Location2>,<Driving>,<Walking> and only one entry per line.
 *
 * @param filename - string reference containing the name of the input file
 *
 * @return vector of struct DistanceRecord read from input file or empty vector if error opening file
 */
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

/**
 * @brief Loads data into a graph
 *
 * Calls readLocations() and readDistances() to read from the input files Locations.csv and Distances.csv respectively. <br>
 * Then loads the data read into the input graph, skipping invalid edges.
 *
 * @param graph - reference to a graph object where the data will be loaded
 */
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


/**
 * @brief Parses source node from string sourceLine
 *
 * Parses the sourceLine with the format Source:<id>. <br>
 * If the node has an invalid format or does not exist in the graph it prints an error to the terminal.
 *
 * @param graph - reference to a graph object containing the start node
 * @param sourceLine - string reference to the source line with format Source:<id>
 *
 * @return start node parsed from sourceLine or -1 in case of error
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

/**
 * @brief Parses end node from string destinationLine
 *
 * Parses the destinationLine with the format Destination:<id>. <br>
 * If the node has an invalid format or does not exist in the graph it prints an error to the terminal.
 *
 * @param graph - reference to a graph object containing the end node
 * @param destinationLine - string reference to the destination line with format Destination:<id>
 *
 * @return end node parsed from destinationLine or -1 in case of error
 */
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

/**
 * @brief Parses include node from string includeNodeLine
 *
 * Parses the includeNodeLine with the format IncludeNode:<id>. <br>
 * If the node has an invalid format or does not exist in the graph it prints a warning to the terminal.
 *
 * @param graph - reference to a graph object containing the include node
 * @param includeNodeLine - string reference to the include node line with format IncludeNode:<id>
 *
 * @return include node parsed from includeNodeLine or -1 in case of error
 */
int parseIncludeNode(const Graph<int>& graph, const std::string& includeNodeLine) {
    int includeNode = -1;
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
        std::cout << "Warning: Invalid input format. Expected 'IncludeNode:<id>'\n";
        return -1;
    }
    return includeNode;
}

/**
 * @brief Parses and processes the nodes to avoid from the string avoidNodesLine
 *
 * Parses the avoidNodesLine with the format AvoidNodes:<id>,<id>,.... <br>
 * If any node has an invalid format, is the same as the startNode or endNode or does not exist in the graph it prints a warning to the terminal. <br>
 * If a node is valid it sets its ignore flag to true.
 *
 * @param graph - reference to a graph object containing the nodes to avoid
 * @param avoidNodesLine - string reference to the avoid nodes line with format AvoidNodes:<id>,<id>,...
 * @param startNode - node from which the route to be processed will start
 * @param endNode - node where the route to be processed will end
 */
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

/**
 * @brief Parses and processes the segments to avoid from the string avoidSegmentsLine
 *
 * Parses the avoidSegmentsLine with the format AvoidSegments:(id,id),(id,id),.... <br>
 * If any segment has an invalid format or does not exist in the graph it prints a warning to the terminal. <br>
 * If a segment is valid it sets its ignore flag to true.
 *
 * @param graph - reference to a graph object containing the segments to avoid
 * @param avoidSegmentsLine - string reference to the avoid segments line with format AvoidSegments:(id,id),(id,id),...
 */
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
        else std::cout << "Warning: Invalid edge format in AvoidSegments.\n";
    }
}

/**
 * @brief Parses the max walk time from string maxWalkTimeLine
 *
 * Parses the maxWalkTime with the format MaxWalkTime:<int>. <br>
 * If the value has an invalid format or is negative it prints a warning to the terminal.
 *
 * @param graph - reference to a graph object containing the include node
 * @param maxWalkTimeLine - string reference to the max walk time line with format MaxWalkTime:<int>
 *
 * @return Max Walk Time parsed from maxWalkTimeLine or INT_MAX in case of error
 */
int parseMaxWalkTime(const Graph<int>& graph, const std::string& maxWalkTimeLine) {
    int maxWalkTime;
    size_t pos = maxWalkTimeLine.find(':');
    if (pos != std::string::npos) {
        std::string maxWalkTimeStr = maxWalkTimeLine.substr(pos + 1);
        try {maxWalkTime = std::stoi(maxWalkTimeStr);}
        catch (const std::exception& e) {
            std::cout << "Warning: Invalid input format. Max Walk Time must be a valid integer. Ignoring Max Walk Time.\n";
            return INT_MAX;
        }
        if (maxWalkTime <= 0) {
            std::cout << "Warning: Invalid input format. Max Walk Time must be a valid integer. Ignoring Max Walk Time.\n";
            return INT_MAX;
        }
    } else {
        std::cout << "Warning: Invalid input format. Expected 'MaxWalkTime:<int>'\n";
        return INT_MAX;
    }
    return maxWalkTime;
}