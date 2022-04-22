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

no-change:
	./dynamic-pathing -d 5 -g inputs/no-change/no-change-graph.txt -a inputs/no-change/no-change-agent.txt -o outputs/no-change-output.txt > outputs/no-change.log

can-change:
	./dynamic-pathing -d 5 -g inputs/can-change/can-change-graph.txt -a inputs/can-change/can-change-agent.txt -o outputs/can-change-ouput.txt > outputs/can-change.log

s-goals:
	./dynamic-pathing -d 5 -g inputs/s-goals/s-goals-graph.txt -a inputs/s-goals/s-goals-agent.txt -o outputs/s-goals-output.txt > outputs/s-goals.log

unreachable:
	./dynamic-pathing -d 6 -g inputs/unreachable/unreachable-graph.txt -a inputs/unreachable/unreachable-agent.txt -o outputs/unreachable-ouput.txt > outputs/unreachable.log