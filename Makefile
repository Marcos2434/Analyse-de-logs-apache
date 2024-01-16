CC = g++
CFLAGS = -g -Wall -std=c++11 # -DMAP
SRC_DIR = ./src
TMP_DIR = ./tmp

all: analog

analog: analog.o
	$(CC) $(CFLAGS) -o analog $(TMP_DIR)/analog.o

analog.o: $(SRC_DIR)/analog.cpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)/analog.cpp -o $(TMP_DIR)/analog.o 

# run: analog
# 	$(SRC_DIR)/analog

clean:
	rm -f analog $(TMP_DIR)/*.o *.o