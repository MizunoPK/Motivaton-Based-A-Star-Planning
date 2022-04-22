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
1) GraphDefinition:
   1) <Min State Value>,<Max State Value>
   2) <Min X Value>,<Max X Value>
   3) <Min Y Value>,<Max Y Value>
   4...) <x>,<y> <internal state> <change value>
3) AgentDefinition:
   1) <Starting Internal State>
   2) Starting Coordinate
   3) Primary Goal Coordinate
   4) <Secondary Goal(s)> or NULL if there are no secondary goals
   5) <Scaler Vision Value>
   6) <State 1 Threshold>,<State 2 Threshold>,...,<State n Threshold>

Output File Structure:
<x,y> <agent internal state> <potential path from this node to the primary goal> <chosen goal node*>

*the s-goal used to make the path, or the p-goal if no s-goal was chosen