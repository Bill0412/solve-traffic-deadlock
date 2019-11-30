#ifndef _COMMON_H
#define _COMMON_H
#define DEBUG 1

#include <string>

/*
 * State: the states of the car
 * ---------------------------------------------------
 * wait: waiting in the queue, after the arrived car
 * arrive: arrive at the crossing, wait for the first mutex
 * m1: acquires the first mutex
 * m2: acquires the second mutex
 * leave: the car leaves the crossing, at the opposite side
 */
enum class State { waiting, arrive, m1, m2, leave, count };

enum class Direction { north, east, south, west, count };

enum class Mutex { a, b, c, d, count};

Direction char2direction(char char_dir);

std::string dir2str(Direction dir);

#endif