CXX = g++
CXXFLAGS = -Wall -g -std=c++14 -Wno-reorder -lpthread

objects = main.o car.o
.PHONY: main
main: $(objects)
	$(CXX) $(CXXFLAGS) -o p1-1 $(objects)

traffic.o: traffic.h road.h car.h common.h

road.o: road.h car.h common.h

car.o: common.h car.h



.PHONY: clean
clear:
	rm p1-1 $(objects)