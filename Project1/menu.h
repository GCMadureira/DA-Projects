#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <fstream>
#include <limits>
#include <regex>
#include "data_structures/Graph.h"
#include "data_structures/MutablePriorityQueue.h"
#include "parsing.cpp"
#include "search.cpp"
#include "parsing.h"
#include "batchmode.cpp"
#include "search.h"
#include <climits>

/**
 * @brief Writes inputs to input.txt
 *
 * Writes the parameters received on the call to the input.txt file depending on the choice value.
 * Uses the format specified on the DA project description document. Unused parameters may contain invalid values.
 *
 * @param choice - represents the route type chosen and dictates which parameters will be written to the file
 * @param source - integer representing the id of the source node
 * @param destination - integer representing the id of the destination node
 * @param maxWalkTime - integer containing the maximum allowed time to spend walking
 * @param avoidNodes - string containing the nodes to avoid with the format AvoidNodes:<id>,<id>,...
 * @param avoidSegments - string containing the segments to avoid with the format AvoidSegments:(id,id),(id,id),...
 * @param includeNode - integer representing the id of the include node
 */
void writeInputToFile(int choice, int source, int destination, int maxWalkTime,
                      const std::string& avoidNodes, const std::string& avoidSegments, int includeNode);


/**
 * @brief Gets a valid positive integer
 *
 * Asks for input on the terminal until it receives valid input, that is, a valid positive integer.
 *
 * @param prompt - string of text that will be printed on the terminal to ask for input
 *
 * @return valid integer value that was received from the terminal
 */
int getValidInt(const std::string& prompt);

/**
 * @brief Gets a valid list of nodes/segments
 *
 * Asks for input on the terminal until it receives valid input matching the string pattern using regex. <br>
 * Allows the input to be emtpy as well.
 *
 * @param prompt - string of text that will be printed on the terminal to ask for input
 * @param pattern - string containing a regex expression used to check if the input is valid
 *
 * @return string with the valid input (the empty string is always also valid input)
 */
std::string getValidList(const std::string& prompt, const std::string& pattern);

/**
 * @brief Receives route parameters depending on the route plan
 *
 * Calls getValidInt() and getValidList() to receive the input parameters used to execute the algorithm.
 * It always receives the source and destination nodes independent of the route plan chosen, however all the other parameters are choice dependent. <br>
 * At the end it calls writeInputToFile() passing the parameters received to save them on an input file that can be processed.
 *
 * @param choice - represents the route type chosen and dictates which parameters will be received
 */
void receiveInput(int choice);

/**
 * @brief Receives a choice from the terminal to select the mode (manual or batch)
 *
 * Asks for terminal input until it receives a valid one.
 * - if 1 is received then call receiveInput() to ask for the route parameters
 * - if 2 is received do nothing since batch mode uses the inputs from a file
 * - if 3 is received then return 1 to signal the calling function to not proceed with the algorithm, allowing choice changing
 *
 * @param choice - represents the route type chosen and is passed to receiveInput()
 *
 * @return 0 to proceed with the algorithm execution and 1 otherwise
 */
int selectMode(int choice);

#endif //MENU_H
