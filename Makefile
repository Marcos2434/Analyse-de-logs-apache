CC = g++
CFLAGS = -g -Wall -std=c++11 # -DMAP

all: analog

analog: analog.o Analyzer.o
	$(CC) $(CFLAGS) -o analog analog.o Analyzer.o

analog.o: Analyzer.o analog.cpp Analyzer.h
	$(CC) $(CFLAGS) -c analog.cpp -o analog.o 

Analyzer.o: Analyzer.cpp Analyzer.h
	$(CC) $(CFLAGS) -c Analyzer.cpp

run: analog
	./analog

clean:
	rm -f analog *.o 