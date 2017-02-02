CC=gcc
CXX=g++

LIB=-pthread -ldl -lwthttp -lwt -lboost_signals
CFLAGS=-g -std=c++11 -Wall -O2
BIN=dollars

all: $(BIN)

dollars: main.cpp test.cpp database.cpp account.cpp estate.cpp menu_item.cpp content_box.cpp sqlite3.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LIB)

sqlite3.o: sqlite3.c
	$(CC) -o $@ -c $^

clean:
	rm -f $(BIN)

testclean: clean
	rm -f *.db

.PHONY: all, clean
