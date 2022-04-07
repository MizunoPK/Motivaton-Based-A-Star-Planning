#ifndef STATE_SPACE_H
#define STATE_SPACE_H

#include <map>
#include <vector>
#include "Node.h"
#include <memory>

// Helper Struct
// Stores the std::shared_ptr<Node> and Weight for some adjacency in the Adjacencies Table
struct Adjacency {
    std::shared_ptr<Node> node;
    int weight;

    Adjacency(std::shared_ptr<Node> n, int w) {
        node = n;
        weight = w;
    }
};

class StateSpace {
private:
    // Map: Nodes
    // Node Name -> Node Pointer 
    std::map<std::string, std::shared_ptr<Node>> nodes;

    // Map: adjacencyMap
    // std::shared_ptr<Node> -> Vector of Tuples: (std::shared_ptr<Node>, Travel Weight)
    std::map<std::shared_ptr<Node>, std::vector<std::shared_ptr<Adjacency>>*> adjacencyMap;
public:
    // Function: initNodes
    // Description: initializes the nodes map with the provided data
    void initNodes(std::map<std::string, std::shared_ptr<Node>>);

    // Function: initAdjacencies
    // Description: initializes the adjacencyList map with the provided data
    void initAdjacencies(std::map<std::shared_ptr<Node>, std::vector<std::shared_ptr<Adjacency>>*>);
    
    // Function: getNode
    // Input: nodeName - The name of the node being fetched
    // Output: std::shared_ptr<Node> - A pointer to the fetched node 
    // Description: Accesses the nodes map and returns the requested Node pointer 
    std::shared_ptr<Node> getNode(std::string nodeName);

    // Function: getAdjacencyList
    // Input: node - The pointer of the node whose list is being fetched
    // Output: vector - The associated adjacencyList 
    // Description: Accesses the adjacencyList map and returns the requested vector
    std::vector<std::shared_ptr<Adjacency>>* getAdjacencyList(std::shared_ptr<Node> node);

    // Debug Tools:
    void printNodes();
    void printAdjacencies();
};


#endif //STATE_SPACE_H
