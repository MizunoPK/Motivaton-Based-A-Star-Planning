#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>

class Node {
private:
    std::vector<int> coord; // The coordinate of this node 
    std::vector<double> state; // The internal state associated with this node 
    std::vector<double> modifiers; // The value modifiers that will be applied to the agent after going to this node
    bool canChangeAgent;
    bool traversed; // whether or not the path has already traversed to this node
    
public:
    // Function: Constructor 
    // Description: Initializes the information associated with this node 
    Node(std::vector<int>& coord, std::vector<double>& state, std::vector<double>& modifiers);

    // Getters
    std::vector<int>* getCoord();
    std::vector<double>* getState();
    std::vector<double>* getModifiers();
    bool getCanChangeAgent();
    bool getTraversed();

    // Setters
    void setTraversed(bool traversed);
};


#endif //NODE_H
