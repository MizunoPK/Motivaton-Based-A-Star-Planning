#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>

class Node {
private:
    std::vector<int> coord; // The coordinate of this node 
    std::vector<int> state; // The internal state associated with this node 
    std::vector<int> modifiers; // The value modifiers that will be applied to the agent after going to this node
    
public:
    // Function: Constructor 
    // Description: Initializes the information associated with this node 
    Node(std::vector<int> coord, std::vector<int> state, std::vector<int> modifiers);

    // Getters
    std::vector<int> getCoord();
    std::vector<int> getState();
    std::vector<int> getModifiers();
};


#endif //NODE_H
