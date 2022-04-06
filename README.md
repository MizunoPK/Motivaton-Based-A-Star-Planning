# Robot-Planning-FP
Individual Tasks:
Emil - Visualization of a Graph 
Matt - Generating Input Files
Kai - Bare bones A*

Task Structure / TimeLine:
1) Implement Basic A* Algorithm && Visualization && Check Outputs
2) Integrate A* Algorithm with our Structures
3) Add Secondary Goal Functionality

Check Outputs:
- Total Runtime
- Number of Goals Reached
- Number of Nodes Traversed

Input File(s) Structure:
1) NodeDefinition:
   1) Node | [internal state] | [change value]
2) AdjaceniesDefinition:
   1) Node | [{Adjacent Node},{Weight}] or NULL if there are no adjacent nodes
3) AgentDefinition:
   1) [Starting Internal State]
   2) Starting Node
   3) Primary Goal
   4) [Secondary Goal(s)] or NULL if there are no secondary goals

#Classes:
##Simulation
Primary Goal
1) Data Structures:
- List of Secondary Goals
2) Integrated Classes:
- State Space
- Agent
3) Methods:
- runSearch()
- outputPath()
##StateSpace
1) Data Structures:
- HashTable: AdjacencyList
2) Methods:
- lookup(Node)
##Node
Node Name
1) Data Structures:
- List of Internal State(s)
- List of Change Values
##Agent
1) Data Structures:
- List of Internal State(s)
2) Methods:
- getState()
- setState()