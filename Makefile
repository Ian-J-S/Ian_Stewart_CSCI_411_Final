# Ian Stewart
# Makefile for CSCI411 Final Project 
# Fall 2024, CSU Chico

CFLAGS = -std=c++17 -lsndfile -o
SRC1 = fft.cpp
SRC2 = wavegen.cpp

EXE1 = fft
EXE2 = wavegen

all: $(EXE1) $(EXE2)

$(EXE1) : $(SRC1)
	g++ $(SRC1) $(CFLAGS) $(EXE1)

$(EXE2) : $(SRC2)
	g++ $(SRC2) $(CFLAGS) $(EXE2)

.PHONY:
	clean
	debug

clean:
	rm -rf $(EXE1) $(EXE2)

debug:
	g++ $(SRC1) -g $(CFLAGS) $(EXE1)
	g++ $(SRC2) -g $(CFLAGS) $(EXE2)
