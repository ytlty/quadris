main: main.o tetrimino.o board.o window.o  
	g++ main.o tetrimino.o board.o window.o  -o quadris -lX11
main.o: main.cc tetrimino.h board.h
	g++ -c main.cc
tetrimino.o: tetrimino.h tetrimino.cc
	g++ -c tetrimino.cc
board.o: board.h board.cc 
	g++ -c board.cc
window.o: window.h window.cc
	g++ -c window.cc 

.PHONY: clean
clean:
	rm *.o quadris
