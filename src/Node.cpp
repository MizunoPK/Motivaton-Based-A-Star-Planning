#include "Node.h"

Node::Node(std::string name, std::vector<int> state, std::vector<int> modifiers) {
    this->name = name;
    this->state = state;
    this->modifiers = modifiers;
}

// Getters:
std::string Node::getName() {return this->name;}
std::vector<int> Node::getState() {return this->state;}
std::vector<int> Node::getModifiers() {return this->modifiers;}