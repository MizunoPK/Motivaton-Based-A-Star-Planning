#ifndef STATE_SPACE_H
#define STATE_SPACE_H

#include <unordered_map>
#include <vector>
#include "Node.h"
#include <memory>

class StateSpace {
private:
    // * Map: Graph Nodes
    // Node Coord -> Node Pointer 
    // Opted for a map here instead of a vector because we could potentially have many unused coordinates that are untraversable...
    //    using a vector would just lead to a lot of wasted space.
    std::unordered_map<std::string, std::shared_ptr<Node>> graph;
    
    // Min/Max values for X and Y
    std::vector<int> x_bounds;
    std::vector<int> y_bounds;

    // Min/Max Values for state values
    std::vector<double> state_bounds;

    // The scaling factor for g_costs that helps us ensure g_costs are weighted the same as h_costs
    double gCostScale;

    // Helper Function: addNeighbor
    // Description: Checks if the given neighbor is valid to traverse, if so it adds it to the given vector of nodes
    void addNeighbor(std::vector<std::shared_ptr<Node>>& adjacencies, std::vector<int> neighbor);

public:
    // * Function: Constructpr
    StateSpace(std::vector<int>& x_bounds, std::vector<int>& y_bounds, std::vector<double>& state_bounds);

    // * Function: setNode
    // Description: adds/sets a node to the graph
    void setNode(std::string key, std::shared_ptr<Node> val);
    
    // * Function: getNode
    // Description: Accesses the nodes map and returns the requested Node pointer 
    // Input: nodeCoord - The coordinate of the node being fetched
    // Output: std::shared_ptr<Node> - A pointer to the fetched node 
    std::shared_ptr<Node> getNode(std::vector<int> nodeCoord);

    // * Function: getAdjacencyList
    // Description: Returns a list of nodes that are adjacent to a given node
    // Input: node - The pointer of the node whose list is being fetched
    // Output: vector - The associated adjacencyList 
    std::vector<std::shared_ptr<Node>> getAdjacencyList(std::shared_ptr<Node> node);

    // Getters
    std::vector<double>* getStateBounds();
    double getGCostScale();

    // * Debug Tools:
    void printNodes();
};


#endif //STATE_SPACE_H
