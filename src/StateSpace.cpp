#include "includes.h"
#include "StateSpace.h"
#include "util.h"
#include <iostream>

StateSpace::StateSpace(std::vector<int>& x_bounds, std::vector<int>& y_bounds) {
    this->x_bounds = x_bounds;
    this->y_bounds = y_bounds;
}

void StateSpace::setNode(std::string key, std::shared_ptr<Node> val) {
    this->graph[key] = val;
}
 
std::shared_ptr<Node> StateSpace::getNode(std::vector<int> nodeCoord) {
    return this->graph[getCoordString(&nodeCoord)];
}

void StateSpace::addNeighbor(std::vector<std::shared_ptr<Node>>& adjacencies, std::vector<int> neighbor) {
    if ( neighbor.at(0) >= this->x_bounds.at(0)
        && neighbor.at(0) <= this->x_bounds.at(1)
        && neighbor.at(1) >= this->y_bounds.at(0)
        && neighbor.at(1) <= this->y_bounds.at(1)
        && this->graph.find(getCoordString(&neighbor)) != this->graph.end() ) {
            adjacencies.push_back(this->getNode(neighbor));
    }
}

std::vector<std::shared_ptr<Node>> StateSpace::getAdjacencyList(std::shared_ptr<Node> node) {
    std::vector<std::shared_ptr<Node>> adjacencies;
    std::vector<int>* coord = node->getCoord();
    addNeighbor(adjacencies, std::vector<int> {coord->at(0) + 1, coord->at(1)});
    addNeighbor(adjacencies, std::vector<int> {coord->at(0) - 1, coord->at(1)});
    addNeighbor(adjacencies, std::vector<int> {coord->at(0), coord->at(1) + 1});
    addNeighbor(adjacencies, std::vector<int> {coord->at(0), coord->at(1) - 1});
    return adjacencies;
}

// * Debug Tools:
void StateSpace::printNodes() {
    if (LOGGING_LEVEL > 3) {
        DEBUG << "X Bounds: ";
        printIntVector(&(this->x_bounds));
        NEWL;

        DEBUG << "Y Bounds: ";
        printIntVector(&(this->y_bounds));
        NEWL;

        DEBUG << "State Space Nodes:" << ENDL;
        for (auto const& [key, val] : this->graph) {
            DEBUG << key
                    << " (" << val << ") : ";

            printDoubleVector(val->getState());
            std::cout << " - ";
            printDoubleVector(val->getModifiers());

            NEWL;
        }
    }
}