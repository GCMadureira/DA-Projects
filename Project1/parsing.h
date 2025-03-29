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
#include "data_structures/Graph.h"


/**
 * @brief Structure representing a record from Locations.csv
 *
 * Stores the location name, id, code and parking status of a node.
 */
struct LocationRecord {
    std::string location; ///< Name of location
    int id;               ///< Unique ID
    std::string code;     ///< Simple code (key for lookup)
    bool parking;         ///< 1 indicates parking exists; 0 otherwise
};


/**
 * @brief Structure representing a record from Distances.csv
 *
 * Stores the code of the nodes connected by the segment, as well as its driving and walking time.
 */
struct DistanceRecord {
    std::string code1;    ///< Code of the start location
    std::string code2;    ///< Code of the end location
    int drivingTime;      ///< Driving time in minutes (or -1 if not drivable)
    int walkingTime;      ///< Walking time in minutes
};



// Function declarations

/**
 * @brief Convert the Parking field from string to bool.
 *
 * @param s - input string reference
 *
 * @return true if the string is "1" or false otherwise
 */
bool stringToBool(const std::string &s);

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
std::vector<LocationRecord> readLocations(const std::string &filename);

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
std::vector<DistanceRecord> readDistances(const std::string &filename);

/**
 * @brief Loads data into a graph
 *
 * Calls readLocations() and readDistances() to read from the input files Locations.csv and Distances.csv respectively. <br>
 * Then loads the data read into the input graph, skipping invalid edges.
 *
 * @param graph - reference to a graph object where the data will be loaded
 */
void loadGraph(Graph<int>& graph);

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
int parseSource(const Graph<int>& graph, const std::string& sourceLine);

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
int parseDestination(const Graph<int>& graph, const std::string& destinationLine);

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
int parseIncludeNode(const Graph<int>& graph, const std::string& includeNodeLine);

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

void parseAvoidNodes(const Graph<int>& graph, const std::string& avoidNodesLine, int startNode, int endNode);

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
void parseAvoidSegments(const Graph<int>& graph, const std::string& avoidSegmentsLine);

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
int parseMaxWalkTime(const Graph<int>& graph, const std::string& maxWalkTimeLine);

#endif //URBANMAPPROJECT_PARSING_H
