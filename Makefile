# A simple makefile for CSE 100 PA4

CC=g++
CXXFLAGS=-std=c++11
LDFLAGS=

# if passed "type=opt" at command-line, compile with "-O3" flag (otherwise use "-g" for debugging)

ifeq ($(type),opt)
    CPPFLAGS += -g
    LDFLAGS += -g
else
    CPPFLAGS += -O3
    LDFLAGS += -O3
endif

all: pathfinder linkpredictor awardsceremony

# include what ever source code *.hpp files pathfinder relies on (these are merely the ones that were used in the solution)

awardsceremony: ActorGraph.o

linkpredictor: ActorGraph.o

pathfinder: ActorGraph.o

test: test.o ActorGraph.o

test2: test2.o ActorGraph.o

test3: test3.o ActorGraph.o

# include what ever source code *.hpp files ActorGraph relies on (these are merely the ones that were used in the solution)

# Note: you do not have to include a *.cpp file if it aleady has a paired *.hpp file that is already included with class/method headers

ActorGraph.o: ActorGraph.hpp Node.hpp

test.o: test.cpp ActorGraph.hpp Node.hpp Edge.hpp

test2.o: test2.cpp ActorGraph.hpp Node.hpp Edge.hpp

test3.o: test3.cpp ActorGraph.hpp Node.hpp Edge.hpp

clean:
	rm -f pathfinder linkpredictor awardsceremony *.o core*

