CC=clang++
CFLAGS=-std=c++20 -Wall -Wextra -g

SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)

TARGET=main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)