#!/usr/bin/env python3
# #Imports
from cProfile import label
import os
from matplotlib import pyplot as plt, patches
from matplotlib.collections import PatchCollection 
from matplotlib.patches import Circle
import matplotlib.patches as mpatches
import pandas as pd  
import time
import numpy as np 
#Initialize Lists:
#Output Imports:
pos = []
agentStates = []
path = []
# #Graph Imports:
xminMax = []
yminMax = []
nodes = []
internalStates = []
changeVals =  [] 
# #Agent Imports:
agentStartState = []
startpos = []
primaryGoal = []
secondaryGoals = []
# #General Values (for Plotting):
graphSize = 1
stepCount = 0
currPos = []
goalsR = 0
t = 0
######################################################################################################################
# #Get File Names
outFile = input("Please Enter an Output File (i.e. 'example.txt'): ")
graphFile = input("Please Enter a Graph File (i.e. 'basic1-graph.txt'): ")
agentFile = input("Please Enter an Agent File (i.e. 'basic1-agent.txt'): ")
# ##Read Output File:
os.chdir("..")
os.chdir(os.path.join(os.getcwd(), 'outputs'))
with open(outFile) as o:
    t = o.readline().rstrip() #Reads Time to Traverse 
    for line in o:
        i = 1       
        for word in line.split():
            if(i == 1):
                pos.append(word)
                #print(pos)
            if(i == 2):
                agentStates.append(word)
            if(i == 3):
                path.append(word.split("-"))
            i += 1
######################################################################################################################
os.chdir("..")
os.chdir(os.path.join(os.getcwd(), 'inputs/no-change')) ####ChangeFolder Based on File Location####
with open(agentFile) as a:
    agentStartState = a.readline().rstrip()
    startpos = a.readline().rstrip()
    primaryGoal = a.readline().rstrip()
    secondaryGoals = a.readline().rstrip()
    #Doesn't Read In Scalar Vision Value
    #Doesn't Read In State Thresholds
######################################################################################################################
with open(graphFile) as g:
    xminMax = g.readline().rstrip();
    yminMax = g.readline().rstrip()
    for line in g:
        n = 1
        for word in line.split():
            if(n == 1):
                nodes.append(word)
            if(n == 2):
                internalStates.append(word)
            if(n == 3):
                changeVals.append(word)
            n += 1
        stepCount+=1
os.chdir("../..")
os.chdir(os.path.join(os.getcwd(), 'outputs'))
# # # ###############################################################
# # # ###########################Graphing############################
# # # ###############################################################
# # Determine Graph Size
graphSize += int(xminMax[2]) - int(xminMax[0]);
#graphSize = 5
# Make Empty Data Set Size of Graph
data = np.ones((graphSize, graphSize)) * np.nan
# Make a Figure (Subplot1-Graph, Subplot2-State Table)
plt.ion()
fig, (ax, ax2) = plt.subplots(1, 2, tight_layout=True)
# Set a Title
fig.suptitle("Maze Navigation Visualization", fontsize=20)
# Setting x-axis label and y-axis label
plt.xlabel("Horizontial Motion")
plt.ylabel("Vertical Motion")
# Draw the grid
for x in range(graphSize + 1):
    ax.axhline(x, lw=2, color='k', zorder=5)
    ax.axvline(x, lw=2, color='k', zorder=5)
def setCirclepos(x, y):
    return (x+.5), (y+.5)

# Iterate Through Steps
i = 0
#Set Start State
x,y = setCirclepos(int(startpos[0]), int(startpos[2]))
s=Circle((x, y), .5,color='blue')
ax.add_artist(s)
#Set Goal State
x,y = setCirclepos(int(primaryGoal[0]), int(primaryGoal[2]))
e=Circle((x, y), .5,color='green')
ax.add_artist(e)
#Set Secondary Goals
if(secondaryGoals != 'NULL'):
    print(secondaryGoals)
    for goal in secondaryGoals.split("-"):
        x,y = setCirclepos(int(goal[0]), int(goal[2]))
        e=Circle((x, y), .5,color='orange')
        ax.add_artist(e)
