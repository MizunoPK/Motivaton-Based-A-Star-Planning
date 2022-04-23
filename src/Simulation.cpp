#include "includes.h"
#include "Simulation.h"
#include "util.h"
#include <fstream>
#include <vector>

Simulation::Simulation(std::string graphFile, std::string agentFile, std::string outputPath) {
    FUNCTION_TRACE << "Simulation constructor called" << ENDL;

    initializeStateSpace(graphFile);
    initializeAgent(agentFile);

    this->outputPath = outputPath;

    // Debugging Prints
    this->ss->printNodes();
    this->agent->printAgentInfo();
}

void Simulation::initializeStateSpace(std::string graphFile) {
    FUNCTION_TRACE << "Simulation::initializeStateSpace called" << ENDL;

    // Process the Graph File
    std::fstream graphFileStream(graphFile);
    if (graphFileStream.is_open()) {
        std::string fileLine;

        // Get the state value bounds
        std::getline(graphFileStream, fileLine);
        std::vector<double> state_bounds = splitDoubleList(fileLine);

        // Get the bounds
        std::getline(graphFileStream, fileLine);
        std::vector<int> x_bounds = splitIntList(fileLine);
        std::getline(graphFileStream, fileLine);
        std::vector<int> y_bounds = splitIntList(fileLine);
        this->ss = std::make_shared<StateSpace>(x_bounds, y_bounds, state_bounds);

        this->agentCanChange = false;
        while (graphFileStream) {
            std::getline(graphFileStream, fileLine);
            std::vector<std::string> lineVector = split(fileLine, ' ');

            // Get node coords
            std::string coordsString = lineVector.at(0);
            std::vector<int> coords = splitIntList(coordsString);
            
            // Get node internal state
            std::vector<double> state = splitDoubleList(lineVector.at(1));

            // Get node agent-modifiers
            std::vector<double> modifiers = splitDoubleList(lineVector.at(2));
            // Check if the agent can change
            if ( !agentCanChange ) {
                for ( int i=0; i < modifiers.size(); i++ ) {
                    if ( modifiers.at(i) > 0 ) {
                        agentCanChange = true;
                        break;
                    }
                }
            }

            // Make a node and add it to the state space graph
            ss->setNode(coordsString, std::make_shared<Node>(coords, state, modifiers));
        }
        graphFileStream.close();
    }
    else {
        ERROR << "Could not open Node File \"" << graphFile << "\"" << ENDL;
        exit(-1);
    }
}

void Simulation::initializeAgent(std::string agentFile) {
    FUNCTION_TRACE << "Simulation::initializeAgent called" << ENDL;

    // Process the Agent File
    std::fstream agentFileStream(agentFile);
    if (agentFileStream.is_open()) {
        std::string fileLine;
        // The agent file should always have 4 lines of relevent data
        // First line is actor state:
        std::getline(agentFileStream, fileLine);
        std::vector<double> actorState = splitDoubleList(fileLine);

        // Second Line is Starting Node:
        std::getline(agentFileStream, fileLine);
        std::shared_ptr<Node> startingNode = this->ss->getNode(splitIntList(fileLine));

        // Third Line is Primary Goal:
        std::getline(agentFileStream, fileLine);
        std::shared_ptr<Node> primaryGoal = this->ss->getNode(splitIntList(fileLine));

        // Fourth Line is Secondary Goals:
        std::getline(agentFileStream, fileLine);
        std::vector<std::shared_ptr<Node>> secondaryGoals;
        if (fileLine != "NULL") {
            std::vector<std::string> nodeStrs = split(fileLine, '-');
            for ( int i=0; i < nodeStrs.size(); i++ ) {
                std::shared_ptr<Node> goal = this->ss->getNode(splitIntList(nodeStrs.at(i)));
                secondaryGoals.push_back(goal);
            }
        }

        // Fifth Line is Secondary Goal Vision Range
        std::getline(agentFileStream, fileLine);
        int vision = stoi(fileLine);

        // Sixth Line is the Secondary Goal State Ranges
        std::getline(agentFileStream, fileLine);
        std::vector<double> goalRanges = splitDoubleList(fileLine);
        
        // Make the agent
        agent = std::make_shared<Agent>(actorState, startingNode, primaryGoal, secondaryGoals, vision, goalRanges);
        agentFileStream.close();
    }
    else {
        ERROR << "Could not open Agent File \"" << agentFile << "\"" << ENDL;
        exit(-1);
    }
}

