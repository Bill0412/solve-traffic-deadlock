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
    std::queue <Car> m_queue;
    pthread_t m_pid;

public:
    Road(Direction direction);
    ~Road();
    void push_car(Car& car);
    Car& pop_car();

    bool is_first_car_arrived();
    void set_first_car_arrived();

    bool is_road_empty();

    void set_first_priority();
    
    static void* road_event_handler(void* args);

};

#endif