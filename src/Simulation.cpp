#include "includes.h"
#include "Simulation.h"
#include "util.h"
#include <fstream>
#include <vector>

Simulation::Simulation(std::string graphFile, std::string agentFile) {
    initializeStateSpace(graphFile);
    initializeAgent(agentFile);

    // Debugging Prints
    this->ss->printNodes();
    this->agent->printAgentInfo();
}

void Simulation::initializeStateSpace(std::string graphFile) {
    // Process the Graph File
    std::fstream graphFileStream(graphFile);
    if (graphFileStream.is_open()) {
        std::string fileLine;

        // Get the bounds
        std::getline(graphFileStream, fileLine);
        std::vector<int> x_bounds = splitIntList(fileLine);
        std::getline(graphFileStream, fileLine);
        std::vector<int> y_bounds = splitIntList(fileLine);
        this->ss = std::make_shared<StateSpace>(x_bounds, y_bounds);

        while (graphFileStream) {
            std::getline(graphFileStream, fileLine);
            std::vector<std::string> lineVector = split(fileLine, ' ');

            // Get node coords
            std::string coordsString = lineVector.at(0);
            std::vector<int> coords = splitIntList(coordsString);
            
            // Get node internal state
            std::vector<int> state = splitIntList(lineVector.at(1));

            // Get node agent-modifiers
            std::vector<int> modifiers = splitIntList(lineVector.at(2));

            // Make a node and add it to the state space graph
            ss->setNode(coordsString, std::make_shared<Node>(coords, state, modifiers));
        }
        graphFileStream.close();
    }
    else {
        ERROR << "Could not open Node File \"" << graphFile << "\"" << ENDL;
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
        std::vector<int> actorState = splitIntList(fileLine);

        // Second Line is Starting Node:
        std::getline(agentFileStream, fileLine);
        std::shared_ptr<Node> startingNode = this->ss->getNode(splitIntList(fileLine));

        // Third Line is Primary Goal:
        std::getline(agentFileStream, fileLine);
        std::shared_ptr<Node> primaryGoal = this->ss->getNode(splitIntList(fileLine));

        // Fourth Line is Secondary Goals:
        std::getline(agentFileStream, fileLine);
        std::vector<std::shared_ptr<Node>> secondaryGoals;
        if (fileLine != "NULL") {
            std::vector<std::string> nodeStrs = split(fileLine, '-');
            for ( int i=0; i < nodeStrs.size(); i++ ) {
                std::shared_ptr<Node> goal = this->ss->getNode(splitIntList(nodeStrs.at(i)));
                secondaryGoals.push_back(goal);
            }
        }
        
        // Make the agent
        agent = std::make_shared<Agent>(actorState, startingNode, primaryGoal, secondaryGoals);
        agentFileStream.close();
    }
    else {
        ERROR << "Could not open Agent File \"" << agentFile << "\"" << ENDL;
        exit(-1);
    }
}

// void Simulation::runSearch() {
//     // define OPEN - priority queue of nodes ready to be evaluated
//     std::vector<std::shared_ptr<SearchNode>> openQueue;

//     // define CLOSED - a set of nodes already evaluated
//     //               - defined as a map associating a Node -> SearchNode ... 
//     //                  Makes it easier to find a SearchNode and ensure only one SearchNode exists per Node
//     std::unordered_map<std::shared_ptr<Node>, std::shared_ptr<SearchNode>> closedMap;

//     // add the start node to OPEN
//     std::weak_ptr<SearchNode> w;
//     openQueue.push_back(std::make_shared<SearchNode>(agent->getStartingNode(), 0, w));

//     // loop through the search
//     while (true) {
//         std::shared_ptr<SearchNode> current = openQueue.back(); // current = node in OPEN with the lowest f_cost
//         openQueue.pop_back(); // remove current from OPEN
//         closedMap[current->node] = current; // add current to CLOSED
//         bool sortOPEN = false; // if we make any changes that need us to re-sort open, update this flag

//         // if current is the target node - the path has been found
//         if ( current->node == agent->getPrimaryGoal() ) {
//             break;
//         }
        
//         // foreach neighbor of the current node
//         std::vector<std::shared_ptr<Adjacency>> neighbors = this->ss->getAdjacencyList(current->node);
//         for ( int i=0; i < neighbors.size(); i++ ) {
//             std::shared_ptr<Adjacency> neighbor = neighbors.at(i);
//             // if neighbor is in CLOSED
//             if ( closedMap.find(neighbor->node) != closedMap.end() ) {
//                 // skip to the next neighbor
//                 continue;
//             }

//             // TODO - Calculate the cost of the path to this neighbor
//             // g_cost = distance from starting node
//             // h_cost = heuristic calculated distance from end node
//             // f_cost = g_cost + h_cost ... The total cost of the node we use to determine if we want to move there
//             double f_cost;

//             // Determine the location of the neighbor in OPEN
//             int neighbor_i = -1; // -1 index indiciates the neighbor is not in OPEN
//             for ( int j=0; j < openQueue.size(); j++ ) {
//                 if ( openQueue.at(j)->node == neighbor->node ) {
//                     neighbor_i = j;
//                     break;
//                 }
//             }

//             // if neighbor is not in OPEN - add it
//             if ( neighbor_i == -1 ) {
//                 openQueue.push_back(std::make_shared<SearchNode>(neighbor->node, f_cost, current));
//                 sortOPEN = true;
//             }
//             // it is in OPEN and the new path to neighbor is shorter - update its f_cost and parent node
//             else if (neighbor_i != -1 && f_cost < openQueue.at(neighbor_i)->f_cost) {
//                 openQueue.at(i)->f_cost = f_cost;
//                 std::weak_ptr<SearchNode> prevNode = current;
//                 openQueue.at(i)->prevNode = prevNode;
//                 sortOPEN = true;
//             }
//         }
        
//         // if internal state changed, or anything in OPEN changed, re-sort OPEN
//         if ( sortOPEN ) {
//             // Use QUICK sort
//             this->quickSort(&openQueue, 0, openQueue.size() - 1);
//         }
//     }
// }

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