double Simulation::calculateGCost(std::vector<double>* v1, std::vector<double>* v2) {
    FUNCTION_TRACE << "Simulation::calculateGCost called" << ENDL;

    // Currently, the weight is being calculated based on the difference between each corresponding state
    // Example:
    //      v1=[10, 0] and v2=[3, 3] have a difference vector of [7, 3]
    //      We add up the differences to get a weight of 10
    double weight = 0;
    for ( int i=0; i < v1->size(); i++ ) {
        weight += abs(v1->at(i) - v2->at(i));
    }
    
    // Scale the weight down so it is comparable to the h_cost
    weight *= this->ss->getGCostScale();

    return weight;
}

double Simulation::calculateHCost(std::shared_ptr<Node> n1, std::shared_ptr<Node> n2) {
    FUNCTION_TRACE << "Simulation::calculateHCost called" << ENDL;

    std::vector<int>* n1Coord = n1->getCoord();
    std::vector<int>* n2Coord = n2->getCoord();
    double h_cost = abs(n1Coord->at(0) - n2Coord->at(0)) + abs(n1Coord->at(1) - n2Coord->at(1));

    return h_cost;
}

void Simulation::runSearch() {
    FUNCTION_TRACE << "Simulation::runSearch called" << ENDL;

    // Start measuring time
    start = std::chrono::high_resolution_clock::now();

    TRACE << "Starting Search..." << ENDL;

    // Set the first node to be checked as the agent's start node
    std::shared_ptr<Node> startingNode = this->agent->getStartingNode();

    // Loop until we reach the goal
    while(true) {
        TRACE << "Getting Anticipated Path from Starting Node " 
            << getCoordString(startingNode->getCoord()) << ENDL;

        // Get the path from the new start node to a goal
        // IMPORTANT: this path is stored backwards
        std::vector<std::shared_ptr<Node>> anticipatedPath;
        std::shared_ptr<Node> goalNode = this->getPath(startingNode, anticipatedPath);

        if (anticipatedPath.size() == 0) {
            FATAL << "COULD NOT FIND ANY POTENTIAL PATH TO THE PRIMARY GOAL... EXITING SIMULATION" << ENDL;
            exit(-1); 
        }

        TRACE << "Found a potential path to goal node " << getCoordString(goalNode->getCoord()) << ENDL;

        if ( LOGGING_LEVEL > 4 ) {
            TRACE << "Anticipated Path: ";
            for ( int i = anticipatedPath.size() - 1; i >= 0; i-- ) {
                std::cout << getCoordString(anticipatedPath.at(i)->getCoord()) << " - ";
            }
            NEWL;
        }

        // If the agent cannot change and there are no secondary goals, or we have a vision value of 0... Then don't bother running A* again
        // TODO For the game application, change this to only care about vision
        if ( !this->agentCanChange && 
            (this->agent->getSecondaryGoals()->size() == 0 || this->agent->getVision() == 0 )) {
            INFO << "No need to run A* again... Returning this as the final path" << ENDL;
            // Add all nodes to the final path
            for ( int i = anticipatedPath.size() - 1; i >= 0; i--  ) {
                // Get a vector of the path in question
                auto start = anticipatedPath.begin();
                auto end = anticipatedPath.begin() + i + 1;
                std::vector<std::shared_ptr<Node>> subPath(i+1);
                copy(start, end, subPath.begin());

                this->finalPath.push_back( std::make_shared<FinalPathNode>(anticipatedPath.at(i), *(this->agent->getState()), subPath, goalNode) );
            }

            // Exit the loop
            break;
        }

        // Add the node to the Final Path
        TRACE << "Adding node " << getCoordString(startingNode->getCoord()) << " to final path..." << ENDL;
        this->finalPath.push_back( std::make_shared<FinalPathNode>(startingNode, *(this->agent->getState()), anticipatedPath, goalNode) );
        startingNode->setTraversed(true);

        // if we reached the primary goal, stop looping
        if ( startingNode == this->agent->getPrimaryGoal() ) {
            TRACE << "We have found the goal! Breaking search..." << ENDL;
            break;
        }

        // if we reached a secondary goal, remove the goal from the list of s-goals
        this->agent->deleteSecondaryGoal(startingNode);

        // Get the first node in the path
        // Note: this will be the node second from the end~ because anticipated path is returned backwards and including the startNode
        std::shared_ptr<Node> nextNode = anticipatedPath.at(anticipatedPath.size() - 2);
        
        // if we change the agent's state, perform the change
        if ( nextNode->getCanChangeAgent() ) {
            this->agent->updateState(nextNode->getModifiers(), this->ss->getStateBounds());

            if ( LOGGING_LEVEL > 4 ) {
                TRACE << "Agent state changed... New state: ";
                printDoubleVector(this->agent->getState());
                NEWL;
            }
        }
        startingNode = nextNode;
    }
    //Timing
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    // Output the final path
    outputToFile();
}


