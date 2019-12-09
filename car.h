#ifndef _CAR_H
#define _CAR_H
#include "common.h"

#include <string>
#include <pthread.h>
class Traffic;
class Road;

class Car {
private:
    Direction m_direction;    // which direction the car is from
    State m_state;
    int m_index;
    // int m_arrive_interval;    // should generate this randomely(less than 1s)
    
    pthread_t m_pid;

    int m_hungary_count;

    // mutex required later
    Mutex m_mutex1;
    Mutex m_mutex2;


    pthread_mutex_t m_pthread_mutex1;
    pthread_mutex_t m_pthread_mutex2;
    
    // 
    pthread_mutex_t* m_mutexes;

    pthread_mutex_t m_state_mutex;

    // if the car is prioritized to go
    bool m_is_first_priority;

    //
    bool m_is_arrive_signalled;

    // the ancestor traffic the car is in
    Traffic* m_traffic;

    // for mark if the car is firstly arrived at the crossing
    bool m_is_just_arrived;
    bool m_has_entered_m1;

    int m_run();

    Direction m_char2direction(char char_dir);
    std::string m_get_str_direction();
    std::string m_get_str_state();

    State m_get_state();

    // set the required mutexes later
    void m_init_required_mutexes();
    void m_set_mutex12(Mutex mutex1, Mutex mutex2);
    
    void m_set_has_entered_m1();

    // see if the right hand side of the car is empty
    bool m_is_rhs_not_arrived();

    void m_set_is_first_priority();
    bool m_get_is_first_priority();
    bool m_reset_is_first_priority();
public:
    Car(); // default constructor
    Car(int index, Direction direction, Traffic& traffic, pthread_mutex_t* mutexes, State state=State::waiting);
    Car(int index, char char_dir, Traffic& traffic, pthread_mutex_t* mutexes, State state=State::waiting);

    Direction get_direction();
    // Direction set_direction();
    
    State get_state();
    
    State set_state(State state);

    bool is_arrived();
    
    // change the state of m_is_just_arrived;
    bool is_just_arrived();
    void set_is_just_arrived();
    void reset_is_just_arrived();

    void display_state();

    void set_first_priority();
    bool is_first_priority();

    bool is_rhs_not_arrived();
    bool has_left_road();

    // mutex operations
    void lock_mutex1();
    void unlock_mutex1();
    void lock_mutex2();
    void unlock_mutex2();

    // state mutex
    void lock_state_mutex();
    void unlock_state_mutex();

    bool get_has_entered_m1();
    void set_has_entered_m1();

    // this is thr run thread of the car
    // all the possible behaviors of the car should be handled here
    // car event handler
    static void *static_ptr_car_handler(void* args);
    Road& get_road();
    Traffic& get_traffic();

    int get_index();

    bool is_arrive_signalled();
    void set_arrive_signalled();
    void reset_arrive_signalled();
};


#endif