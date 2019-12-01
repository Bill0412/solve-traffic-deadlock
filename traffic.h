#ifndef _TRAFFIC_H
#define _TRAFFIC_H

#include "car.h"
#include "road.h"
#include "common.h"

#include <vector>

class Traffic
{
private:
    std::vector<Road*> m_roads;  // north, east, south, west roads
    int m_index;  // the overall counter for car indices.
    
    // 4 sections at the crossing
    pthread_mutex_t m_mutex[4];
    pthread_mutex_t m_console_mutex;

    bool m_roads_initialized;
    Direction m_priority_direction;

    // store the previous deadlock car indices
    std::vector<int> m_prev_deadlock_state;
    bool m_is_stall;    // stall all the car threads if a deadlock happens

    // pids
    pthread_t m_pid_car_generator;
    pthread_t m_pid_deadlock_detector;
    pthread_t m_pid_leaving_car_collector;
    pthread_t m_pid_traffic_end_detector;

    // generate cars according to user input
    void m_generate_cars(std::string directions);

    // init the traffic conditions:
    // 1. start generating car threads
    // 2. deadlock detector thread, when all the 4 cars arrive at the intersection
    //      one of these cars should get an instruction
    // 3. leave car thread collector
    // 
    void m_init_traffic(std::string directions);

    // generate cars continously
    // void* m_generate_car_thread();

    // leaving car collector, should be handled by road threads
    // void* m_leaving_car_collector_thread();

    // get next index and increase the index
    int m_get_index();

    // add a new car to the traffic, according to the direction
    void m_push_car(Car* ptr_car);
 
    void m_set_roads_initialized();

public:
    Traffic();  // default constructor 
    Traffic(std::string directions);
    ~Traffic();     // destructor


    // generate car continously, this should be the main loop
    // maybe add some time interval to make sure it can be
    // displaed at a acceptable speed.
    // Car generate_car();

    Road& get_road(Direction direction);

    // deadlock detector thread
    static void* deadlock_detector_thread(void* args);
    bool is_all_first_cars_arrived();
    void set_first_priority(Direction direction);
    
    // if no cars in all the roads, the traffic should end.
    static void* traffic_end_detector(void* args);
    bool is_all_roads_empty();

    // signal deadlock
    void signal_deadlock(Direction direction);
    bool is_different_deadlock_situation();
    bool all_arrives_signalled();
    void reset_all_arrives_signalled();

    // console mutex
    void lock_console_mutex();
    void unlock_console_mutex();

    // stall
    bool get_is_stall();
    bool set_is_stall();
    bool reset_is_stall();

    Direction get_priority_direction();
};

#endif