
# This script generates a graph txt file to be parsed for robot planning final project

# GraphGenerator currently generates 2D graph only

class Node:
    def __init__(self, i, j, MaxWeight, GraphDim):
        self.coordinates = "\n(" + str(i) + "," + str(j) + ") "
        self.Weights = ""
        self.AgentUpdateValues = "(0,0,0)"
        self.GraphDim = GraphDim
        self.MaxWeight = MaxWeight
        self.i = i
        self.j = j
    def GenerateWeights(self):
        di = self.MaxWeight / self.GraphDim[0]
        dj = self.MaxWeight / self.GraphDim[1]

        #w1 goes linearly with i (left to right)
        w1 = round(di * self.i, 3)
        #w2 goes linearly with j (up and down)
        w2 = round(dj * self.j, 3)
        #w3 goes linearly with the average of i and j (diagonal)
        w3 = round((w1 + w2) / 2, 3)

        self.Weights = "(" + str(w1) + "," + str(10) + "," + str(w3) + ") "

    # def GenerateAgentUpdate(self:)


    def MakeNode(self):
        # self.GenerateAgentUpdate()
        self.GenerateWeights()
        return self.coordinates + self.Weights + self.AgentUpdateValues


class GraphGenerator:

    def __init__(self, MaxWeight, WeightDim, GraphDim):
        self.MaxWeight = MaxWeight
        self.WeightDim = WeightDim
        self.GraphDim = GraphDim
        self.Lines = []
        self.f = open('InputGraph.txt', 'w')

    def GenerateLines(self):
        for i in range(self.GraphDim[0]):
            for j in range(self.GraphDim[1]):
                ThisNode = Node(i, j, self.MaxWeight, self.GraphDim)
                self.Lines.append(ThisNode.MakeNode())


    def GenerateGraph(self):
        self.GenerateLines()
        self.f.writelines(self.Lines)
        self.f.close()



InputGraph = GraphGenerator(10, 3, (10,30))
InputGraph.GenerateGraph()