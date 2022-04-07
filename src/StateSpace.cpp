#include "StateSpace.h"
#include "util.h"
#include <iostream>

void StateSpace::initNodes(std::map<std::string, std::shared_ptr<Node>> nodes) {
    this->nodes = nodes;
}

void StateSpace::initAdjacencies(std::map<std::shared_ptr<Node>, std::vector<std::shared_ptr<Adjacency>>*> adjacencyMap) {
    this->adjacencyMap = adjacencyMap;
}
 
std::shared_ptr<Node> StateSpace::getNode(std::string nodeName) {
    return this->nodes[nodeName];
}

std::vector<std::shared_ptr<Adjacency>>* StateSpace::getAdjacencyList(std::shared_ptr<Node> node) {
    return this->adjacencyMap[node];
}

// Debug Tools:
void StateSpace::printNodes() {
    std::cout << "State Space Nodes:" << std::endl;
    for (auto const& [key, val] : this->nodes) {
        std::cout << key
                << " (" << val << ") : ";

        printStateVector(val->getState());
        std::cout << " - ";
        printStateVector(val->getModifiers());

        std::cout << std::endl;
    }
}
void StateSpace::printAdjacencies() {
    std::cout << "State Space Adjacency Table:" << std::endl;
    for (auto const& [key, val] : this->adjacencyMap) {
        std::cout << key->getName() << ":[";

        for ( int i=0; i < val->size(); i++ ) {
            std::cout << "(" << val->at(i)->node->getName() << "-" << val->at(i)->weight << ")";
            if ( i < val->size() - 1 ) {
                std::cout << ",";
            }
        }

        std::cout << "]" << std::endl;
    }
}