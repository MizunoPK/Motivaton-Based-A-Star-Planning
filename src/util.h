#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

template <typename Out>
void split(const std::string &s, char delim, Out result);

// * Function: splitString
// Description: Helper function used to split a string 
// Input:
//      str - string being split 
//      delim - delimiter to split on 
// Output: vector<string> - a vector made up of the substrings
std::vector<std::string> split(const std::string &s, char delim);

// * Function: splitIntList
// Description: splits a given string representing a state into a vector of integers
// Input:
//      listStr - The list being split, in string form. Should look like "x,x,x,...,x"
// Output: vector<int> - a vector made up of the ints
std::vector<int> splitIntList(std::string listStr);

// * Function: splitDoubleList
// Description: splits a given string representing a state into a vector of doubles
// Input:
//      listStr - The list being split, in string form. Should look like "x,x,x,...,x"
// Output: vector<double> - a vector made up of the state doubles
std::vector<double> splitDoubleList(std::string listStr);

// Helper Function: getCoordString
// Description: Return the string version of a coordinate
std::string getCoordString(std::vector<int> coord);

// * Debugging Functions:
void printIntVector(std::vector<int> nums);
void printDoubleVector(std::vector<double> nums);

#endif //UTIL_H