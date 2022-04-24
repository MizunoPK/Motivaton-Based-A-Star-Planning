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

basic1:
	./dynamic-pathing -d 5 -g inputs/basic1/basic1-graph.txt -a inputs/basic1/basic1-agent.txt -o outputs/basic1-output.txt > outputs/basic1.log
	python3 ./pythonScripts/VisualizeOutputs.py outputs/basic1-output.txt inputs/basic1/basic1-graph.txt inputs/basic1/basic1-agent.txt
no-change:
	./dynamic-pathing -d 5 -g inputs/no-change/no-change-graph.txt -a inputs/no-change/no-change-agent.txt -o outputs/no-change-output.txt > outputs/no-change.log
	python3 ./pythonScripts/VisualizeOutputs.py outputs/no-change-output.txt inputs/no-change/no-change-graph.txt inputs/no-change/no-change-agent.txt
can-change:
	./dynamic-pathing -d 5 -g inputs/can-change/can-change-graph.txt -a inputs/can-change/can-change-agent.txt -o outputs/can-change-output.txt > outputs/can-change.log
	python3 ./pythonScripts/VisualizeOutputs.py outputs/can-change-output.txt inputs/can-change/can-change-graph.txt inputs/can-change/can-change-agent.txt
s-goals:
	./dynamic-pathing -p -d 5 -g inputs/s-goals/s-goals-graph.txt -a inputs/s-goals/s-goals-agent.txt -o outputs/s-goals-output.txt > outputs/s-goals.log
	# python3 ./pythonScripts/VisualizeOutputs.py outputs/s-goals-output.txt inputs/s-goals/s-goals-graph.txt inputs/s-goals/s-goals-agent.txt
unreachable:
	./dynamic-pathing -d 6 -g inputs/unreachable/unreachable-graph.txt -a inputs/unreachable/unreachable-agent.txt -o outputs/unreachable-output.txt > outputs/unreachable.log
	python3 ./pythonScripts/VisualizeOutputs.py outputs/unreachable-output.txt inputs/unreachable/unreachable-graph.txt inputs/unreachable/unreachable-agent.txt
big-graph:
	python3 ./pythonScripts/GenerateInput.py ./inputs/input-params/input-parameters.txt
	./dynamic-pathing -d 5 -g inputs/big-graph/InputGraph.txt -a inputs/big-graph/InputAgent.txt -o outputs/big-graph-output.txt > outputs/big-graph.log
	python3 ./pythonScripts/VisualizeOutputs.py outputs/big-graph-output.txt inputs/big-graph/InputGraph.txt inputs/big-graph/InputAgent.txt

present:
	./dynamic-pathing -d 7 -g inputs/presentation/InputGraph.txt -a inputs/presentation/InputAgent.txt -o outputs/present-output.txt > outputs/present.log
	# python3 ./pythonScripts/VisualizeOutputs.py outputs/present-output.txt inputs/presentation/InputGraph.txt inputs/presentation/InputAgent.txt
