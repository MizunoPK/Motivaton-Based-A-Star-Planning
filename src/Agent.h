#ifndef AGENT_H
#define AGENT_H

#include <string>
#include <vector>
#include <memory>
#include "Node.h"

class Agent {
private:
    std::vector<double> state; // The internal state of the agent
    std::shared_ptr<Node> startingNode;
    std::shared_ptr<Node> primaryGoal;
    std::vector<std::shared_ptr<Node>> secondaryGoals;
    
public:
    // Constructor
    Agent(std::vector<double>& state, std::shared_ptr<Node> startingNode, std::shared_ptr<Node> primaryGoal, std::vector<std::shared_ptr<Node>>& secondaryGoals);

    // Getters
    std::vector<double>* getState();
    std::shared_ptr<Node> getStartingNode();
    std::shared_ptr<Node> getPrimaryGoal();
    std::vector<std::shared_ptr<Node>>* getSecondaryGoals();

    // Debugging Functions:
    void printAgentInfo();
};


#endif //AGENT_H