blue_patch = mpatches.Patch(color='blue', label='Start Position')
red_patch = mpatches.Patch(color='red', label='Path')
black_patch = mpatches.Patch(color='black', label='Current/Past Positions')
green_patch = mpatches.Patch(color='green', label='Goal Position')
orange_patch = mpatches.Patch(color='orange', label='Secondary Goals')

for x in pos:
    if(x == primaryGoal):
        goalsR+=1
    if(secondaryGoals != 'NULL'):
        for s in secondaryGoals.split("-"):
            if(x == s):
                goalsR+=1

fig.legend(handles=[blue_patch, red_patch, black_patch, green_patch, orange_patch],loc='lower left', bbox_to_anchor=(0.65, 0.65, 0.5, 0.5))
ax.text(-.5,-.5,('Number of Nodes Traversed: ' + str(len(pos))),horizontalalignment='left',verticalalignment='center')
ax.text(-.5,-.7,('Number of Goals Reached ' + str(goalsR)),horizontalalignment='left',verticalalignment='center')
ax.text(-.5,-.9,('Time to Traverse ' + str(t)),horizontalalignment='left',verticalalignment='center')

df = pd.DataFrame([[str(agentStartState)]], index=[0], columns=['A'])
column_labels=["Agent State"]
row_labels=["Intial State"]
currPos=startpos
currPatches = []
while True:
    #Reset Step:
    if(i==len(agentStates)):
        i = 0
        row_labels=["Initial State"]
        df = pd.DataFrame([[str(agentStartState)]], index=[0], columns=['A'])
        currPos=startpos
        # Drawing updated values
        fig.canvas.draw()
        fig.canvas.flush_events()
        currPatches = []
        ax2.cla()

    # Draw the boxes
    ax.imshow(data, interpolation='none', extent=[0, graphSize, 0, graphSize], zorder=0)
    
    pathPatches = []
    for x in range(len(path[i])):
        temp = path[i][x]
        x,y = setCirclepos(int(temp[0]), int(temp[2]))
        circle = Circle((x, y), .5,color='red')
        pathPatches.append(circle)

    for x in pathPatches:
        ax.add_artist(x)
        
    x,y = setCirclepos(int(currPos[0]), int(currPos[2]))
    current = Circle((x, y), .5,color='black')
    currPatches.append(current)

    for x in currPatches:
        ax.add_artist(x)
    
    ax2.axis('tight')
    ax2.axis('off')
    ax2.table(cellText=df.values,colLabels=column_labels, rowLabels=row_labels,cellLoc='center',loc="center")
    # Make X-axis Y-axis
    ax.set_xticks(np.arange(0, graphSize + 1, 1));
    ax.set_yticks(np.arange(0, graphSize + 1, 1));
    # Update Iteration:
    row_labels.append("Iteration" + str(i))
    df2 = pd.DataFrame([[str(agentStates[i])]], index=[i+1], columns=['A'])
    df = df.append(df2)
    if(len(path[i])==1):
        currPos = path[i][0]
    else:
        currPos = path[i][1] ####
    i += 1
    # Drawing updated values
    fig.canvas.draw()

    fig.canvas.flush_events()

    time.sleep(0.5)
    for x in pathPatches:
        x.remove()
    for x in currPatches:
        x.remove()
    # Drawing updated values
    fig.canvas.draw()
    fig.canvas.flush_events()
    time.sleep(.5)
###################################Draw Weights Info###########################################
# names = ["Hi", "Hello"]
    # # Draw Weights
    # for x in range(1,2):
    #     ax.text(x-.5,x-.5, names[x-1],horizontalalignment='center',verticalalignment='center') #pos plus .5 in x and 1.5 in y
    #     fig.canvas.draw()
    #     fig.canvas.flush_events()
    # plt.text(1.5,1.5,'Hello World !',horizontalalignment='center',verticalalignment='center') #pos plus 1.5 in x and 1.5 in y
    # plt.text(1.5,0.5,'Hello World !',horizontalalignment='center',verticalalignment='center') #pos plus 1.5 in x and .5 in y
    # #
    #ax.text(2-.5,2-.5, names[2-1],horizontalalignment='center',verticalalignment='center') #pos plus .5 in x and 1.5 in y
    