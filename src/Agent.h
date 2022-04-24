#ifndef AGENT_H
#define AGENT_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_set>
#include "Node.h"

class Agent {
private:
    std::vector<double> state; // The internal state of the agent
    std::shared_ptr<Node> startingNode;
    std::shared_ptr<Node> primaryGoal;
    std::vector<std::shared_ptr<Node>> secondaryGoals;
    int vision;
    std::vector<double> goalRanges; // The range around each state that the agent is willing to accept when looking at s-goals
    
public:
    // Constructor
    Agent(std::vector<double>& state, std::shared_ptr<Node> startingNode, std::shared_ptr<Node> primaryGoal, std::vector<std::shared_ptr<Node>>& secondaryGoals, int vision, std::vector<double>& goalRanges);

    // * Function: updateState
    // Description: Updates the internal state of the agent reflecting the provided change values
    void updateState(std::vector<double>* modifiers, std::vector<double>* stateBounds);

    // * Function: isSecondaryGoal
    // Description: Check if the given node is a secondary goal. Return true if so, false if not
    bool isSecondaryGoal(std::shared_ptr<Node> node);

    // * Function: deleteSecondaryGoal
    // Description: If the given node is a secondary goal, it will be deleted from the list of s-goals
    // Input: The node to check/delete
    // Output: True if we deleted an s-goal, false if not
    bool deleteSecondaryGoal(std::shared_ptr<Node> node);

    // Getters
    std::vector<double>* getState();
    std::shared_ptr<Node> getStartingNode();
    std::shared_ptr<Node> getPrimaryGoal();
    std::vector<std::shared_ptr<Node>>* getSecondaryGoals();
    int getVision();
    std::vector<double>* getGoalRanges();


    // Debugging Functions:
    void printAgentInfo();
};


#endif //AGENT_H
