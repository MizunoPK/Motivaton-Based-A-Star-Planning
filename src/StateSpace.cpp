#include "StateSpace.h"
#include <iostream>

void StateSpace::initNodes(std::map<std::string, Node*> nodes) {
    this->nodes = nodes;
}

void StateSpace::initAdjacencies(std::map<Node*, std::vector<Adjacency*>*> adjacencyMap) {
    this->adjacencyMap = adjacencyMap;
}
 
Node* StateSpace::getNode(std::string nodeName) {
    return this->nodes[nodeName];
}

std::vector<Adjacency*>* StateSpace::getAdjacencyList(Node* node) {
    return this->adjacencyMap[node];
}

// Debug Tools:
void StateSpace::printNodes() {
    std::cout << "State Space Nodes:" << std::endl;
    for (auto const& [key, val] : this->nodes) {
        std::cout << key
                << ':'
                << val
                << std::endl;
    }
}
void StateSpace::printAdjacencies() {
    std::cout << "State Space Adjacency Table:" << std::endl;
    for (auto const& [key, val] : this->adjacencyMap) {
        std::cout << key->getName() << ":[";

        for ( int i=0; i < val->size(); i++ ) {
            std::cout << "(" << val->at(i)->node << "-" << val->at(i)->weight << "),";
        }

        std::cout << "]" << std::endl;
    }
}