#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

template <typename Out>
void split(const std::string &s, char delim, Out result);

// Function: splitString
// Input:
//      str - string being split 
//      delim - delimiter to split on 
// Output: vector<string> - a vector made up of the substrings
// Description: Helper function used to split a string 
std::vector<std::string> split(const std::string &s, char delim);

#endif //UTIL_H