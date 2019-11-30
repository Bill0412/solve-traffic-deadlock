#include "traffic.h"

#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <algorithm>
// class Traffic;

Traffic::Traffic()
{

}

Traffic::Traffic(std::string directions)
    :
    m_index(0),
    m_roads_initialized(false),
    m_prev_deadlock_state({0, 0, 0, 0})
{
    m_pid_car_generator = NULL;
    m_pid_deadlock_detector = NULL; 
    
    // init console mutex
    pthread_mutex_init(&m_console_mutex, NULL);

    m_init_traffic(directions);
}

Traffic::~Traffic()
{
    for(Road* road : m_roads)
    {
        delete road;
    }
}


void Traffic::m_generate_cars(std::string directions)
{
    for(char const &char_dir : directions)
    {
        Car* car = new Car(m_get_index(), char_dir, *this, m_mutex);
        m_push_car(car);
    }   
    
}  

void Traffic::m_init_traffic(std::string directions)
{
    // init 4 roads
    for(int i = 0; i < static_cast<int>(Direction::count); i++)
    {
        m_roads.push_back(new Road(static_cast<Direction>(i)));
    }

    // init 4 mutexes
    for(int i = static_cast<int>(Mutex::a); i < static_cast<int>(Mutex::count); i++)
    {
        pthread_mutex_init(&m_mutex[i], NULL);
    }

    // deadlock detector thread, when all the 4 cars arrive at the intersection simultaneously
    //      one of these cars should get an instruction to take action to avoid deadlock
    pthread_create(&m_pid_deadlock_detector, NULL, deadlock_detector_thread, this);

    // leave car thread collector
    // pthread_create(&m_pid_leaving_car_collector, NULL, m_leaving_car_collector_thread, NULL);
    // done in road.cpp

    // cars generator thread
    // pthread_create(&m_pid_car_generator, NULL, m_generate_car_thread, NULL);

    // get the cars from input
    m_generate_cars(directions);

    // set roads initialized
    m_set_roads_initialized();

    pthread_create(&m_pid_traffic_end_detector, NULL, traffic_end_detector, this);

    pthread_join(m_pid_traffic_end_detector, NULL);
}

void* Traffic::deadlock_detector_thread(void* args)
{
    Traffic* traffic = static_cast<Traffic*>(args);
    while(true)
    {
        if(traffic->is_all_first_cars_arrived()) 
        {

            // get the current deadlock state(the indices of the cars)
            if(traffic->is_different_deadlock_situation())
            {
                // stall all cars
                traffic->set_is_stall();

                // send signal to the north car to go first
                // maybe later set priority in turns
                Direction direction = Direction::north;

                // wait until all arrives signalled
                while(!traffic->all_arrives_signalled());
                // signal deadlock when the deadlock is not signaled before
                // judge with a deadlock state, store indices of
                // the deadlock car pairs.
                traffic->signal_deadlock(direction);

                traffic->set_first_priority(direction);

                traffic->reset_all_arrives_signalled();

                traffic->reset_is_stall();
            }
        }
    }
}

int Traffic::m_get_index()
{
    return ++m_index;
}

void Traffic::m_push_car(Car* car)
{
    auto direction = car->get_direction();

    m_roads[static_cast<int>(direction)]->push_car(*car);
}

void Traffic::m_set_roads_initialized()
{
    for(int i = 0; i < static_cast<int>(Direction::count); i++)
    {
        m_roads[i]->set_is_initialized();
    }

    m_roads_initialized = true;
}

Road& Traffic::get_road(Direction direction)
{
    return *m_roads[static_cast<int>(direction)];
}

bool Traffic::is_all_first_cars_arrived()
{
    for(int i = 0; i < 4; i++)
    {
        Road* road = m_roads[i];
        if(!road->is_first_car_arrived())
        {
            return false;
        }
    }

    return true;
}

void Traffic::set_first_priority(Direction direction)
{
    m_roads[static_cast<int>(direction)]->set_first_priority();
}


void* Traffic::traffic_end_detector(void* args)
{
    Traffic* traffic = static_cast<Traffic*>(args);

    // wait until all the cars have gone through the crossing
    // while(!traffic->is_all_roads_empty());
    while(true);
}

bool Traffic::is_all_roads_empty()
{
    for(int i = 0; i < 4; i++)
    {
        Road* road = m_roads[i];
        if(!road->is_road_empty())
        {
            return false;
        }
    }
    return true;
}

void Traffic::signal_deadlock(Direction direction)
{
    lock_console_mutex();
    std::cout << "DEADLOCK: car jam detected, singalling " << dir2str(direction) << " to go\n";
    unlock_console_mutex();
}

bool Traffic::is_different_deadlock_situation()
{
    std::vector<int> deadlock_state = {};
    for(int i = 0; i < static_cast<int>(Direction::count); i++)
    {
        Car* front_car = m_roads[i]->get_ptr_front_car();
        if(!front_car) 
        {
            return false;   
        }
        deadlock_state.push_back(front_car->get_index());
    }
    std::sort(deadlock_state.begin(), deadlock_state.end());
    bool is_diff = (m_prev_deadlock_state != deadlock_state);
    if(is_diff) 
    {
        m_prev_deadlock_state = deadlock_state;
    }
    return is_diff;
}

bool Traffic::all_arrives_signalled()
{
    for(int i = 0; i < static_cast<int>(Direction::count); i++)
    {
        if(!(m_roads[i]->get_ptr_front_car()->is_arrive_signalled()))
        {
            return false;
        }
    }
    return true;
}

void Traffic::reset_all_arrives_signalled()
{
    for(int i = 0; i < static_cast<int>(Direction::count); i++)
    {
        m_roads[i]->get_ptr_front_car()->reset_arrive_signalled();
    }
}

void Traffic::lock_console_mutex()
{
    pthread_mutex_lock(&m_console_mutex);
}

void Traffic::unlock_console_mutex()
{
    pthread_mutex_unlock(&m_console_mutex);
}

bool Traffic::get_is_stall()
{
    return m_is_stall;
}

bool Traffic::set_is_stall()
{
    m_is_stall = true;
}

bool Traffic::reset_is_stall()
{
    m_is_stall = false;
}