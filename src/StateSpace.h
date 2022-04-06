#ifndef STATE_SPACE_H
#define STATE_SPACE_H

#include <map>
#include <vector>
#include "Node.h"

// Helper Struct
// Stores the Node* and Weight for some adjacency in the Adjacencies Table
struct Adjacency {
    Node* node;
    int weight;
};

class StateSpace {
private:
    // Map: Nodes
    // Node Name -> Node Pointer 
    std::map<std::string, Node*> nodes;

    // Map: adjacencyMap
    // Node* -> Vector of Tuples: (Node*, Travel Weight)
    std::map<Node*, std::vector<Adjacency*>*> adjacencyMap;
public:
    // Function: initNodes
    // Description: initializes the nodes map with the provided data
    void initNodes(std::map<std::string, Node*>);

    // Function: initAdjacencies
    // Description: initializes the adjacencyList map with the provided data
    void initAdjacencies(std::map<Node*, std::vector<Adjacency*>*>);
    
    // Function: getNode
    // Input: nodeName - The name of the node being fetched
    // Output: Node* - A pointer to the fetched node 
    // Description: Accesses the nodes map and returns the requested Node pointer 
    Node* getNode(std::string nodeName);

    // Function: getAdjacencyList
    // Input: node - The pointer of the node whose list is being fetched
    // Output: vector - The associated adjacencyList 
    // Description: Accesses the adjacencyList map and returns the requested vector
    std::vector<Adjacency*>* getAdjacencyList(Node* node);

    // Debug Tools:
    void printNodes();
    void printAdjacencies();
};


#endif //STATE_SPACE_H
