install: vg
	cp vg ../bin/vg

.PHONY: clean install

clean:
	rm *.o vg

vg: main.o sdlglgc.o sdlgc.o board.o gc.o ui.o framer.o agent.o
	g++ -ggdb3 -o vg main.o sdlglgc.o sdlgc.o board.o gc.o agent.o ui.o framer.o `sdl2-config --libs` -lSDL2_mixer -lGL

main.o: main.cpp sdlgc.h gc.h framer.h
	g++ -ggdb3 -c main.cpp -o main.o `sdl2-config --cflags` -ggdb3
framer.o: framer.cpp framer.h
	g++ -ggdb3 -c framer.cpp -o framer.o
board.o: board.cpp gc.h
	g++ -ggdb3 -c board.cpp -o board.o
agent.o: agent.cpp agent.h
	g++ -ggdb3 -c agent.cpp -o agemt.o
gc.o: gc.cpp gc.h
	g++ -ggdb3 -c gc.cpp -o gc.o
ui.o: ui.cpp ui.h
	g++ -ggdb3 -c ui.cpp -o ui.o
sdlgc.o: sdlgc.cpp gc.h sdlgc.h
	g++ -ggdb3 -c sdlgc.cpp -o sdlgc.o `sdl2-config --cflags`
sdlglgc.o: sdlglgc.cpp gc.h sdlgc.h sdlglgc.h
	g++ -ggdb3 -c sdlglgc.cpp -o sdlglgc.o `sdl2-config --cflags`
