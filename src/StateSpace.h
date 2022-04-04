#ifndef STATE_SPACE_H
#define STATE_SPACE_H

#include <map>
#include <tuple>
#include <vector>
#include "Node.h"

class StateSpace {
private:
    // Map: Nodes
    // Node Name -> Node Pointer 
    std::map<std::string, Node*> nodes;

    // Map: adjacencyList
    // Node Name -> Vector of Tuples: (Node Pointer, Travel Weight)
    std::map<std::string, std::vector<std::tuple<Node*, int>>> adjacencyList;
public:
    // Function: Constructor
    StateSpace(std::map<std::string, Node*>, std::map<std::string, std::vector<std::tuple<Node*, int>>>);
    
    // Function: getNode
    // Input: nodeName - The name of the node being fetched
    // Output: Node* - A pointer to the fetched node 
    // Description: Accesses the nodes map and returns the requested Node pointer 
    Node* getNode(std::string nodeName);

    // Function: getAdjacencyList
    // Input: nodeName - The name of the node being fetched
    // Output: vector - The associated adjacencyList 
    // Description: Accesses the adjacencyList map and returns the requested vector
    std::vector<std::tuple<Node*, int>> getAdjacencyList(std::string nodeName);
};


#endif //STATE_SPACE_H
