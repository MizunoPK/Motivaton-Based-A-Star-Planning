# A* Path Planner for Dynamic Agents with Internal Motivations
## Project Description:
This repository contains code developed for the final project in the Robot Planning & Manipulation course at Colorado School of Mines.

Many efficient path planners are used to find trajectories from some beginning state to goal state. Whether designed for a physical or virtual agent, this creates for a static agent as it interacts with the environment around it. This behavior is not sufficient in modeling the behavior of human-like agents with internal motivations. In this project, we have developed an augmented version of A* path planning that allows an agent to determine a path to its goal that best aligns with its internal motivations. The agent has internal states that can be impacted by its environment, follows paths that align with its current internal state, and attempts to accomplish secondary goals that also align with its internal state.

This repository contains the source code of our modified version of A* located in the `./src` folder. Additionally, there are two helper Python scripts in the `./pythonScripts` folder. The `GenerateInput.py` script is used to help generate random agents and state spaces to test our algorithm with. The `VisualizeOutputs.py` script is used to make a visualization of the path outputted by our A* algorithm.

# Compiling and Running the Planner

### ./dynamic-pathing Command Line Arguments:
- `-a <path to agent file>`
   - *REQUIRED~* specify which file contains the agent information
- `-g <path to graph file>`
   - *REQUIRED~* specify which file contains the state space information
- `-o <path to output file>`
   - *REQUIRED~* specify what file you want to send the output of the search to
- `-p`
   - Optional Flag~ Set this if you want the output file to be in a readable format. This will prevent you from being able to use the output file in the visualization script
- `-d <debug level>`
   - Optional Flag~ The program prints debugging information to the console. The level you set with this flag changes the type of information that gets printed.


### Input Files Structures:

Graph Definition File:
   - Line 1: `<Min State Value>,<Max State Value>`
      - (Set the maximum and minimum bounds that state values can possibily be within)
   - Line 2: `<Min X Value>,<Max X Value>`
      - (Set the X-value bounds of the traversable graph)
   - Line 3: `<Min Y Value>,<Max Y Value>`
      - (Set the Y-value bounds of the traversable graph)
   - Line 4+: `<x>,<y> [internal state] [change values]`
      - (Each line from here on represents a traversable node in the graph. We need each traversable node's coordinate, internal state, and change values. Any nodes that could exist in the graph that are not listed here will be assumed to be untraversable.)

Agent Definition File:
   - Line 1: `[Starting Internal State]`
   - Line 2: `<Starting Coordinate>`
   - Line 3: `<Primary Goal Coordinate>`
   - Line 4: `[Secondary Goal(s)] or NULL if there are no secondary goals`
   - Line 5: `<Scaler Vision Value>`
   - Line 6: `[State Difference Thresholds]`

See the `./inputs` folder for example input files.

### Output File Structure:
This is the structure for the output file that can be sent to the visualization script:
- Line 1: `<Search Runtime>`
- Line 2... `<x,y> <agent internal state> <potential path from this node to the primary goal> <chosen goal node*>`
   - Each line from 2 onward shows a step in the agent's path to the primary goal. It contains the agent's coordinate position, the internal state of the agent after reaching that position, the potential path to the goal it generated from its current position, and which goal it is prioritizing on its potential path. 

Set the `-p` flag in the `./dynamic-pathing` command line arguments to generate a prettier, human-readable format of this output file.

# Using the Python Utility Scripts