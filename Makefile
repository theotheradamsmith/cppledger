CC = gcc
CXX = g++

BIN = dollars
CFLAGS = -g -std=c++11 -Wall -O2
LIB = -pthread -ldl -lwthttp -lwt -lboost_signals

SRC = main.cpp test.cpp database.cpp account.cpp
SRC += estate.cpp menu_item.cpp content_box.cpp account_display.cpp

all: $(BIN)

dollars: $(SRC) sqlite3.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LIB)

sqlite3.o: sqlite3.c
	$(CC) -o $@ -c $^

clean:
	rm -f $(BIN)

testclean: clean
	rm -f *.db

.PHONY: all, clean
