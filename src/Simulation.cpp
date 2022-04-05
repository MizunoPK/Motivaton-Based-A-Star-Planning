#include "Simulation.h"
#include <fstream>
#include <map>
#include <vector>
#include <tuple>

Simulation::Simulation(std::string nodeFile, std::string adjacenciesFile, std::string agentFile) {
    initializeStateSpace(nodeFile, adjacenciesFile);
    initializeAgent(agentFile);
}

void Simulation::initializeStateSpace(std::string nodeFile, std::string adjacenciesFile) {
    // Process the Node File
    std::fstream nodeFileStream(nodeFile);
    if (nodeFileStream.is_open()) {
        // Make the map of nodes 
        std::map<std::string, Node*> nodeMap;
        std::string fileLine;
        while (fileLine) {
            std::getline(nodeFileStream, fileLine);
            std::cout << fileLine << std::endl;
        }
    }
    else {
        std::cout << "Error: Could not open Node File \"" << nodeFile << "\"" << std::endl;
        exit(-1);
    }
}

void Simulation::initializeAgent(std::string agentFile) {

}

void Simulation::runSearch() {}

void Simulation::outputPath() {}