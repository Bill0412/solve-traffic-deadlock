#ifndef _TRAFFIC_H
#define _TRAFFIC_H

#include "car.h"
#include "road.h"
#include "common.h"

class Traffic
{
private:
    Road* m_roads[4];  // north, east, south, west roads
    int m_index;  // the overall counter for car indices.

    // mutexes for 4 Quardrants

    // pids
    pthread_t m_pid_car_generator;
    pthread_t m_pid_deadlock_detector;
    pthread_t m_pid_leaving_car_collector;

    // generate cars according to user input
    void m_generate_cars(std::string directions);

    // init the traffic conditions:
    // 1. start generating car threads
    // 2. deadlock detector thread, when all the 4 cars arrive at the intersection
    //      one of these cars should get an instruction
    // 3. leave car thread collector
    // 
    void m_init_traffic();

    // generate cars continously
    // void* m_generate_car_thread();

    // deadlock detector thread
    void* m_deadlock_detector_thread();
    bool m_is_all_first_cars_arrived();

    // leaving car collector, should be handled by road threads
    // void* m_leaving_car_collector_thread();

    // get next index and increase the index
    int m_get_index();

    // add a new car to the traffic, according to the direction
    void m_push_car(Car* ptr_car, Direction direction);

    // 4 sections at the crossing
    pthread_mutex_t m_mutex[4];
public:
    Traffic();  // default constructor 
    Traffic(std::string directions);
    ~Traffic();     // destructor


    // generate car continously, this should be the main loop
    // maybe add some time interval to make sure it can be
    // displaed at a acceptable speed.
    // Car generate_car();

    Road& get_road(Direction direction);

    
};

#endif