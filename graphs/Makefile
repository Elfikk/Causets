GRAPH_SRCS=directed_graph.cpp
GRAPH_OBJS=directed_graph.o

all: build_dir directed_graph.o

build_dir:
	mkdir $(MAKEDIR)/$(BUILDDIR)/graphs

directed_graph.o: directed_graph.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $(MAKEDIR)/$(BUILDDIR)/graphs/directed_graph.o

clean:
	rm -rf $(BUILDDIR)/graphs/$(GRAPH_OBJS)
