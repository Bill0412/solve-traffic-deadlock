#include "road.h"

#include <pthread.h>

Road::Road(Direction direction)
{ 
    m_direction = direction;

     // road event handler, make the car arrive and leave
     
}

bool Road::is_first_car_arrived()
{
    Car& first_car = m_queue.front;
    if(!first_car) 
    {
        return false;
    }
    return (State::arrive == first_car->state);
}
