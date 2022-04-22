
# This script generates a graph txt file to be parsed for robot planning final project

# GraphGenerator currently generates 2D graph only

import sys
import random

class Node:
    def __init__(self, i, j, MaxWeight, GraphDim):
        self.coordinates = "\n" + str(i) + "," + str(j) + " "
        self.Weights = ""
        self.AgentUpdateValues = "(0,0,0)"
        self.GraphDim = GraphDim
        self.MaxWeight = MaxWeight
        self.i = i
        self.j = j
    def GenerateWeights(self):
        di = float(self.MaxWeight) / float(self.GraphDim[0])
        dj = float(self.MaxWeight) / float(self.GraphDim[1])

        #w1 goes linearly with i (left to right)
        w1 = round(di * self.i, 3)
        #w2 goes linearly with j (up and down)
        w2 = round(dj * self.j, 3)
        #w3 goes linearly with the average of i and j (diagonal)
        w3 = round((w1 + w2) / 2.0, 3)

        mu = 0
        sigma = 1 / (self.MaxWeight*2)

        self.Weights = "" + str(w1) + "," + str(w2) + "," + str(w3) + " "
        self.AgentUpdateValues = "" + str( round(w1/10.0 + random.gauss(mu, sigma),3)) + "," + str( round(w2/10.0 + random.gauss(mu, sigma),3)) + "," + str( round(w3/10.0 + random.gauss(mu, sigma),3)) + ""

    # def GenerateAgentUpdate(self:)


    def MakeNode(self):
        # self.GenerateAgentUpdate()
        self.GenerateWeights()
        return self.coordinates + self.Weights + self.AgentUpdateValues


class GraphGenerator:

    def __init__(self, MinMaxWeight, WeightDim, GraphDim1, GraphDim2):
        # self.MinWeight = MinMaxWeight[0]
        # self.MaxWeight = MinMaxWeight[1]
        self.MaxWeight = MinMaxWeight
        self.WeightDim = WeightDim
        self.GraphDim1 = GraphDim1
        self.GraphDim2 = GraphDim2
        self.Lines = []
        self.f = open('InputGraph.txt', 'w')

    def GenerateLines(self):

        # Add Graph Dimensions to top of file
        Line = str(GraphDim1[0])+","+str(GraphDim1[1])
        self.Lines.append(Line)
        Line = "\n"+str(GraphDim2[0])+","+str(GraphDim2[1])
        self.Lines.append(Line)
        
        
        GraphSpans = [GraphDim1[1]-GraphDim1[0], GraphDim2[1]-GraphDim2[0]]
        
        for i in range(self.GraphDim1[0], self.GraphDim1[1], 1):
            for j in range(self.GraphDim2[0], self.GraphDim2[1], 1):
                ThisNode = Node(i, j, self.MaxWeight, GraphSpans)
                self.Lines.append(ThisNode.MakeNode())

    def GenerateGraph(self):
        self.GenerateLines()
        self.f.writelines(self.Lines)
        self.f.close()

# Output to graph 
# State Bounds (min, max)
# Graph Bounds (minDim1, maxDim1) \n (minDim2, maxDim2)
# Graph type
# WeightBounds = [float(sys.argv[0]), float(sys.argv[1]) ]
# graphBounds = [sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5] ]
# graphType = str(sys.argv[6])


# GraphGenerator starts with (List (state weight bounds), List())
MaxWeight = 10
WeightDim = 3
GraphDim1 = (0,10)
GraphDim2 = (-5, 20)

InputParameters = sys.argv[0]
InputGraph = GraphGenerator(MaxWeight, WeightDim, GraphDim1, GraphDim2)
InputGraph.GenerateGraph()


# AgentGenerator 