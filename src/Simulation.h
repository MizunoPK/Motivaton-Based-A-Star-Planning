#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <string>
#include <memory>
#include <unordered_set>
#include "StateSpace.h"
#include "Node.h"
#include "Agent.h"
#include <chrono> //Time Measuring

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

    // Another helper structure used only by the search
    // Used to store information about each node in the final path
    // This information is used by the outputToFile function
    struct FinalPathNode {
        std::shared_ptr<Node> node, goalNode;
        std::vector<double> agentState;
        std::vector<std::shared_ptr<Node>> anticipatedPath;

        FinalPathNode(std::shared_ptr<Node> n, std::vector<double> as, std::vector<std::shared_ptr<Node>> ap, std::shared_ptr<Node> g) {
            node = n;
            agentState = as;
            anticipatedPath = ap;
            goalNode = g;
        }

    };

private:
    // * VARIABLES
    std::shared_ptr<StateSpace> ss;
    std::shared_ptr<Agent> agent;
    std::string outputPath;
    bool makePrettyOutput;
    std::vector<std::shared_ptr<FinalPathNode>> finalPath; 
    bool agentCanChange;

    // * EVALUATION METRIC STUFF
    // Time
    // Using time point and system_clock
    std::chrono::duration<double> total_elapsed_seconds;
    std::chrono::duration<double> max_astar_time;
    void endAStarClock(std::chrono::time_point<std::chrono::system_clock> start);
    int sGoalsReached = 0;

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

    // * getPath
    // Description: Gets the best possible path forward from the given start node
    // Inputs:
    //      startingNode - The node we would be starting the path from
    //      &anticiptedPath - A reference to the vector we'll be storing the anticipatedPath. This allows us to functionally return two things
    // Output: goalNode - Return the secondary goal that was chosen, or return the p-goal if no s-goal was chosen
    std::shared_ptr<Node> getPath(std::shared_ptr<Node> startingNode, std::vector<std::shared_ptr<Node>> &anticipatedPath);

    // * getSecondaryGoals
    // Description: Compiles a list of viable secondary goals within vision range. 
    // Inputs
    //      &viableGoals - A vector of viable secondary goal nodes. Use the SearchNode struct so that we can sort them by g_costs
    //      &checkedNodes - a hash set of already checked nodes
    //      centerNode - The node at the center of the vision radius for this call
    //      range - the vision range of the center node, this decrements with each call to getSecondaryGoals
    void getSecondaryGoals(std::vector<std::shared_ptr<SearchNode>> &viableGoals, std::unordered_set<std::shared_ptr<Node>> &checkedNodes, std::shared_ptr<Node> centerNode, int range);

    // * Function: runAstar
    // Description: Runs one interation of A* for the given start node
    // Inputs:
    //      startNode - The starting node of the path being searched for
    //      goalNode - the node at the end of the path
    // Output: vector<Node> - a vector of nodes representing the generated path
    std::vector<std::shared_ptr<Node>> runAstar(std::shared_ptr<Node> startingNode, std::shared_ptr<Node> goalNode);

    // * Function: calculateGCost
    // Description: For the given two state vectors, calculate the G-cost
    // Input: Two vectors of state values being compared
    // Output: double - The weight associated from going between the two nodes
    double calculateGCost(std::vector<double>* v1, std::vector<double>* v2);

    // * Function: calculateHCost
    // Description: Calculate the heuristic h_cost between the two given nodes
    // Input: Two nodes being compared
    // Output: double - the calculated h_cost
    double calculateHCost(std::shared_ptr<Node> n1, std::shared_ptr<Node> n2);

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
    int partition(std::vector<std::shared_ptr<SearchNode>>* vector, int low, int high);

    // * Function: derivePathFromClosed 
    // Description: Outputs the resultant path of an individual A* search to a vector, given a closed map of SearchNodes
    std::vector<std::shared_ptr<Node>> derivePathFromClosed(std::unordered_map<std::shared_ptr<Node>, std::shared_ptr<SearchNode>>* closedMap, std::shared_ptr<Node> startingNode, std::shared_ptr<Node> goalNode);

    // * Function: recurseThroughSearchNode
    // Description: Helper function for derivePathFromClosed. Recursively outputs the nodes in the path to the provided vector
    void recurseThroughSearchNode(std::vector<std::shared_ptr<Node>>* path, std::weak_ptr<SearchNode> pathNode, std::shared_ptr<Node> startingNode);

    // * Function outputToFile
    // Description: Outputs the resultant path of the graph to the designated output file
    void outputToFile();
    
public:
    // * Function: Constructor
    // Description: Initializes all the information provided to the simulation via the config files
    // Inputs:
    //      graphFile - The path to the file outlining the graph nodes
    //      agentFile - The path to the file outlining agent's starting state, starting node, and primary/secondary goals
    //      outputFile - The path to where we want to output the results of the search
    //      makePrettyOutput - a boolean flag set when we want a readable output file
    Simulation(std::string graphFile, std::string agentFile, std::string outputFile, bool makePrettyOutput);

    // * Function: runSearch 
    // Description: Runs the A* search on the initialized data
    void runSearch();
};


#endif //SIMULATION_H
