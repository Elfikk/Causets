SRCS=main.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

export MAKEDIR = $(PWD)
export BUILDDIR = build

export CXXFLAGS = -I$(MAKEDIR) -I$(MAKEDIR)/graphs -L$(MAKEDIR)/$(BUILDDIR) -L$(MAKEDIR)/$(BUILDDIR)/graphs -Wall -g

TARGET = main

all: build_dir dag test

dag:
	$(MAKE) -C graphs

build_dir:
	mkdir -p $(BUILDDIR)

test: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(MAKEDIR)/$(BUILDDIR)/$(TARGET) $(MAKEDIR)/$(BUILDDIR)/$(OBJS) $(MAKEDIR)/$(BUILDDIR)/graphs/directed_graph.o

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $(MAKEDIR)/$(BUILDDIR)/$@

clean:
	rm -rf $(BUILDDIR)

run:
	./build/main
