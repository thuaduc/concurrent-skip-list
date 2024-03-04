CXX = clang++ -std=c++20
CFLAGS = -Wall -Wextra -c -O3

SRCDIR = src/
INCDIR = inc/
OBJDIR = obj/
TESTDIR = test/
BINDIR = bin/

OBJS = $(addprefix $(OBJDIR), concurrentSkipList.o node.o)
INCLUDE_FLAGS = $(addprefix -I, $(INCDIR))

.PHONY: all clean main

all: main

main: $(BINDIR)csl.a
	$(CXX) -Wall -Wextra -g -O2 -o $@ main.cpp $^

$(BINDIR)csl.a: $(OBJS)
	ar rcs $@ $^

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CXX) $(CFLAGS) $(INCLUDE_FLAGS) $< -o $@

clean:
	rm -f $(OBJDIR)* $(BINDIR)* main 
	rm -r main.dSYM

