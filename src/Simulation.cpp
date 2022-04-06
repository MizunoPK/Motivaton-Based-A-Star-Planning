#include "Simulation.h"
#include "util.h"
#include <fstream>
#include <map>
#include <vector>

Simulation::Simulation(std::string nodeFile, std::string adjacenciesFile, std::string agentFile) {
    initializeStateSpace(nodeFile, adjacenciesFile);
    initializeAgent(agentFile);

    // Debugging Prints
    this->ss->printNodes();
    this->ss->printAdjacencies();
    this->agent->printAgentInfo();
}

void Simulation::initializeStateSpace(std::string nodeFile, std::string adjacenciesFile) {
    this->ss = new StateSpace();
    this->initStateSpaceNodes(nodeFile);
    this->initStateSpaceAdjs(adjacenciesFile);
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
            std::vector<int> state = splitStateList(lineVector.at(1));

            // Get node agent-modifiers
            std::vector<int> modifiers = splitStateList(lineVector.at(2));

            nodeMap[lineVector.at(0)] = new Node(nodeName, state, modifiers);
        }
        ss->initNodes(nodeMap);
        nodeFileStream.close();
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
        adjFileStream.close();
    }
    else {
        std::cout << "Error: Could not open Adjacencies File \"" << adjacenciesFile << "\"" << std::endl;
        exit(-1);
    }
}

void Simulation::initializeAgent(std::string agentFile) {
    // Process the Agent File
    std::fstream agentFileStream(agentFile);
    if (agentFileStream.is_open()) {
        std::string fileLine;
        // The agent file should always have 4 lines of relevent data
        // First line is actor state:
        std::getline(agentFileStream, fileLine);
        std::vector<int> actorState = splitStateList(fileLine);

        // Second Line is Starting Node:
        std::getline(agentFileStream, fileLine);
        Node* startingNode = this->ss->getNode(fileLine);

        // Third Line is Primary Goal:
        std::getline(agentFileStream, fileLine);
        Node* primaryGoal = this->ss->getNode(fileLine);

        // Fourth Line is Secondary Goals:
        std::getline(agentFileStream, fileLine);
        std::vector<Node*> secondaryGoals;
        if (fileLine != "NULL") {
            std::vector<std::string> nodeStrs = split(fileLine, ',');
            for ( int i=0; i < nodeStrs.size(); i++ ) {
                Node* goal = this->ss->getNode(nodeStrs.at(i));
                secondaryGoals.push_back(goal);
            }
        }
        
        // Make the agent
        agent = new Agent(actorState, startingNode, primaryGoal, secondaryGoals);
        agentFileStream.close();
    }
    else {
        std::cout << "Error: Could not open Agent File \"" << agentFile << "\"" << std::endl;
        exit(-1);
    }
}

void Simulation::runSearch() {}

void Simulation::outputPath() {}