#ifndef AGENT_H
#define AGENT_H

#include <string>
#include <vector>
#include "Node.h"

class Agent {
private:
    std::vector<int> state; // The internal state of the agent
    Node* startingNode;
    Node* primaryGoal;
    std::vector<Node*> secondaryGoals;
    
public:
    // Constructor
    Agent(std::vector<int> state, Node* startingNode, Node* primaryGoal, std::vector<Node*> secondaryGoals);

    // Getters
    std::vector<int> getState();
    Node* getStartingNode();
    Node* getPrimaryGoal();
    std::vector<Node*> getSecondaryGoals();

    // Debugging Functions:
    void printAgentInfo();
};


#endif //AGENT_H
