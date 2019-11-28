#include "common.h"

Direction char2direction(char char_dir)
{
    switch(char_dir)
    {
        case 'n': return Direction::north; 
        case 'e': return Direction::east;
        case 's': return Direction::south; 
        case 'w': return Direction::west;
        default: return Direction::count;
    }
}