#include "includes.h"
#include "Node.h"

Node::Node(std::vector<int> coord, std::vector<int> state, std::vector<int> modifiers) {
    this->coord = coord;
    this->state = state;
    this->modifiers = modifiers;
}

// * Getters:
std::vector<int> Node::getCoord() {return this->coord;}
std::vector<int> Node::getState() {return this->state;}
std::vector<int> Node::getModifiers() {return this->modifiers;}

double Node::getWeight() {return (double)this->state.at(0);}