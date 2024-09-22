PILOT_NAME = Festnew
MORE_FILE = Astar
OBJ = ./obj/
SRC = ./src/
HEADER = ./include/
BIN = ./bin/
CC = gcc
CFLAGS = -Wall -std=c99 -Wextra  -pedantic -Wpedantic -O3 -I $(HEADER)
LDLIBS = -lm
DRIVERS_PATH = ../drivers

.PHONY: all install clean

all: $(BIN)$(PILOT_NAME) install

install: $(DRIVERS_PATH)/$(PILOT_NAME)

$(DRIVERS_PATH)/$(PILOT_NAME): $(BIN)$(PILOT_NAME)
	cp $(BIN)$(PILOT_NAME) $(DRIVERS_PATH)

$(BIN)$(PILOT_NAME): $(OBJ)$(PILOT_NAME).o $(OBJ)$(MORE_FILE).o $(OBJ)follow_line.o $(OBJ)Queue.o $(OBJ)graph.o $(OBJ)List.o $(OBJ)HashTable.o $(OBJ)DestinationList.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

#VelocityPilote
$(OBJ)$(PILOT_NAME).o: $(SRC)$(PILOT_NAME).c
	$(CC) $(CFLAGS) -I . $< -c -o $@

#follow_line
$(OBJ)follow_line.o: $(SRC)follow_line.c $(HEADER)follow_line.h
	$(CC) $(CFLAGS) -I ./ $< -c -o $@

#Astar
$(OBJ)$(MORE_FILE).o: $(SRC)$(MORE_FILE).c $(HEADER)$(MORE_FILE).h
	$(CC) $(CFLAGS) -I ./ $< -c -o $@

#Queue
$(OBJ)Queue.o: $(SRC)Queue.c $(HEADER)Queue.h
	$(CC) $(CFLAGS) -I ./ $< -c -o $@

#HashTable
$(OBJ)HashTable.o: $(SRC)HashTable.c $(HEADER)HashTable.h
	$(CC) $(CFLAGS) -I ./ $< -c -o $@

#List
$(OBJ)List.o: $(SRC)List.c $(HEADER)List.h
	$(CC) $(CFLAGS) -I ./ $< -c -o $@

#DestinationList
$(OBJ)DestinationList.o: $(SRC)DestinationList.c $(HEADER)DestinationList.h
	$(CC) $(CFLAGS) -I ./ $< -c -o $@

#Graph
$(OBJ)graph.o: $(SRC)graph.c $(HEADER)graph.h
	$(CC) $(CFLAGS) -I ./ $< -c -o $@

disclean:
	rm $(BIN)$(PILOT_NAME)
	rm $(DRIVERS_PATH)/$(PILOT_NAME)

clean:
	rm $(OBJ)*
