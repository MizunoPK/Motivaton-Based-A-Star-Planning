#
# Makefile
# 

CXX = g++
LD = g++
CXXFLAGS = -g -std=c++17
LDFLAGS = 


TARGET = dynamic-pathing
OBJ_FILES = main.o
INC_FILES = $(wildcard src/*.h)


#
# Any libraries we might n/home/muleeed.
#
LIBRARYS =


${TARGET}: ${OBJ_FILES}
	${LD} ${LDFLAGS} ${OBJ_FILES} -o $@ ${LIBRARYS}

%.o : src/%.cpp ${INC_FILES}
	${CXX} -c ${CXXFLAGS} -o $@ $<

clean:
	rm -f core ${TARGET} ${OBJ_FILES}