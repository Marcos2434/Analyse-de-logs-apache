CC = g++
CFLAGS = -g -Wall -std=c++11 # -DMAP
SRC_DIR = ./src
TMP_DIR = ./tmp

all: analog

analog: analog.o Analyzer.o
	$(CC) $(CFLAGS) -o analog $(TMP_DIR)/analog.o $(TMP_DIR)/Analyzer.o

analog.o: Analyzer.o $(SRC_DIR)/analog.cpp $(SRC_DIR)/Analyzer.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/analog.cpp -o $(TMP_DIR)/analog.o 

Analyzer.o: $(SRC_DIR)/Analyzer.cpp $(SRC_DIR)/Analyzer.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/Analyzer.cpp -o $(TMP_DIR)/Analyzer.o

run: analog
	./analog

clean:
	rm -f analog $(TMP_DIR)/*.o *.o