std::shared_ptr<Node> Simulation::getPath(std::shared_ptr<Node> startingNode, std::vector<std::shared_ptr<Node>> &anticipatedPath) {
    FUNCTION_TRACE << "Simulation::getPath called" << ENDL;

    DEEP_TRACE << "getPath called... Beginning search for secondary goals..." << ENDL;
    // Get useful info
    std::vector<std::shared_ptr<Node>>* secondaryGoals = this->agent->getSecondaryGoals();
    std::vector<double>* goalRanges = this->agent->getGoalRanges();
    std::vector<double>* agentState = this->agent->getState();
    std::vector<int>* startingNodeCoord = startingNode->getCoord();

    // Compile a list of secondary goals within vision range
    std::vector<std::shared_ptr<SearchNode>> viableGoals;
    for ( int i=0; i < secondaryGoals->size(); i++ ) {
        std::vector<int>* nodeCoord = secondaryGoals->at(i)->getCoord();
        // Currently, the vision is checking all nodes in the circle around the starting node
        // TODO Modify this to have a more limitted range of vision behind the agent
        if ( abs(nodeCoord->at(0) - startingNodeCoord->at(0)) <= this->agent->getVision()
            && abs(nodeCoord->at(1) - startingNodeCoord->at(1)) <= this->agent->getVision()
            && !secondaryGoals->at(i)->getTraversed() ) {
                // Figure out if this goal is worth it
                // Check if the goal is within the agent's threshold range for secondary goals
                bool isViable = true;
                std::vector<double>* goalState = secondaryGoals->at(i)->getState();
                for ( int j=0; j < goalRanges->size(); j++ ) {
                    if ( abs(agentState->at(j) - goalState->at(j)) > goalRanges->at(j) ) {
                        isViable = false;
                        break;
                    }
                }

                // If it is a viable secondary goal, add it to the list
                if ( isViable ) {
                    // Make a SearchNode object we can use to store the g-cost and sort later
                    double g_cost = calculateGCost(agentState, goalState);
                    std::weak_ptr<SearchNode> w;
                    viableGoals.push_back(std::make_shared<SearchNode>(secondaryGoals->at(i), g_cost, g_cost, w));
                }
         }
    }

    // Sort goals by g_costs
    // This will sort from highest cost to lowest cost
    this->quickSort(&viableGoals, 0, viableGoals.size() - 1);

    if ( LOGGING_LEVEL > 4 ) {
        if ( viableGoals.size() > 0 ) {
            TRACE << "Viable Secondary Goals: ";
            for(int i=0; i < viableGoals.size(); i++) {
                TRACE << getCoordString(viableGoals.at(i)->node->getCoord()) << " - ";
            }
            NEWL;
        }
        else {
            TRACE << "No viable goals within vision range" << ENDL;
        }
    }

    // Get the heuristic distance from the start node to primary goal
    // TODO Consider whether to keep using h cost here or compare actual distances
    double pGoalHCost = this->calculateHCost(startingNode, this->agent->getPrimaryGoal());

    // Loop through the potential secondary goals, from most viable to least viable
    // If the primary goal is better than the goal, pursue the primary goal instead
    // If there is a possible path from the start node, to the secondary goal, to the primary goal, then
    //      choose the secondary goal as the temporary goal
    for ( int i = viableGoals.size() - 1; i >= 0; i-- ) {
        // Get the heuristic distance from the start node to this secondary goal
        double sGoalHCost = this->calculateHCost(startingNode, viableGoals.at(i)->node);

        // If the primary goal looks to be closer: just pursue that
        if ( pGoalHCost <= sGoalHCost ) {
            TRACE << "Primary goal is closer than secondary goals... Will pursue primary goal..." << ENDL;
            break;
        }

        // Check if there is a possible path from the start node to primary goal, through the secondary goal
        std::vector<std::shared_ptr<Node>> sGoalPath = runAstar(startingNode, viableGoals.at(i)->node);
        if ( sGoalPath.size() > 0 ) {
            // Temporarily mark the path up to the secondary goal as "traversed", so that we can simulate 
            //      What the path from secondary goal to primary goal may look like
            // Don't set the secondary goal as traversed~ since that will be treated as the next start node
            for ( int j = sGoalPath.size() - 1; j > 0; j-- ) {
                sGoalPath.at(j)->setTraversed(true);
            }

            // Get the path from s-goal to p-goal
            std::vector<std::shared_ptr<Node>> s_to_p_path = runAstar(viableGoals.at(i)->node, this->agent->getPrimaryGoal());

            // Set the nodes in the start -> sgoal path back to being untraversed
            for ( int j = sGoalPath.size() - 1; j > 0; j-- ) {
                sGoalPath.at(j)->setTraversed(false);
            }

            // If a path exists from s-goal to p-goal, select this s-goal as the next destination
            if ( s_to_p_path.size() > 0 ) {
                TRACE << "Found a viable secondary goal (" << getCoordString(viableGoals.at(i)->node->getCoord()) << ")... Returning path from start node to primary goal, that runs through this secondary goal" << ENDL;

                // merge the two paths we found into one path from start node to p-goal
                anticipatedPath = s_to_p_path;
                for ( int i=1; i < sGoalPath.size(); i++  ) {
                    anticipatedPath.push_back(sGoalPath.at(i));
                }

                return viableGoals.at(i)->node;
            }
            // If no path exists, move on and check the next s-goal
            else {
                DEEP_TRACE << "No path exists from secondary node " << getCoordString(viableGoals.at(i)->node->getCoord()) << " to primary goal " << ENDL;
                continue;
            }
        }
        // If we couldn't get to the secondary goal... skip to the next secondary goal
        else {
            DEEP_TRACE << "No path exists from start node " << getCoordString(startingNode->getCoord()) << " to secondary goal " << getCoordString(viableGoals.at(i)->node->getCoord()) << ENDL;
            continue;
        }
    }

    // If we made it this far, then there is no secondary goals to go to
    // So, get the path to the primary goal
    DEEP_TRACE << "No viable secondary goals found... Finding path to primary goal..." << ENDL;
    anticipatedPath = runAstar(startingNode, this->agent->getPrimaryGoal());
    return this->agent->getPrimaryGoal();
}

