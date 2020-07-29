sdlvg: main.o sdlgc.o board.o gc.o
	g++ -o sdlvg main.o sdlgc.o board.o gc.o `sdl2-config --libs` -lSDL2_mixer

main.o: main.cpp sdlgc.h gc.h
	g++ -c main.cpp -o main.o `sdl2-config --cflags` -ggdb3
board.o: board.cpp gc.h
	g++ -c board.cpp -o board.o
gc.o: gc.cpp gc.h
	g++ -c gc.cpp -o gc.o
sdlgc.o: sdlgc.cpp gc.h sdlgc.h
	g++ -c sdlgc.cpp -o sdlgc.o `sdl2-config --cflags`

