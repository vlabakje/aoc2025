CXX      := g++
CXXFLAGS := -Wall -Wextra -pedantic -std=c++23 -O2

# All day directories
DAYS := $(wildcard day*/)

# Expand programs: day01/one, day01/two, day02/one, ...
PROGRAMS := $(foreach d,$(DAYS),$(d)one $(d)two)

# Default build
all: $(PROGRAMS)

# Pattern rule: build dayXX/one from dayXX/one.cpp
day%/one: day%/one.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

# Pattern rule: build dayXX/two from dayXX/two.cpp
day%/two: day%/two.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

# Clean all built binaries
clean:
	rm -f day*/one day*/two