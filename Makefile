#
# Makefile
# 

CXX = g++
LD = g++
CXXFLAGS = -g -std=c++17
LDFLAGS = -g


TARGET = dynamic-pathing
OBJ_FILES = bin/main.o bin/Simulation.o bin/StateSpace.o bin/Node.o bin/Agent.o bin/util.o
INC_FILES = $(wildcard src/*.h)


#
# Any libraries we might n/home/muleeed.
#
LIBRARYS =

${TARGET}: ${OBJ_FILES}
	${LD} ${LDFLAGS} ${OBJ_FILES} -o $@ ${LIBRARYS}

bin/%.o : src/%.cpp ${INC_FILES}
	${CXX} -c ${CXXFLAGS} -o $@ $<

clean:
	rm -f core ${TARGET} ${OBJ_FILES}


3x3:
	./dynamic-pathing -p -g inputs/3x3/3x3-graph.txt -a inputs/3x3/3x3-agent.txt -o outputs/3x3-readable-output.txt
3x3-visualize:
	./dynamic-pathing -g inputs/3x3/3x3-graph.txt -a inputs/3x3/3x3-agent.txt -o outputs/3x3-output.txt
	python3 ./pythonScripts/VisualizeOutputs.py outputs/3x3-output.txt inputs/3x3/3x3-graph.txt inputs/3x3/3x3-agent.txt

8x8:
	./dynamic-pathing -p -g inputs/8x8/InputGraph.txt -a inputs/8x8/InputAgent.txt -o outputs/8x8-readable-output.txt
8x8-visualize:
	./dynamic-pathing -g inputs/8x8/InputGraph.txt -a inputs/8x8/InputAgent.txt -o outputs/8x8-output.txt
	python3 ./pythonScripts/VisualizeOutputs.py outputs/8x8-output.txt inputs/8x8/InputGraph.txt inputs/8x8/InputAgent.txt
