main: main.o
	g++ -o main main.o `sdl2-config --libs`
main.o: main.cpp
	g++ -c main.cpp -o main.o `sdl2-config --cflags`
