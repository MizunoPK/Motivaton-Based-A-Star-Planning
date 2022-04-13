#include "includes.h"
#include "Node.h"

Node::Node(std::vector<int>& coord, std::vector<double>& state, std::vector<double>& modifiers) {
    this->coord = coord;
    this->state = state;
    this->modifiers = modifiers;

    // Check if this node will be able to change the agent
    this->canChangeAgent = false;
    for ( int i = 0; i < modifiers.size(); i++ ) {
        if ( modifiers.at(i) != 0 ) {
            this->canChangeAgent = true;
            break;
        }
    }
}

// * Getters:
std::vector<int>* Node::getCoord() {return &(this->coord);}
std::vector<double>* Node::getState() {return &(this->state);}
std::vector<double>* Node::getModifiers() {return &(this->modifiers);}
bool Node::getCanChangeAgent() {return this->canChangeAgent;};