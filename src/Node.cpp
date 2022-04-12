#include "includes.h"
#include "Node.h"

Node::Node(std::vector<int> coord, std::vector<double> state, std::vector<double> modifiers) {
    this->coord = coord;
    this->state = state;
    this->modifiers = modifiers;
}

// * Getters:
std::vector<int> Node::getCoord() {return this->coord;}
std::vector<double> Node::getState() {return this->state;}
std::vector<double> Node::getModifiers() {return this->modifiers;}

double Node::getWeight() {return (double)this->state.at(0);}