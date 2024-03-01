CXX = clang++ -std=c++20
CFLAGS = -Wall -Wextra -c -O3 -g

SRCDIR = src/
INCDIR = inc/
OBJDIR = obj/
TESTDIR = test/
BINDIR = bin/

OBJS = $(addprefix $(OBJDIR), concurrentSkipList.o node.o)
INCLUDE_FLAGS = -I$(INCDIR)

.PHONY: all clean main

all: $(BINDIR)csl.a

$(BINDIR)csl.a: $(OBJS)
	ar rcs $@ $^

$(OBJDIR)%.o:
	$(CXX) $(CFLAGS) $(INCLUDE_FLAGS) $< -o $@

main: $(BINDIR)csl.a
	$(CXX) -Wall -Wextra -O3 -g -o $@ main.cpp $< 

clean:
	rm -f $(OBJDIR)*

