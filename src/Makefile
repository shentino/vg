install: vg
	cp vg ../bin/vg

.PHONY: clean install

clean:
	rm *.o vg

vg: main.o sdlglgc.o sdlgc.o board.o gc.o ui.o framer.o
	g++ -o vg main.o sdlglgc.o sdlgc.o board.o gc.o ui.o framer.o `sdl2-config --libs` -lSDL2_mixer

main.o: main.cpp sdlgc.h gc.h framer.h
	g++ -c main.cpp -o main.o `sdl2-config --cflags` -ggdb3
framer.o: framer.cpp framer.h
	g++ -c framer.cpp -o framer.o
board.o: board.cpp gc.h
	g++ -c board.cpp -o board.o
gc.o: gc.cpp gc.h
	g++ -c gc.cpp -o gc.o
ui.o: ui.cpp ui.h
	g++ -c ui.cpp -o ui.o
sdlgc.o: sdlgc.cpp gc.h sdlgc.h
	g++ -c sdlgc.cpp -o sdlgc.o `sdl2-config --cflags`
sdlglgc.o: sdlglgc.cpp gc.h sdlgc.h sdlglgc.h
	g++ -c sdlglgc.cpp -o sdlglgc.o `sdl2-config --cflags`
