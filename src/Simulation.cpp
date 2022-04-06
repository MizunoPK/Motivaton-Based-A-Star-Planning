#include "Simulation.h"
#include "util.h"
#include <fstream>
#include <map>
#include <vector>

Simulation::Simulation(std::string nodeFile, std::string adjacenciesFile, std::string agentFile) {
    initializeStateSpace(nodeFile, adjacenciesFile);
    initializeAgent(agentFile);
}

void Simulation::initializeStateSpace(std::string nodeFile, std::string adjacenciesFile) {
    this->ss = new StateSpace();
    this->initStateSpaceNodes(nodeFile);
    this->initStateSpaceAdjs(adjacenciesFile);

    this->ss->printNodes();
    this->ss->printAdjacencies();
}

void Simulation::initStateSpaceNodes(std::string nodeFile) {
    // Process the Node File
    std::fstream nodeFileStream(nodeFile);
    if (nodeFileStream.is_open()) {
        // Make the map of nodes 
        std::map<std::string, Node*> nodeMap;
        std::string fileLine;
        while (nodeFileStream) {
            std::getline(nodeFileStream, fileLine);
            std::vector<std::string> lineVector = split(fileLine, ' ');

            // Get node name
            std::string nodeName = lineVector.at(0);
            
            // Get node internal state
            std::vector<std::string> stateStrings = split(lineVector.at(1), ',');
            std::vector<int> state;
            for (int i=0; i < stateStrings.size(); i++) {
                int num = stoi(stateStrings.at(i));
                state.push_back(num);
            }

            // Get node agent-modifiers
            std::vector<std::string> modifierStrings = split(lineVector.at(2), ',');
            std::vector<int> modifiers;
            for (int i=0; i < modifierStrings.size(); i++) {
                int num = std::stoi(modifierStrings.at(i));
                modifiers.push_back(num);
            }

            nodeMap[lineVector.at(0)] = new Node(nodeName, state, modifiers);
        }
        ss->initNodes(nodeMap);
    }
    else {
        std::cout << "Error: Could not open Node File \"" << nodeFile << "\"" << std::endl;
        exit(-1);
    }
}

void Simulation::initStateSpaceAdjs(std::string adjacenciesFile) {
    // Process the Adjacencies File
    std::fstream adjFileStream(adjacenciesFile);
    if (adjFileStream.is_open()) {
        // Make the map of nodes 
        std::map<Node*, std::vector<Adjacency*>*> adjMap;
        std::string fileLine;
        while (adjFileStream) {
            std::getline(adjFileStream, fileLine);
            std::vector<std::string> lineVector = split(fileLine, ' ');

            // Get the node this line is defining the adjacency list for
            Node* baseNode = this->ss->getNode(lineVector.at(0));

            // Parse the adjacency list
            std::vector<Adjacency*>* adjacencyList = new std::vector<Adjacency*>;
            if ( lineVector.at(1) != "NULL" ) {
                std::vector<std::string> adjString = split(lineVector.at(1), ',');
                for ( int i=0; i < adjString.size(); i++ ) {
                    std::vector<std::string> adjacency = split(adjString.at(i), '-');

                    Node* adjacentNode = this->ss->getNode(adjacency.at(0));
                    int weight = std::stoi(adjacency.at(1));
                    Adjacency *adjacencyTuple = new Adjacency {adjacentNode, weight};

                    adjacencyList->push_back(adjacencyTuple);
                }
            }

            // Add to the map
            adjMap[baseNode] = adjacencyList;
        }
        ss->initAdjacencies(adjMap);
    }
    else {
        std::cout << "Error: Could not open Adjacencies File \"" << adjacenciesFile << "\"" << std::endl;
        exit(-1);
    }
}

void Simulation::initializeAgent(std::string agentFile) {

}

void Simulation::runSearch() {}

void Simulation::outputPath() {}