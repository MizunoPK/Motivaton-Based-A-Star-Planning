#include "includes.h"
#include "Agent.h"
#include "util.h"
#include <iostream>
#include <algorithm>
#include <bits/stdc++.h>

Agent::Agent(std::vector<double>& state, std::shared_ptr<Node> startingNode, std::shared_ptr<Node> primaryGoal, std::vector<std::shared_ptr<Node>>& secondaryGoals, int vision, std::vector<double>& goalRanges) {
    this->state = state;
    this->startingNode = startingNode;
    this->primaryGoal = primaryGoal;
    this->secondaryGoals = secondaryGoals;
    this->vision = vision;
    this->goalRanges = goalRanges;
}

void Agent::updateState(std::vector<double>* modifiers, std::vector<double>* stateBounds) {
    for ( int i=0; i < modifiers->size(); i++ ) {
        this->state.at(i) = std::clamp(this->state.at(i) + modifiers->at(i), stateBounds->at(0), stateBounds->at(1));
    }

    if ( LOGGING_LEVEL > 4 ) {
        TRACE << "Updated Agent Internal State to: [";
        for ( int i=0; i < this->state.size(); i++ ) {
            std::cout << this->state.at(i) << ",";
        }
        NEWL;
    }
}

bool Agent::deleteSecondaryGoal(std::shared_ptr<Node> node) {
    auto it = find(this->secondaryGoals.begin(), this->secondaryGoals.end(), node);
    if ( it != this->secondaryGoals.end() ) {
        TRACE << "A secondary goal has been reached... Deleting from list of secondary goals" << ENDL;
        this->secondaryGoals.erase(it);
        return true;
    }
    return false;
}

// * Getters
std::vector<double>* Agent::getState() {return &(this->state);}
std::shared_ptr<Node> Agent::getStartingNode() {return this->startingNode;}
std::shared_ptr<Node> Agent::getPrimaryGoal() {return this->primaryGoal;}
std::vector<std::shared_ptr<Node>>* Agent::getSecondaryGoals() {return &(this->secondaryGoals);}
int Agent::getVision() {return this->vision;}
std::vector<double>* Agent::getGoalRanges() {return &(this->goalRanges);}

// * Debugging Functions:
void Agent::printAgentInfo() {
    if (LOGGING_LEVEL > 3) {
        DEBUG << "Agent Internal State: ";
        printDoubleVector(getState());
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

        DEBUG << "Vision Range: " << this->vision << ENDL;

        DEBUG << "Secondary Goal Ranges: ";
        printDoubleVector(getGoalRanges());
        NEWL; 
    }
}