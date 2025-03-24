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
    std::vector<LocationRecord> locations = readLocations("DataSets/Locations.csv");
    std::vector<DistanceRecord> distances = readDistances("DataSets/Distances.csv");

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

