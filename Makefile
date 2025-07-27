SRCS=Graphs/DirectedGraph.cpp main.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

CXXFLAGS = -Wall -g

TARGET = main

all: test

test: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
