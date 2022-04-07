#include "Simulation.h"
#include "util.h"
#include <fstream>
#include <unordered_map>
#include <vector>
#include <unordered_set>

Simulation::Simulation(std::string nodeFile, std::string adjacenciesFile, std::string agentFile) {
    initializeStateSpace(nodeFile, adjacenciesFile);
    initializeAgent(agentFile);

    // Debugging Prints
    this->ss->printNodes();
    this->ss->printAdjacencies();
    this->agent->printAgentInfo();
}

void Simulation::initializeStateSpace(std::string nodeFile, std::string adjacenciesFile) {
    this->ss = std::make_shared<StateSpace>();
    this->initStateSpaceNodes(nodeFile);
    this->initStateSpaceAdjs(adjacenciesFile);
}

void Simulation::initStateSpaceNodes(std::string nodeFile) {
    // Process the Node File
    std::fstream nodeFileStream(nodeFile);
    if (nodeFileStream.is_open()) {
        // Make the map of nodes 
        std::unordered_map<std::string, std::shared_ptr<Node>> nodeMap;
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

            nodeMap[lineVector.at(0)] = std::make_shared<Node>(nodeName, state, modifiers);
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
        std::unordered_map<std::shared_ptr<Node>, std::vector<std::shared_ptr<Adjacency>>> adjMap;
        std::string fileLine;
        while (adjFileStream) {
            std::getline(adjFileStream, fileLine);
            std::vector<std::string> lineVector = split(fileLine, ' ');

            // Get the node this line is defining the adjacency list for
            std::shared_ptr<Node> baseNode = this->ss->getNode(lineVector.at(0));

            // Parse the adjacency list
            std::vector<std::shared_ptr<Adjacency>> adjacencyList;
            if ( lineVector.at(1) != "NULL" ) {
                std::vector<std::string> adjString = split(lineVector.at(1), ',');
                for ( int i=0; i < adjString.size(); i++ ) {
                    std::vector<std::string> adjacency = split(adjString.at(i), '-');

                    std::shared_ptr<Node> adjacentNode = this->ss->getNode(adjacency.at(0));
                    int weight = std::stoi(adjacency.at(1));
                    std::shared_ptr<Adjacency> adjacencyTuple = std::make_shared<Adjacency>(adjacentNode, weight);

                    adjacencyList.push_back(adjacencyTuple);
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
        std::shared_ptr<Node> startingNode = this->ss->getNode(fileLine);

        // Third Line is Primary Goal:
        std::getline(agentFileStream, fileLine);
        std::shared_ptr<Node> primaryGoal = this->ss->getNode(fileLine);

        // Fourth Line is Secondary Goals:
        std::getline(agentFileStream, fileLine);
        std::vector<std::shared_ptr<Node>> secondaryGoals;
        if (fileLine != "NULL") {
            std::vector<std::string> nodeStrs = split(fileLine, ',');
            for ( int i=0; i < nodeStrs.size(); i++ ) {
                std::shared_ptr<Node> goal = this->ss->getNode(nodeStrs.at(i));
                secondaryGoals.push_back(goal);
            }
        }
        
        // Make the agent
        agent = std::make_shared<Agent>(actorState, startingNode, primaryGoal, secondaryGoals);
        agentFileStream.close();
    }
    else {
        std::cout << "Error: Could not open Agent File \"" << agentFile << "\"" << std::endl;
        exit(-1);
    }
}

void Simulation::runSearch() {
    // define OPEN - priority queue of nodes ready to be evaluated
    std::vector<std::shared_ptr<SearchNode>> openQueue;

    // define CLOSED - a set of nodes already evaluated
    //               - defined as a map associating a Node -> SearchNode ... 
    //                  Makes it easier to find a SearchNode and ensure only one SearchNode exists per Node
    std::unordered_map<std::shared_ptr<Node>, std::shared_ptr<SearchNode>> closedMap;

    // add the start node to OPEN
    std::weak_ptr<SearchNode> w;
    openQueue.push_back(std::make_shared<SearchNode>(agent->getStartingNode(), 0, w));

    // loop through the search
    while (true) {
        std::shared_ptr<SearchNode> current = openQueue.back(); // current = node in OPEN with the lowest f_cost
        openQueue.pop_back(); // remove current from OPEN
        closedMap[current->node] = current; // add current to CLOSED
        bool sortOPEN = false; // if we make any changes that need us to re-sort open, update this flag

        // if current is the target node - the path has been found
        if ( current->node == agent->getPrimaryGoal() ) {
            break;
        }
        
        // foreach neighbor of the current node
        std::vector<std::shared_ptr<Adjacency>> neighbors = this->ss->getAdjacencyList(current->node);
        for ( int i=0; i < neighbors.size(); i++ ) {
            std::shared_ptr<Adjacency> neighbor = neighbors.at(i);
            // if neighbor is in CLOSED
            if ( closedMap.find(neighbor->node) != closedMap.end() ) {
                // skip to the next neighbor
                continue;
            }

            // TODO - Calculate the cost of the path to this neighbor
            // g_cost = distance from starting node
            // h_cost = heuristic calculated distance from end node
            // f_cost = g_cost + h_cost ... The total cost of the node we use to determine if we want to move there
            double f_cost;

            // Determine the location of the neighbor in OPEN
            int neighbor_i = -1; // -1 index indiciates the neighbor is not in OPEN
            for ( int j=0; j < openQueue.size(); j++ ) {
                if ( openQueue.at(j)->node == neighbor->node ) {
                    neighbor_i = j;
                    break;
                }
            }

            // if neighbor is not in OPEN - add it
            if ( neighbor_i == -1 ) {
                openQueue.push_back(std::make_shared<SearchNode>(neighbor->node, f_cost, current));
                sortOPEN = true;
            }
            // it is in OPEN and the new path to neighbor is shorter - update its f_cost and parent node
            else if (neighbor_i != -1 && f_cost < openQueue.at(neighbor_i)->f_cost) {
                openQueue.at(i)->f_cost = f_cost;
                std::weak_ptr<SearchNode> prevNode = current;
                openQueue.at(i)->prevNode = prevNode;
                sortOPEN = true;
            }
        }
        
        // if internal state changed, or anything in OPEN changed, re-sort OPEN
        if ( sortOPEN ) {
            // Use QUICK sort
            this->quickSort(&openQueue, 0, openQueue.size() - 1);
        }
    }
}

void Simulation::quickSort(std::vector<std::shared_ptr<SearchNode>>* vector, int low, int high) {
    if ( low < high ) {
      /* p is partitioning index, vector[p] is now
        at right place */
      int p = partition(vector, low, high);

      // Separately sort elements before
      // partition and after partition
      quickSort(vector, low, p - 1);
      quickSort(vector, p + 1, high);
   }
}

int Simulation::partition(std::vector<std::shared_ptr<SearchNode>>* vector, int low, int high) {
    double pivot = vector->at(high)->f_cost; // pivot
    int i = (low - 1); // Index of smaller element and indicates the right position of pivot found so far
 
    for (int j = low; j < high; j++)
    {
        // If current element is smaller than the pivot
        if (vector->at(j)->f_cost < pivot)
        {
            i++; // increment index of smaller element
            vector->at(i).swap(vector->at(j));
        }
    }
    vector->at(i + 1).swap(vector->at(high));
    return (i + 1);
}

void Simulation::outputPath() {}