CXX      := g++
CXXFLAGS := -Wall -Werror -O2 -std=c++17
LDFLAGS  := -pthread

SRC      := main.cc
OBJ      := $(SRC:.cc=.o)
EXE      := thread

.PHONY: all clean

# first rule
$(EXE): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# compile .cc files
%.o: %.cc sort.h
	$(CXX) $(CXXFLAGS) -c $<

all: $(EXE)            # alias (optional)

clean:
	rm -f $(OBJ) $(EXE)
