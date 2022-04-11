#include "includes.h"
#include "util.h"
#include <sstream>
#include <iterator>
#include <iostream>

template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

std::vector<int> splitIntList(std::string listStr) {
    std::vector<std::string> stringSplit = split(listStr, ',');
    std::vector<int> intSplit;
    for (int i=0; i < stringSplit.size(); i++) {
        int num = std::stoi(stringSplit.at(i));
        intSplit.push_back(num);
    }
    return intSplit;
}

std::string getCoordString(std::vector<int> coord) {
    return std::to_string(coord.at(0)) + "," + std::to_string(coord.at(1));
}

// * Debugging Functions:
void printIntVector(std::vector<int> nums) {
    std::cout << "[";
    for ( int i=0; i < nums.size(); i++ ) {
        std::cout << nums.at(i);
        if ( i < nums.size() - 1 ) {
            std::cout << ", ";
        }
    }
    std::cout << "]";
}