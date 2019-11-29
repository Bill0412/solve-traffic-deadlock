#include "road.h"

#include <pthread.h>
#include <iostream>

Road::Road(Direction direction)
{ 
    m_direction = direction;

     // road event handler, make the car arrive and leave
     pthread_create(&m_pid, NULL, road_event_handler, this);

#if DEBUG
    std::cout << "Road Created.\n";
#endif
}

void Road::push_car(Car& car)
{
    m_queue.push(car);
}

Car& Road::pop_car()
{
    Car& car = m_queue.front();
    m_queue.pop();
    return car;
}

bool Road::is_first_car_arrived()
{
    Car& first_car = m_queue.front();
    if(m_queue.empty()) 
    {
        return false;
    }
    return (State::arrive == first_car.get_state());
}

void Road::set_first_car_arrived()
{
    m_queue.front().set_state(State::arrive);
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

void* Road::road_event_handler(void* args)
{
    Road* road = static_cast<Road*>(args);

    while(true)
    {
        // make the first car the arrived car
        if(!road->is_road_empty()) 
        {
#if DEBUG
        // std::cout << "Road is not empty.\n";
#endif
            road->set_first_car_arrived();
        }

        // if a car enters the first mutex section, remove it from the queue
        // and push the car into a to be deleted queue


        // garbage collection
        // remove the cars in the to be deleted queue that have already left
        

       
    }
}