std::vector<std::shared_ptr<Node>> Simulation::runAstar(std::shared_ptr<Node> startingNode, std::shared_ptr<Node> goalNode) {
    FUNCTION_TRACE << "Simulation::runAstar called" << ENDL;

    // define OPEN - priority queue of nodes ready to be evaluated
    std::vector<std::shared_ptr<SearchNode>> openQueue;

    // define CLOSED - a set of nodes already evaluated
    //               - defined as a map associating a Node -> SearchNode ... 
    //                  Makes it easier to find a SearchNode and ensure only one SearchNode exists per Node
    std::unordered_map<std::shared_ptr<Node>, std::shared_ptr<SearchNode>> closedMap;

    // add the start node to OPEN
    std::weak_ptr<SearchNode> w;
    openQueue.push_back(std::make_shared<SearchNode>(startingNode, 0, 0, w));

    // loop through the search
    while (true) {
        // If there's nothing in the open queue: exit the function
        // This means that there is no possible way to get to the goal
        if ( openQueue.size() == 0 ) {
            DEEP_TRACE << "Open queue is empty... Searching is no longer possible. Exiting A*..." << ENDL;
            return {};
        }

        if ( LOGGING_LEVEL > 5 ) {
            std::string openString = "";
            for ( int i = openQueue.size() - 1; i >= 0; i-- ) {
                openString = openString + "(" + getCoordString(openQueue.at(i)->node->getCoord()) + ") ";
            }
            DEEP_TRACE << "Current OPEN queue: " << openString << ENDL;
        }

        std::shared_ptr<SearchNode> current = openQueue.back(); // current = node in OPEN with the lowest f_cost
        openQueue.pop_back(); // remove current from OPEN
        closedMap[current->node] = current; // add current to CLOSED
        bool sortOPEN = false; // if we make any changes that need us to re-sort open, update this flag

        DEEP_TRACE << "Popped node " << getCoordString(current->node->getCoord()) << " from OPEN" << ENDL;

        // if current is the target node - the path has been found
        if ( current->node == goalNode ) {
            DEEP_TRACE << "PRIMARY GOAL FOUND... ENDING SEARCH" << ENDL;
            break;
        }
        
        // foreach neighbor of the current node
        std::vector<std::shared_ptr<Node>> neighbors = this->ss->getAdjacencyList(current->node);
        for ( int i=0; i < neighbors.size(); i++ ) {
            std::shared_ptr<Node> neighbor = neighbors.at(i);
            DEEP_TRACE << "Checking Neighbor: " << getCoordString(neighbor->getCoord()) << ENDL;
            // if neighbor is in CLOSED
            if ( closedMap.find(neighbor) != closedMap.end() ) {
                // skip to the next neighbor
                DEEP_TRACE << "This neighbor is already in CLOSED... Skipping..." << ENDL;
                continue;
            }
            // if neighbor has already been traversed
            if ( neighbor->getTraversed() ) {
                DEEP_TRACE << "Neighbor has already been traversed to... Skipping..." << ENDL;
                continue;
            }

            // Calculate the cost of the path to this neighbor
            // g_cost = distance from starting node
            double g_cost = current->g_cost + calculateGCost(agent->getState(), neighbor->getState());
            DEEP_TRACE << "G Cost: " << g_cost << ENDL;
            // h_cost = heuristic calculated distance from end node - manhatten distance
            double h_cost = this->calculateHCost(neighbor, goalNode);
            DEEP_TRACE << "H Cost: " << h_cost << ENDL;
            // f_cost = g_cost + h_cost ... The total cost of the node we use to determine if we want to move there
            double f_cost = h_cost + g_cost;
            DEEP_TRACE << "F Cost: " << f_cost << ENDL;

            // Determine the location of the neighbor in OPEN
            int neighbor_i = -1; // -1 index indiciates the neighbor is not in OPEN
            for ( int j=0; j < openQueue.size(); j++ ) {
                if ( openQueue.at(j)->node == neighbor ) {
                    neighbor_i = j;
                    break;
                }
            }

            // if neighbor is not in OPEN - add it
            if ( neighbor_i == -1 ) {
                openQueue.push_back(std::make_shared<SearchNode>(neighbor, g_cost, f_cost, current));
                sortOPEN = true;
                DEEP_TRACE << "New valid neighbor found... Adding to OPEN" << ENDL;
            }
            // it is in OPEN and the new path to neighbor is shorter - update its f_cost and parent node
            else if (neighbor_i != -1 && f_cost < openQueue.at(neighbor_i)->f_cost) {
                openQueue.at(i)->f_cost = f_cost;
                std::weak_ptr<SearchNode> prevNode = current;
                openQueue.at(i)->prevNode = prevNode;
                sortOPEN = true;
                DEEP_TRACE << "Neighbor already is in OPEN, but we have found a better path to it. Updating the entry in OPEN." << ENDL;
            }
        }
        
        // if internal state changed, or anything in OPEN changed, re-sort OPEN
        if ( sortOPEN ) {
            // Use QUICK sort
            this->quickSort(&openQueue, 0, openQueue.size() - 1);
        }
    }

    // Output the results of the search
    return this->derivePathFromClosed(&closedMap, startingNode, goalNode);
}

