#include "road.h"

#include <pthread.h>

Road::Road(Direction direction)
{ 
    m_direction = direction;

     // road event handler, make the car arrive and leave
     
}

void Road::push_car(Car& car)
{
    m_queue.push(car);
}

Car& Road::pop_car()
{
    return m_queue.pop();
}

bool Road::is_first_car_arrived()
{
    Car& first_car = m_queue.front;
    if(m_queue.empty()) 
    {
        return false;
    }
    return (State::arrive == first_car->state);
}

bool Road::is_road_empty()
{
    return m_queue.empty();
}


void Road::set_first_priority()
{
    if(is_road_empty()) 
    {
        return;
    }

    m_queue.front().set_first_priority();
}