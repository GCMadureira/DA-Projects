//
// Created by Gabriel Sousa on 24/02/2025.
//

#ifndef URBANMAPPROJECT_PARSING_H
#define URBANMAPPROJECT_PARSING_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include "Graph.h"

// Structure representing a record from Locations.csv
struct LocationRecord {
    std::string location; // Name of location
    int id;               // Unique ID
    std::string code;     // Simple code (key for lookup)
    bool parking;         // 1 indicates parking exists; 0 otherwise
};

// Structure representing a record from Distances.csv
struct DistanceRecord {
    std::string code1;    // Code of the start location
    std::string code2;    // Code of the end location
    int drivingTime;      // Driving time in minutes (or -1 if not drivable)
    int walkingTime;      // Walking time in minutes
};

// Function declarations
bool stringToBool(const std::string &s);
std::vector<LocationRecord> readLocations(const std::string &filename);
std::vector<DistanceRecord> readDistances(const std::string &filename);


#endif //URBANMAPPROJECT_PARSING_H
