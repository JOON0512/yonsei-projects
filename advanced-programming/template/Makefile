# compiler, flags
CXX      := g++
CXXFLAGS := -Wall -Werror -g -std=c++11

# object files list
OBJS     := string.o main.o
TARGET   := vector            # exec file name

# basic rules
# when 'make' or 'make vector', it follows the sequence below
vector: $(OBJS)
	$(CXX) -o $@ $^

# each '.cc' -> '.o'
# %.o pattern:  string.cc -> string.o,  main.cc -> main.o
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)
