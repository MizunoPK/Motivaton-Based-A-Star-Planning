# A* Path Planner for Dynamic Agents with Internal Motivations
## Project Description:
This repository contains code developed for the final project in the Robot Planning & Manipulation course at Colorado School of Mines. See our final report for a detailed overview of this project.

Many traditional path planners are used to find the shortest trajectory from some beginning state to goal state. This results in a static agent that moves through an environment a certain way every time. This behavior is not sufficient in modeling the behavior of human-like agents with internal motivations. In this project, we have developed an augmented version of A* path planning that allows an agent to determine a path to its goal that best aligns with its internal motivations. The agent has internal states that can be impacted by its environment, follows paths that align with its current internal state, and attempts to accomplish secondary goals that also align with its internal state.

This repository contains the source code of our modified version of A* located in the `./src` folder, written in C++. Additionally, there are two helper Python scripts in the `./pythonScripts` folder. The `GenerateInput.py` script is used to help generate random agents and state spaces to test our algorithm with. The `VisualizeOutputs.py` script is used to make a visualization of the path outputted by our A* algorithm.

# Compiling and Running the Planner
You can compile the planner simply by running `make`. It is compiled using g++ with C++-17 and there are no special libraries that need to be installed. This will compile the source code into an executable called `dynamic-pathing`. You can then run this executable from the command line by providing the required command line arguments.

### ./dynamic-pathing Command Line Arguments:
- `-a <path to agent file>`
   - *REQUIRED~* specify the path to the config file that contains the agent information
- `-g <path to graph file>`
   - *REQUIRED~* specify the path to the config file that contains the state space graph information
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

See the `./outputs` folder for example output files that were generated for their corresponding input files from the `./inputs` folder.

### Example Planner Runs
Here is an example run of the planner:<br>
*Note: the `make 7x7` target mentioned below is a shortcut to running this example command*<br>
`./dynamic-pathing -p -g inputs/7x7/InputGraph.txt -a inputs/7x7/InputAgent.txt -o outputs/7x7-readable-output.txt`

We have also provided Makefile targets to quickly run the our planning algorithm with example inputs. *After* compiling the code, you can run the following make commands to quickly test the planner.
- `make 3x3`
   - This will run the planner on the inputs in the `./inputs/3x3` folder. This is a 3-by-3 graph where the agent has 2 possible secondary goals. This run will create an output file in readable format at `./outputs/3x3-readable-output.txt`
- `make 3x3-visualization`
   - This will run the planner on the same 3x3 input as the previous command, except it creates a machine-readable output file and automatically launches the visualization script using that generated output file.
- `make 7x7`
   - This will run the planner on the inputs in the `./inputs/7x7` folder. These inputs are the ones used to generate the example we show in our presentation and report. It is a 7-by-7 graph with 2 secondary goals. This run will create an output file in readable format at `./outputs/7x7-readable-output.txt`
- `make 7x7-visualization`
   - Just like with the 3x3-visualization target, this target will make a machine-readable output for the 7x7 inputs and automatically send it to the visualization script.

# Using the Python Utility Scripts
## Visualization Script
We have created a python file to aid in visualizing the paths that were generated by our A* simulation. The script using MatPlotLib to make graphics you can click through to see the process the agent went through when navigating the space. The script is located at `./pythonScripts/VisualizeOutputs.py`

This script requires three command line arguments (in this order):
1. `<path to file outputted by the A* algorithm>`
2. `<path to graph file used by the A* algorithm>`
3. `<path to the agent file used by the A* algorithm>`

Here is an example of how to run the visualizer:<br>
`python3 ./pythonScripts/VisualizeOutputs.py outputs/7x7-output.txt inputs/7x7/InputGraph.txt inputs/7x7/InputAgent.txt`

Some important notes:
- This script uses several libraries that you may need to install, such as `matplotlib`, `pandas`, and `numpy`
- The output file from the A* algorithm that needs to be fed to this script must be in the machine-readable format. You get this format by *not* having the `-p` flag when running the `./dynamic-pathing` executable.
- We struggled to get the visualization working on Isengard. We've tested that it works on a native Linux environment though.

## Input File Generator
We have created a python file to aid in generating pseudo-random agents and state spaces to test with our A* simulation. This script is located at `./pythonScripts/GenerateInput.py`

When calling the script, you need to provide a parameters file as a command line argument. This parameters files helps provide some context and structure to base the random generation around. This file is structured like so:
- Line 1: `<Min State Value>,<Max State Value>`
   - (Set the maximum and minimum bounds that state values can possibily be within)
- Line 2: `<Min X Value>,<Max X Value>`
   - (Set the X-value bounds of the traversable graph)
- Line 3: `<Min Y Value>,<Max Y Value>`
   - (Set the Y-value bounds of the traversable graph)
- Line 4: `<Number of Walls>`
   - The script generates obstacles by effectively making paths of walls in the space. Specify how many walls you want here.
- Line 5: `<Is Running Modified A*>`
   - This boolean input (0 or 1), indicate whether we will be running the generated files for a traditional A* run (0) or running with our Modified A* (1)
- Line 6: `<Vision>`
   - Indicate the vision level of the agent

An example parameters file can be seen at `./pythonScripts/input-params/input-parameters.txt`

The script will make an agent file and graph file located at `./inputs/InputAgent.txt` and `./inputs/InputGraph.txt` respectively.

Here is an example run of the Input Generator: <br>
`python3 pythonScripts/GenerateInput.py pythonScripts/input-params/input-parameters.txt`
