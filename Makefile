CXX = g++
CXXFLAGS = -Wall -g -std=c++11 -Wno-reorder -pthread

objects = main.o traffic.o road.o car.o common.o
.PHONY: main
main: $(objects)
	$(CXX) $(CXXFLAGS) -o p1-1 $(objects)

traffic.o: traffic.h common.h road.h car.h 

road.o: road.h common.h car.h 

car.o: car.h common.h 

common.o: common.h


.PHONY: clear
clear:
	rm p1-1 $(objects)