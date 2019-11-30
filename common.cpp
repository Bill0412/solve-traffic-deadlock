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

std::string dir2str(Direction dir)
{
    switch(dir)
    {
        case Direction::north: return "North"; break;
        case Direction::east: return "East"; break;
        case Direction::south: return "South"; break;
        case Direction::west: return "West"; break;
        default: return "Error: dir2str"; break;
    }
}