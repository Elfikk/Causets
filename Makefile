SRCS=main.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

MAKEDIR = $(PWD)
BUILDDIR = build

CXXFLAGS = -I$(MAKEDIR) -L$(MAKEDIR)/$(BUILDDIR) -Wall -g

TARGET = main

all: build_dir test

build_dir:
	mkdir -p $(BUILDDIR)

test: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(MAKEDIR)/$(BUILDDIR)/$(TARGET) $(MAKEDIR)/$(BUILDDIR)/$(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $(MAKEDIR)/$(BUILDDIR)/$@

clean:
	rm -rf $(BUILDDIR)/$(TARGET) $(BUILDDIR)/$(OBJS)

run:
	./main
