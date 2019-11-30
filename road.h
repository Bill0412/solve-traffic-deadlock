#ifndef _ROAD_H
#define _ROAD_H

#include "common.h"
#include "car.h"

#include <queue>
#include <pthread.h>

class Road
{
private:
    Direction m_direction;
    std::queue <Car*> m_queue;
    std::queue <Car*> m_delete_queue; // left the road section
    pthread_t m_pid;
    bool m_is_initialized;

    bool m_is_set_arrive_locked;

public:
    Road(Direction direction);
    ~Road();
    void push_car(Car& car);
    Car& pop_car();

    bool is_first_car_arrived();
    void set_first_car_arrived();

    bool is_road_empty();
    bool is_delete_queue_empty();

    void set_first_priority();
    
    static void* road_event_handler(void* args);
    // this may be buggy
    Car& get_front_car();
    // use this one
    Car* get_ptr_front_car();

    void push_delete_queue(Car& car);
    Car& pop_delete_queue();
    void delete_left_car();

    void set_is_initialized();
    bool get_is_initialized();

    void unlock_set_arrive();
};

#endif