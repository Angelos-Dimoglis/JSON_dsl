# name: Angelos Titos Dimoglis
# AM: csd5078

# compiler related variables
CC = g++
CFLAGS = -g -std=c++20

all: main.out

# Rule to compile any .c file into a .out file with the same name
%.out: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.out
