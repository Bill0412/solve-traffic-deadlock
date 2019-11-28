#ifndef _COMMON_H
#define _COMMON_H
/*
 * State: the states of the car
 * ---------------------------------------------------
 * wait: waiting in the queue, after the arrived car
 * arrive: arrive at the crossing, wait for the first mutex
 * m1: acquires the first mutex
 * m2: acquires the second mutex
 * leave: the car leaves the crossing, at the opposite side
 */
enum State { waiting, arrive, m1, m2, leave };

enum Direction { north, east, west, south, dir_count };

enum Mutex { a, b, c, d, mutex_count};

Direction char2direction(char char_dir)
{
    switch(char_dir)
    {
        case 'n': return north; break;
        case 'e': return east; break;
        case 's': return south; break;
        case 'w': return west; break;
        default: return Direction::dir_count;
    }
}

#endif