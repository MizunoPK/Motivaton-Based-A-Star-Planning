
# This script generates a graph txt file to be parsed for robot planning final project

# GraphGenerator currently generates 2D graph only

import sys
import random
import math


def translate(value, leftMin, leftMax, rightMin, rightMax):
    # Figure out how 'wide' each range is
    leftSpan = leftMax - leftMin
    rightSpan = rightMax - rightMin

    # Convert the left range into a 0-1 range (float)
    valueScaled = float(value - leftMin) / float(leftSpan)

    # Convert the 0-1 range into a value in the right range.
    return rightMin + (valueScaled * rightSpan)

class Node:
    def __init__(self, i, j, MinMaxWeight, GraphSpans, MinDims, MaxDims, IsMizunoAlgorithm):
        self.coordinates = "\n" + str(i) + "," + str(j) + " "
        self.Weights = ""
        self.AgentUpdateValues = ""
        self.GraphSpans = GraphSpans
        self.MinMaxWeight = MinMaxWeight
        self.WeightSpans = abs(self.MinMaxWeight[1]-self.MinMaxWeight[0])
        self.i = i
        self.j = j
        self.MinDims = MinDims
        self.MaxDims = MaxDims
        self.IsMizunoAlgo = int(IsMizunoAlgorithm)
    def GenerateWeights(self):
        if(self.IsMizunoAlgo != 0):
            #w1 goes linearly with i (left to right)
            
            w1 = round( translate(self.i, self.MinDims[0] , self.MaxDims[0] ,self.MinMaxWeight[0], self.MinMaxWeight[1]), 3)
            # #w2 goes linearly with j (up and down)
            w2 = round( translate(self.j, self.MinDims[1] , self.MaxDims[1] ,self.MinMaxWeight[0], self.MinMaxWeight[1]), 3)
            #w3 goes linearly with the average of i and j (diagonal)
            w3 = round((w1 + w2) / 2.0, 3)

            mu = 0
            sigma = 1 / (self.WeightSpans*2)

            self.Weights = "" + str(w1) + "," + str(w2) + "," + str(w3) + " "
            self.AgentUpdateValues = "" + str( round(w1/10.0 + random.gauss(mu, sigma),3)) + "," + str( round(w2/10.0 + random.gauss(mu, sigma),3)) + "," + str( round(w3/10.0 + random.gauss(mu, sigma),3)) + ""
        else:
            self.Weights = "" + str(0) + "," + str(0) + "," + str(0) + " "
            self.AgentUpdateValues = "" + str(0) + "," + str(0) + "," + str( 0) + ""


    def MakeNode(self):
        # self.GenerateAgentUpdate()
        self.GenerateWeights()
        return self.coordinates + self.Weights + self.AgentUpdateValues

class GraphGenerator:

    # def __init__(self, MinMaxWeight, WeightDim, GraphDim1, GraphDim2):
    def __init__(self, InputParams):
        # self.MinWeight = MinMaxWeight[0]
        # self.MaxWeight = MinMaxWeight[1]
        self.MinMaxWeight = InputParams.MinMaxWeight
        self.WeightDim = InputParams.WeightDim
        self.GraphDim1 = InputParams.GraphDim1
        self.GraphDim2 = InputParams.GraphDim2
        self.Lines = []
        self.f = open('./inputs/InputGraph.txt', 'w')
        self.NumberOfWalls = int(InputParams.NumberOfWalls)
        self.IsMizunoAlgorithm = InputParams.IsMizunoAlgorithm
        self.ScalarVision = InputParams.ScalarVision

    def GenerateLines(self):
        # Add Min,Max State weights to top of file
        Line = str(self.MinMaxWeight[0])+","+str(self.MinMaxWeight[1])
        self.Lines.append(Line)
        # Add Graph Dimensions to top of file
        Line = "\n"+str(self.GraphDim1[0])+","+str(self.GraphDim1[1])
        self.Lines.append(Line)
        Line = "\n"+str(self.GraphDim2[0])+","+str(self.GraphDim2[1])
        self.Lines.append(Line)
        
        
        GraphSpans = [self.GraphDim1[1]-self.GraphDim1[0], self.GraphDim2[1]-self.GraphDim2[0]]
        
        for i in range(self.GraphDim1[0], self.GraphDim1[1]+1, 1):
            for j in range(self.GraphDim2[0], self.GraphDim2[1]+1, 1):
                MaxDims = [self.GraphDim1[1], self.GraphDim2[1]]
                ThisNode = Node(i, j, self.MinMaxWeight, GraphSpans, [self.GraphDim1[0], self.GraphDim2[0]], MaxDims, self.IsMizunoAlgorithm)
                self.Lines.append(ThisNode.MakeNode())

    def GenerateGraph(self):
        self.GenerateLines()
        self.GenerateObstacles()
        try:
            while True:
                self.Lines.remove("\nObstacle")
        except ValueError:
            pass
        # Now all the information need to create the Agent is available
        Agent = AgentGenerator(self.MinMaxWeight, self.GraphDim1, self.GraphDim2, self.ScalarVision, self.Lines)
        Agent.GenerateSecondaryGoals()
        Agent.WriteAgent()
        self.f.writelines(self.Lines)
        self.f.close()
    def GenerateObstacles(self):
        ObstacleDensity = .12
        GraphSpans = [self.GraphDim1[1]-self.GraphDim1[0], self.GraphDim2[1]-self.GraphDim2[0]]
        MaxObstacles = ObstacleDensity * (float(GraphSpans[0])*float(GraphSpans[1]))
        # NodeRange = len(self.Lines)
        WallLength = int(MaxObstacles / float(self.NumberOfWalls))
        # Create n walls
        for i in range(self.NumberOfWalls):
            # We never want to take the first or last nodes because those are the beginning and end
            lastNode = int((random.uniform(4, len(self.Lines)-2)))
            # of length WallLength
            for j in range(WallLength):
                # Label randomNode as Obstacle to be deleted 
                # print(self.Lines[lastNode] )
                self.Lines[lastNode] = "\nObstacle"
                # print(self.Lines[lastNode] )
                next = int(random.uniform(0,4))
                
                nextNode = self.GetNextNode(next, lastNode)
                
                lastNode = nextNode
        
    def GetNextNode(self, next, lastNode):
        # handle traversing the array to make wall obstacles. the if statements protect
        # the start and finish nodes from being replaced
        GraphSpan = self.GraphDim2[1]-self.GraphDim2[0]
        if(next == 0):
            # go right
            
            if(((lastNode + 1) <= 3) or ((lastNode + 1) >= (len(self.Lines) - 1))):
                return lastNode
            NextNode = lastNode + 1
        elif(next==1):
            # go left
            
            if(((lastNode - 1) <= 3) or ((lastNode - 1) >= (len(self.Lines) - 1))):
                return lastNode
            NextNode = lastNode - 1
        elif(next==2):
            # go up
        
            if(((lastNode - GraphSpan) <= 3) or ((lastNode - GraphSpan) >= (len(self.Lines) - 1))):
                return lastNode
            NextNode = lastNode - GraphSpan
        else:
            # go down
            
            if(((lastNode + GraphSpan) <= 3) or ((lastNode + GraphSpan) >= (len(self.Lines) - 1))):
                return lastNode
            NextNode = lastNode + GraphSpan
        return NextNode

