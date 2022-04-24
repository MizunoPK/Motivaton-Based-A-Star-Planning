#include "includes.h"
#include "StateSpace.h"
#include "util.h"
#include <iostream>

StateSpace::StateSpace(std::vector<int>& x_bounds, std::vector<int>& y_bounds, std::vector<double>& state_bounds) {
    FUNCTION_TRACE << "State Space constructor called" << ENDL;
    this->x_bounds = x_bounds;
    this->y_bounds = y_bounds;
    this->state_bounds = state_bounds;

    // Calculate the scaling factor that will be needed to ensure g cost and h cost are weighted the same
    double max_h_cost = abs(x_bounds.at(0) - x_bounds.at(1)) + abs(y_bounds.at(0) - y_bounds.at(1));
    DEBUG << "Max H Cost calculated as: " << max_h_cost << ENDL;

    double maxStateDifference = this->state_bounds.at(1) - this->state_bounds.at(0);
    DEBUG << "Maximum difference in state calculated as: " << maxStateDifference << ENDL;

    this->gCostScale = max_h_cost / maxStateDifference;
    DEBUG << "G cost scaling amount calculated as: " << this->gCostScale << ENDL;
}

void StateSpace::setNode(std::string key, std::shared_ptr<Node> val) {
    FUNCTION_TRACE << "StateSpace::setNode called" << ENDL;
    this->graph[key] = val;
}
 
std::shared_ptr<Node> StateSpace::getNode(std::vector<int> nodeCoord) {
    FUNCTION_TRACE << "StateSpace::getNode called" << ENDL;
    std::string nodeStr = getCoordString(&nodeCoord);
    if ( this->graph.find(nodeStr) == this->graph.end() ) {
        FATAL << "Tried to get node " << nodeStr << " which does not exist in the graph. Make sure the node was defined in the graph file." << ENDL;
        exit(-1);
    }

    return this->graph[nodeStr];
}

void StateSpace::addNeighbor(std::vector<std::shared_ptr<Node>>& adjacencies, std::vector<int> neighbor) {
    FUNCTION_TRACE << "StateSpace::addNeighbor called" << ENDL;
    if ( neighbor.at(0) >= this->x_bounds.at(0)
        && neighbor.at(0) <= this->x_bounds.at(1)
        && neighbor.at(1) >= this->y_bounds.at(0)
        && neighbor.at(1) <= this->y_bounds.at(1)
        && this->graph.find(getCoordString(&neighbor)) != this->graph.end() ) {
            adjacencies.push_back(this->getNode(neighbor));
    }
}

std::vector<std::shared_ptr<Node>> StateSpace::getAdjacencyList(std::shared_ptr<Node> node) {
    FUNCTION_TRACE << "StateSpace::getAdjacencyList called" << ENDL;
    std::vector<std::shared_ptr<Node>> adjacencies;
    std::vector<int>* coord = node->getCoord();
    addNeighbor(adjacencies, std::vector<int> {coord->at(0) + 1, coord->at(1)});
    addNeighbor(adjacencies, std::vector<int> {coord->at(0) - 1, coord->at(1)});
    addNeighbor(adjacencies, std::vector<int> {coord->at(0), coord->at(1) + 1});
    addNeighbor(adjacencies, std::vector<int> {coord->at(0), coord->at(1) - 1});
    return adjacencies;
}

bool StateSpace::isInGraph(std::vector<int>* coord) {
    return this->graph.find(getCoordString(coord)) != this->graph.end();
}

std::vector<double>* StateSpace::getStateBounds() {return &(this->state_bounds);}
double StateSpace::getGCostScale() {return this->gCostScale;}

// * Debug Tools:
void StateSpace::printNodes() {
    FUNCTION_TRACE << "StateSpace::printNodes called" << ENDL;
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

            std::cout << " - " << val->getCanChangeAgent();

            NEWL;
        }
    }
}