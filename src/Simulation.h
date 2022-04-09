#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <string>
#include <memory>
#include "StateSpace.h"
#include "Node.h"
#include "Agent.h"

class Simulation {
    // This is a helper structure used only by the search
    // Used for a linked list of nodes to define a path
    // Associated a Node in a path with an f_cost, and the previous node in the path
    struct SearchNode {
        std::shared_ptr<Node> node;
        double f_cost;
        std::weak_ptr<SearchNode> prevNode;

        SearchNode(std::shared_ptr<Node> n, double f, std::weak_ptr<SearchNode> p) {
            node = n;
            f_cost = f;
            prevNode = p;
        }
    };

private:
    // * VARIABLES
    std::shared_ptr<StateSpace> ss;
    std::shared_ptr<Agent> agent;

    // * Function: initializeStateSpace
    // Description: Initializes the local StateSpace object with the information from the input files
    // Input: 
    //      nodesFile - The path to the file outlining the nodes that are being pathed
    //      adjacenciesFile - The path to the file outline the nodes' adjacency list
    void initializeStateSpace(std::string nodeFile, std::string adjacenciesFile);

    // * Function: initStateSpaceNodes
    // Description: Initializes the nodes for the StateSpace object with the information from the input files
    // Input: 
    //      nodesFile - The path to the file outlining the nodes that are being pathed
    void initStateSpaceNodes(std::string nodeFile);

    // * Function: initStateSpaceAdjs
    // Description: Initializes the adjacency lists for the StateSpace object with the information from the input files
    // Input: 
    //      adjacenciesFile - The path to the file outline the nodes' adjacency list
    void initStateSpaceAdjs(std::string adjacenciesFile);

    // * Function: initializeAgent
    // Description: Initializes everything related to the agent
    // Input: 
    //      agentFile - The path to the file outlining agent's starting state, starting node, and primary/secondary goals
    void initializeAgent(std::string agentFile);

    /// * Quick Sort Functions:
    // Description: The main function that implements QuickSort
    // Function: quickSort
    // Input: 
    //      vector --> pointer to the vector being sorted
    //      low --> Starting index,
    //      high --> Ending index
    void quickSort(std::vector<std::shared_ptr<SearchNode>>* vector, int low, int high);

    // * Function: partition
    // Description: Picks a pivot element from the vector, and moves everything smaller than it to be before the pivot, and everything larger is palced after the pivot
    // Input: Same as quickSort
    // Output: The index of the partition
    int partition (std::vector<std::shared_ptr<SearchNode>>* vector, int low, int high);

    
public:
    // * Function: Constructor
    // Description: Initializes all the information provided to the simulation via the config files
    // Inputs:
    //      nodesFile - The path to the file outlining the nodes that are being pathed
    //      adjacenciesFile - The path to the file outline the nodes' adjacency list
    //      agentFile - The path to the file outlining agent's starting state, starting node, and primary/secondary goals
    Simulation(std::string nodeFile, std::string adjacenciesFile, std::string agentFile);

    // * Function: runSearch 
    // Description: Runs the A* search on the initialized data
    void runSearch();

    // * Function: outputPath 
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