class AgentGenerator():

    
    def __init__(self, MinMaxWeight, GraphDim1, GraphDim2, ScalarVision, Lines):
        self.InitialState = [round(random.uniform(MinMaxWeight[0], MinMaxWeight[1]),3), round(random.uniform(MinMaxWeight[0],MinMaxWeight[1]),3), round(random.uniform(MinMaxWeight[0],MinMaxWeight[1]),3)]
        self.StartCoords = str(GraphDim1[0])+","+str(GraphDim2[0])+"\n"
        self.GoalCoords = str(GraphDim1[1])+","+str(GraphDim2[1])+"\n"
        self.ScalarVision = str(ScalarVision) + "\n"
        self.Thresholds = str(round(self.InitialState[0] / 5,3))+","+str(round(self.InitialState[1] / 5,3))+","+ str(round(self.InitialState[2] / 5,3))
        self.MaxGoals =  int(math.floor(.05 * (GraphDim1[1] - GraphDim1[0])*(GraphDim2[1] - GraphDim2[0])))
        #self.MaxGoals =  3
        self.Lines = Lines
        self.SecondaryGoalNodes = self.GenerateSecondaryGoals()
    def GenerateSecondaryGoals(self):
        secondaryGoals = []
        if self.MaxGoals == 0:
            return "NULL"
        for i in range(self.MaxGoals):
            seed = int(random.uniform(4,len(self.Lines)-2))
            node = self.Lines[seed]
            node = node.split(" ")[0][1:]
            secondaryGoals.append(node)
        strSecondaryGoals = ""
        for goal in secondaryGoals:
            strSecondaryGoals = strSecondaryGoals + goal + "-"
        strSecondaryGoals = strSecondaryGoals[:-1]
        strSecondaryGoals = strSecondaryGoals+"\n"
        return strSecondaryGoals
    def WriteAgent(self):
        outputFile = open("./inputs/InputAgent.txt", 'w')
        OutputLines = []
        self.InitialState = str(self.InitialState[0])+","+str(self.InitialState[1])+","+str(self.InitialState[2]) + "\n"
        OutputLines.append(self.InitialState)
        OutputLines.append(self.StartCoords)
        OutputLines.append(self.GoalCoords)
        OutputLines.append(self.SecondaryGoalNodes)
        OutputLines.append(self.ScalarVision)
        OutputLines.append(self.Thresholds)
        outputFile.writelines(OutputLines)
        outputFile.close()



class InputParamsGenerator():
    def __init__(self, InputParametersFile):
        self.WeightDim = 3
        self.paramFile = open(InputParametersFile, 'rt')
    def GenerateParams(self):
        # This whole section handles filtering out helper text
        # from the input-parameters file and mapping those values
        # that are tuples properly
        Line = self.paramFile.readline()
        Line = Line[Line.find(':')+1:]
        Line = tuple(map(int, Line.split(',')))
        self.MinMaxWeight = (Line[0], Line[1])
        Line = self.paramFile.readline()
        Line = Line[Line.find(':')+1:]
        Line = tuple(map(int, Line.split(',')))
        self.GraphDim1 = (Line[0], Line[1])
        Line = self.paramFile.readline()
        Line = Line[Line.find(':')+1:]
        Line = tuple(map(int, Line.split(',')))
        self.GraphDim2 = (Line[0], Line[1])
        Line = self.paramFile.readline()
        self.NumberOfWalls = Line[Line.find(':')+1:]
        Line = self.paramFile.readline()
        self.IsMizunoAlgorithm = Line[Line.find(':')+1:]
        Line = self.paramFile.readline()
        self.ScalarVision = Line[Line.find(':')+1:]


# GraphGenerator starts with (List (state weight bounds), List())
# MaxWeight = 10
# WeightDim = 3
# GraphDim1 = (0,10)
# GraphDim2 = (-5, 20)

InputParametersFile = sys.argv[1]
InputParams = InputParamsGenerator(InputParametersFile)
InputParams.GenerateParams()

# InputGraph = GraphGenerator(MaxWeight, WeightDim, GraphDim1, GraphDim2)
InputGraph = GraphGenerator(InputParams)
InputGraph.GenerateGraph()



# AgentGenerator 
