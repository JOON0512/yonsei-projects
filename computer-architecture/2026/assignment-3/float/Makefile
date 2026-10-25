CC=g++
CFLAG=-Wall -Werror -g -std=c++17
SRC=$(wildcard *.cc)
HDR=$(wildcard *.h)
OBJ=$(SRC:.cc=.o)
EXE=float
CCV:=$(shell $(CC) --version)

ifeq ($(shell echo '$(CCV)' | grep -qi clang && echo clang),clang)
else ifeq ($(shell echo '$(CCV)' | grep -qi g++ && echo g++),g++)
	CFLAG += -fexcess-precision=16
endif

.PHONY: clean

$(EXE): $(OBJ)
	$(CC) $(CFLAG) -o $@ $(OBJ)

%.o: %.cc $(HDR)
	$(CC) $(CFLAG) -o $@ -c $<

clean:
	rm -f $(OBJ) $(EXE)

