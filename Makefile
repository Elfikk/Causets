SRCS=main.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

INCLUDE_DIR_NAMES = graphs utils
INCLUDE_DIRS = $(patsubst %, $(MAKEDIR)/%, $(INCLUDE_DIR_NAMES))
INCLUDE_DIR_FLAG = $(patsubst %, -I%, $(INCLUDE_DIRS))
LINKER_DIR_FLAG = $(patsubst %, -L$(MAKEDIR)/$(BUILDDIR)/%, $(INCLUDE_DIR_NAMES))

INCLUDE_SRCS = $(wildcard $(patsubst %, %/*.cpp, $(INCLUDE_DIR_NAMES)))
INCLUDE_OBJS = $(subst .cpp,.o,$(INCLUDE_SRCS))
INCLUDE_BUILD_OBJS = $(patsubst %, $(MAKEDIR)/$(BUILDDIR)/%,$(patsubst $(MAKEDIR)/%, %, $(INCLUDE_OBJS)))

export MAKEDIR = $(PWD)
export BUILDDIR = build

export CXXFLAGS = -I$(MAKEDIR) $(INCLUDE_DIR_FLAG) -L$(MAKEDIR)/$(BUILDDIR) $(LINKER_DIR_FLAG) -Wall -g

TARGET = main

all: clean build_dir graphs utils test

graphs:
	$(MAKE) -C graphs

utils:
	$(MAKE) -C utils

build_dir:
	mkdir -p $(BUILDDIR)

test: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(MAKEDIR)/$(BUILDDIR)/$(TARGET) $(INCLUDE_BUILD_OBJS) $(MAKEDIR)/$(BUILDDIR)/$(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $(MAKEDIR)/$(BUILDDIR)/$@

clean:
	rm -rf $(BUILDDIR)

run:
	./build/main

.PHONY: graphs utils
