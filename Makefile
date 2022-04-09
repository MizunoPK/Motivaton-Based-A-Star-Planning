#
# Makefile
# 

CXX = g++
LD = g++
CXXFLAGS = -g -std=c++17 -DBOOST_LOG_DYN_LINK
LDFLAGS = -g


TARGET = dynamic-pathing
OBJ_FILES = bin/main.o bin/Simulation.o bin/StateSpace.o bin/Node.o bin/Agent.o bin/util.o
INC_FILES = $(wildcard src/*.h)


#
# Any libraries we might n/home/muleeed.
#
LIBRARYS = -lboost_log_setup -lboost_log -lboost_thread -lboost_system  -lpthread

${TARGET}: ${OBJ_FILES}
	${LD} ${LDFLAGS} ${OBJ_FILES} -o $@ ${LIBRARYS}

bin/%.o : src/%.cpp ${INC_FILES}
	${CXX} -c ${CXXFLAGS} -o $@ $<

clean:
	rm -f core ${TARGET} ${OBJ_FILES}

basic1:
	./dynamic-pathing -v inputs/basic1/basic1-nodes.txt inputs/basic1/basic1-adjacencies.txt inputs/basic1/basic1-agent.txt