#include "includes.h"
#include "Agent.h"
#include "util.h"
#include <iostream>

Agent::Agent(std::vector<double>& state, std::shared_ptr<Node> startingNode, std::shared_ptr<Node> primaryGoal, std::vector<std::shared_ptr<Node>>& secondaryGoals) {
    this->state = state;
    this->startingNode = startingNode;
    this->primaryGoal = primaryGoal;
    this->secondaryGoals = secondaryGoals;
}

// * Getters
std::vector<double> Agent::getState() {return this->state;}
std::shared_ptr<Node> Agent::getStartingNode() {return this->startingNode;}
std::shared_ptr<Node> Agent::getPrimaryGoal() {return this->primaryGoal;}
std::vector<std::shared_ptr<Node>> Agent::getSecondaryGoals() {return this->secondaryGoals;}

// * Debugging Functions:
void Agent::printAgentInfo() {
    if (LOGGING_LEVEL > 3) {
        DEBUG << "Agent Internal State: ";
        printDoubleVector(this->state);
        NEWL;

        DEBUG << "Starting Node: "; 
        printIntVector(this->startingNode->getCoord());
        NEWL;

        DEBUG << "Primary Goal: ";
        printIntVector(this->primaryGoal->getCoord());
        NEWL;

        DEBUG << "Secondary Goals: [";
        for ( int i=0; i < this->secondaryGoals.size(); i++ ) {
            printIntVector(this->secondaryGoals.at(i)->getCoord());
            if ( i < this->secondaryGoals.size() - 1 ) {
                std::cout << ",";
            }
        }
        std::cout << "]" << ENDL;
    }
}