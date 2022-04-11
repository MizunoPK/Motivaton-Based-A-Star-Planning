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
        double g_cost;
        double f_cost;
        std::weak_ptr<SearchNode> prevNode;

        SearchNode(std::shared_ptr<Node> n, double g, double f, std::weak_ptr<SearchNode> p) {
            node = n;
            g_cost = g;
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
    //      graphFile - The path to the file outlining the graph nodes
    void initializeStateSpace(std::string graphFile);

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

    // * Function: outputPath 
    // Description: Outputs the resultant path to a file
    void outputPath(std::unordered_map<std::shared_ptr<Node>, std::shared_ptr<SearchNode>>* closedMap);

    // * Function: findPath
    // Description: Helper function for outputPath. Recursively outputs the nodes in the path to the provided vector
    void findPath(std::vector<std::shared_ptr<Node>>* path, std::weak_ptr<SearchNode> pathNode);
    
public:
    // * Function: Constructor
    // Description: Initializes all the information provided to the simulation via the config files
    // Inputs:
    //      graphFile - The path to the file outlining the graph nodes
    //      agentFile - The path to the file outlining agent's starting state, starting node, and primary/secondary goals
    Simulation(std::string graphFile, std::string agentFile);

    // * Function: runSearch 
    // Description: Runs the A* search on the initialized data
    void runSearch();
};


#endif //SIMULATION_H