void Simulation::quickSort(std::vector<std::shared_ptr<SearchNode>>* vector, int low, int high) {
    FUNCTION_TRACE << "Simulation::quicksort called" << ENDL;
    // TODO: Check for ties on f_costs... If there are ties, then sort based on the g cost... If there are still ties on g_cost, then sort based on the prioritized agent state

    if ( low < high ) {
      /* p is partitioning index, vector[p] is now
        at right place */
      int p = partition(vector, low, high);

      // Separately sort elements before
      // partition and after partition
      quickSort(vector, low, p - 1);
      quickSort(vector, p + 1, high);
   }
}

int Simulation::partition(std::vector<std::shared_ptr<SearchNode>>* vector, int low, int high) {
    double pivot = vector->at(high)->f_cost; // pivot
    int i = (low - 1); // Index of larger element and indicates the right position of pivot found so far
 
    for (int j = low; j < high; j++)
    {
        // If current element is larger than the pivot
        if (vector->at(j)->f_cost > pivot)
        {
            i++; // increment index of larger element
            vector->at(i).swap(vector->at(j));
        }
    }
    vector->at(i + 1).swap(vector->at(high));
    return (i + 1);
}

std::vector<std::shared_ptr<Node>> Simulation::derivePathFromClosed(std::unordered_map<std::shared_ptr<Node>, std::shared_ptr<SearchNode>>* closedMap, std::shared_ptr<Node> startingNode, std::shared_ptr<Node> goalNode) {
    FUNCTION_TRACE << "Simulation::derivePathFromClosed called" << ENDL;
    std::vector<std::shared_ptr<Node>> backwardsPath;
    backwardsPath.push_back(goalNode);
    if ( startingNode != goalNode ) {
        recurseThroughSearchNode(&backwardsPath, (*closedMap)[goalNode]->prevNode, startingNode);
    }
    return backwardsPath;
}

