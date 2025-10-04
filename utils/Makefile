UTILS_SRCS=graph_dump.cpp
UTILS_OBJS=graph_dump.o

all: build_dir graph_dump.o

build_dir:
	mkdir $(MAKEDIR)/$(BUILDDIR)/utils

graph_dump.o: graph_dump.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $(MAKEDIR)/$(BUILDDIR)/utils/graph_dump.o

clean:
	rm -rf $(BUILDDIR)/utils/$(UTILS_OBJS)
