# Video Game Prototype
# Copyright (C) 2021  Raymond Jennings
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

install: vg
	cp vg ../bin/vg

.PHONY: clean install

clean:
	rm *.o vg

vg: main.o sdlglgc.o sdlgc.o board.o gc.o ui.o framer.o agent.o sdl.o
	g++ -ggdb3 -o vg main.o sdlglgc.o sdlgc.o board.o gc.o agent.o sdl.o ui.o framer.o `sdl2-config --libs` -lSDL2_mixer -lGL

main.o: main.cpp sdlgc.h gc.h framer.h
	g++ -ggdb3 -c main.cpp -o main.o `sdl2-config --cflags` -ggdb3
audio.o: audio.cpp audio.h
	g++ -ggdb3 -c audio.cpp -o audio.o
sdl.o: sdl.cpp
	g++ -ggdb3 -c sdl.cpp -o sdl.o
framer.o: framer.cpp framer.h
	g++ -ggdb3 -c framer.cpp -o framer.o
board.o: board.cpp gc.h
	g++ -ggdb3 -c board.cpp -o board.o
agent.o: agent.cpp agent.h
	g++ -ggdb3 -c agent.cpp -o agent.o
gc.o: gc.cpp gc.h
	g++ -ggdb3 -c gc.cpp -o gc.o
ui.o: ui.cpp ui.h
	g++ -ggdb3 -c ui.cpp -o ui.o
sdlgc.o: sdlgc.cpp gc.h sdlgc.h
	g++ -ggdb3 -c sdlgc.cpp -o sdlgc.o `sdl2-config --cflags`
sdlglgc.o: sdlglgc.cpp gc.h sdlgc.h sdlglgc.h
	g++ -ggdb3 -c sdlglgc.cpp -o sdlglgc.o `sdl2-config --cflags`
