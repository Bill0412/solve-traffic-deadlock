#include "road.h"

#include <pthread.h>
#include <iostream>

Road::Road(Direction direction)
    :
    m_is_initialized(false)
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
    m_queue.push(&car);
    // m_is_cars_initialized = true;
}

Car& Road::pop_car()
{
    Car* car = m_queue.front();
    m_queue.pop();
    return *car;
}

bool Road::is_first_car_arrived()
{
    Car* first_car = m_queue.front();
    if(m_queue.empty()) 
    {
        return false;
    }
    return (State::arrive == first_car->get_state());
}

void Road::set_first_car_arrived()
{
    m_queue.front()->set_state(State::arrive);
}

bool Road::is_road_empty()
{
    return m_queue.empty();
}

bool Road::is_delete_queue_empty()
{
    return m_delete_queue.empty();
}

void Road::set_first_priority()
{
    if(is_road_empty()) 
    {
        return;
    }

    m_queue.front()->set_first_priority();
}

void* Road::road_event_handler(void* args)
{
    Road* road = static_cast<Road*>(args);

    while(true)
    {
        while(!road->get_is_initialized());


        // if a car enters the first mutex section, remove it from the queue
        // and push the car into a to be deleted queue
        Car* front_car = road->get_ptr_front_car();

        if(front_car && front_car->has_left_road())
        {
            road->push_delete_queue(road->pop_car());
        }

        // make the first car the arrived car if road is not empty
        if(!road->is_road_empty()) 
        {
#if DEBUG
        // std::cout << "Road is not empty.\n";
#endif
            road->set_first_car_arrived();

        }        

        // garbage collection
        // remove the cars in the to be deleted queue that have already left
        road->delete_left_car();

        // if all the cars are initialized the road is empty
        if(road->is_delete_queue_empty() & road->is_road_empty())
        {
            // break;]
            // idle
        }
       
    }
}

Car& Road::get_front_car()
{
    return *m_queue.front();
}

Car* Road::get_ptr_front_car()
{
    return m_queue.front();
}

void Road::push_delete_queue(Car& car)
{
    m_delete_queue.push(&car);
}

Car& Road::pop_delete_queue()
{
    Car* car = m_delete_queue.front();
    m_delete_queue.pop();
    return *car;
}

void Road::delete_left_car()
{
    if(m_delete_queue.front() && m_delete_queue.front()->get_state() == State::leave)
    {
        Car& car = pop_delete_queue();
        delete &car;
    }
}

void Road::set_is_initialized()
{
    m_is_initialized = true;
}

bool Road::get_is_initialized()
{
    return m_is_initialized;
}