void Simulation::recurseThroughSearchNode(std::vector<std::shared_ptr<Node>>* path, std::weak_ptr<SearchNode> pathNode, std::shared_ptr<Node> startingNode) {
    // Make sure the SearchNode the weak pointer to is pointing to still exists
    if ( auto tempSharedPointer = pathNode.lock() ) {
        path->push_back(tempSharedPointer->node);
        // Recursive Case: There is more to the path to add to the stack
        if ( tempSharedPointer->node != startingNode ) {
            recurseThroughSearchNode(path, tempSharedPointer->prevNode, startingNode);
        }
    }
    else {
        FATAL << "Encountered a weak pointer whose object does not exist!" << ENDL;
        FATAL << "Here's the path that was able to be compiled (Length: " << path->size() << "):" << ENDL;
        for ( int i = path->size() - 1; i >= 0; i-- ) {
            INFO << getCoordString(path->at(i)->getCoord()) << ENDL;
        }
    }
}

void Simulation::outputToFile() {
    FUNCTION_TRACE << "Simulation::outputToFile called" << ENDL;
    TRACE << "Outputting to file: " << this->outputPath << ENDL;

    // Open the file
    std::ofstream outputStream(this->outputPath);

    // Amount of Time Taken
    outputStream <<  elapsed_seconds.count() << "\n";

    // Loop through the final path and output each line
    for ( int i=0; i < this->finalPath.size(); i++ ) {
        std::shared_ptr<FinalPathNode> fpn = this->finalPath.at(i);

        // Start a new line
        if ( i != 0 ) {
            outputStream << "\n";
        }

        // Output the node coordinates
        std::vector<int>* fpnCoords = fpn->node->getCoord();
        outputStream << fpnCoords->at(0) << "," << fpnCoords->at(1) << " ";

        // Output the agent state
        for ( int j=0; j < fpn->agentState.size(); j++ ) {
            outputStream << fpn->agentState.at(j);
            if ( j != fpn->agentState.size() - 1 ) {
                outputStream << ",";
            }
        }
        outputStream << " ";

        // Output the anticipated path
        // Note: anticipated path is stored backwards
        for ( int j = fpn->anticipatedPath.size() - 1; j >= 0; j-- ) {
            std::vector<int>* nodeCoords = fpn->anticipatedPath.at(j)->getCoord();
            outputStream << nodeCoords->at(0) << "," << nodeCoords->at(1);
            if ( j != 0 ) {
                outputStream << "-";
            }
        }

        // Output the goal node
        outputStream << " " << getCoordString(fpn->goalNode->getCoord());
    }

    outputStream.close();
}