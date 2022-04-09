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

// * Function: splitStateList
// Description: splits a given string representing a state into a vector of integers
// Input:
//      listStr - The list being split, in string form. Should look like "x,x,x,...,x"
// Output: vector<int> - a vector made up of the state ints
std::vector<int> splitStateList(std::string listStr);

// * Debugging Functions:
void printStateVector(std::vector<int> states);

#endif //UTIL_H