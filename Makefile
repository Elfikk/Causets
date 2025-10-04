SRCS=main.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

export MAKEDIR = $(PWD)
export BUILDDIR = build

export CXXFLAGS = -I$(MAKEDIR) -I$(MAKEDIR)/graphs -I$(MAKEDIR)/utils -L$(MAKEDIR)/$(BUILDDIR) -L$(MAKEDIR)/$(BUILDDIR)/graphs -L$(MAKEDIR)/$(BUILDDIR)/utils -Wall -g

TARGET = main

all: clean build_dir graphs utils test

graphs:
	$(MAKE) -C graphs

utils:
	$(MAKE) -C utils

build_dir:
	mkdir -p $(BUILDDIR)

test: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(MAKEDIR)/$(BUILDDIR)/$(TARGET) $(MAKEDIR)/$(BUILDDIR)/graphs/directed_graph.o $(MAKEDIR)/$(BUILDDIR)/utils/graph_dump.o $(MAKEDIR)/$(BUILDDIR)/$(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $(MAKEDIR)/$(BUILDDIR)/$@

clean:
	rm -rf $(BUILDDIR)

run:
	./build/main

.PHONY: graphs utils
