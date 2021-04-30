CC=c++
CFLAGS=-Wall -Wextra -O3

all: chess.exe
	
chess.exe: chess.o
	$(CC) $(CFLAGS) chess.o -o chess.exe

chess.o: chess.cpp
	$(CC) $(CFLAGS) -c chess.cpp

clean:
	rm *.exe *.o
