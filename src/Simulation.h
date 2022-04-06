#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <string>
#include "StateSpace.h"
#include "Node.h"
#include "Agent.h"

class Simulation {
private:
    // VARIABLES
    StateSpace* ss;
    Agent* agent;

    // Function: initializeStateSpace
    // Input: 
    //      nodesFile - The path to the file outlining the nodes that are being pathed
    //      adjacenciesFile - The path to the file outline the nodes' adjacency list
    // Description: Initializes the local StateSpace object with the information from the input files
    void initializeStateSpace(std::string nodeFile, std::string adjacenciesFile);

    // Function: initStateSpaceNodes
    // Input: 
    //      nodesFile - The path to the file outlining the nodes that are being pathed
    // Description: Initializes the nodes for the StateSpace object with the information from the input files
    void initStateSpaceNodes(std::string nodeFile);

    // Function: initStateSpaceAdjs
    // Input: 
    //      adjacenciesFile - The path to the file outline the nodes' adjacency list
    // Description: Initializes the adjacency lists for the StateSpace object with the information from the input files
    void initStateSpaceAdjs(std::string adjacenciesFile);

    // Function: initializeAgent
    // Input: 
    //      agentFile - The path to the file outlining agent's starting state, starting node, and primary/secondary goals
    // Description: Initializes everything related to the agent
    void initializeAgent(std::string agentFile);

    
public:
    // Function: Constructor
    // Inputs:
    //      nodesFile - The path to the file outlining the nodes that are being pathed
    //      adjacenciesFile - The path to the file outline the nodes' adjacency list
    //      agentFile - The path to the file outlining agent's starting state, starting node, and primary/secondary goals
    // Description: Initializes all the information provided to the simulation via the config files
    Simulation(std::string nodeFile, std::string adjacenciesFile, std::string agentFile);

    // Function: runSearch 
    // Description: Runs the A* search on the initialized data
    void runSearch();

    // Function: outputPath 
    // Description: Outputs the resultant path to a file
    // Output File Format:
    //      {Starting Node (step 0)}
    //      {Node at step 1}
    //      {Node at step 2}
    //      ...
    //      {Node at step (n-1)}
    //      {Primary Goal Node}
    void outputPath();
};


#endif //SIMULATION_H
