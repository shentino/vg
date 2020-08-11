sdlvg: main.o sdlgc.o board.o gc.o ui.o
	g++ -o sdlvg main.o sdlgc.o board.o gc.o ui.o `sdl2-config --libs` -lSDL2_mixer

.PHONY: clean

clean:
	rm *.o sdlvg

main.o: main.cpp sdlgc.h gc.h
	g++ -c main.cpp -o main.o `sdl2-config --cflags` -ggdb3
board.o: board.cpp gc.h
	g++ -c board.cpp -o board.o
gc.o: gc.cpp gc.h
	g++ -c gc.cpp -o gc.o
ui.o: ui.cpp ui.h
	g++ -c ui.cpp -o ui.o
sdlgc.o: sdlgc.cpp gc.h sdlgc.h
	g++ -c sdlgc.cpp -o sdlgc.o `sdl2-config --cflags`

