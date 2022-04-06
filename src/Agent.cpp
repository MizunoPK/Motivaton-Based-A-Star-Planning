#include "Agent.h"
#include "util.h"
#include <iostream>

Agent::Agent(std::vector<int> state, Node* startingNode, Node* primaryGoal, std::vector<Node*> secondaryGoals) {
    this->state = state;
    this->startingNode = startingNode;
    this->primaryGoal = primaryGoal;
    this->secondaryGoals = secondaryGoals;
}

// Getters
std::vector<int> Agent::getState() {return this->state;}
Node* Agent::getStartingNode() {return this->startingNode;}
Node* Agent::getPrimaryGoal() {return this->primaryGoal;}
std::vector<Node*> Agent::getSecondaryGoals() {return this->secondaryGoals;}

// Debugging Functions:
void Agent::printAgentInfo() {
    std::cout << "Agent Internal State: ";
    printStateVector(this->state);
    std::cout << std::endl;

    std::cout << "Starting Node: " << this->startingNode->getName() << std::endl;

    std::cout << "Primary Goal: " << this->primaryGoal->getName() << std::endl;

    std::cout << "Secondary Goals: [";
    for ( int i=0; i < this->secondaryGoals.size(); i++ ) {
        std::cout << this->secondaryGoals.at(i)->getName();
        if ( i < this->secondaryGoals.size() - 1 ) {
            std::cout << ",";
        }
    }
    std::cout << "]" << std::endl;
}