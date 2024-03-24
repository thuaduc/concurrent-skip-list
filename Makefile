SRCDIR = src/
INCDIR = inc/
BINDIR = bin/
APPDIR = app/
GTEST_DIR = googletest/googletest
GTEST_LIB = $(GTEST_DIR)lib/

CXX = clang++ -std=c++20
CFLAGS = -Wall -Wextra -c -O3
LDFLAGS = -L$(GTEST_LIB) -lgtest -lgtest_main -pthread

OBJS = $(addprefix $(BINDIR), concurrentSkipList.o node.o)
INCS = $(addprefix -I, $(INCDIR))

.PHONY: all clean example test

all: example

example: $(BINDIR)csl.a
	$(CXX) -o $@ $(APPDIR)example.cpp $^

test: $(BINDIR)csl.a
	$(CXX) -o $@ $(APPDIR)test.cpp $^ $(LDFLAGS)

$(BINDIR)csl.a: $(OBJS)
	ar rcs $@ $^

$(BINDIR)%.o: $(SRCDIR)%.cpp
	$(CXX) $(CFLAGS) $(INCS) $< -o $@

clean:
	rm -rf $(BINDIR)* example test example.dSYM

