#include "traffic.h"

#include <stdlib.h>
#include <pthread.h>
// class Traffic;

Traffic()
{

}

Traffic::Traffic(std::string directions)
    :
    m_index(0)
{
    m_pid_car_generator = NULL;
    m_pid_deadlock_detector = NULL;

    m_init_traffic(directions);
}

Traffic::~Traffic()
{
    for(int i = 0; i < Direction::dir_count; i++)
    {
        free m_roads[i];
    }
}


void Traffic::m_generate_cars(std::string directions)
{
    for(char char_dir : directions)
    {
        Car* car = new Car(m_get_index(), char_dir, m_mutex, *this);
        m_push_car(car);
    }   
    
}  

void Traffic::m_init_traffic(std::string directions)
{
    // init 4 roads
    for(int i = 0; i < Direction::dir_count; i++)
    {
        m_roads[i] = new Road(Direction[i]);
    }

    // init 4 mutexes
    for(Mutex m = Mutex::m; i < Mutex::dir_count; i++)
    {
        pthread_mutex_init(&m, NULL);
    }

    // cars generator thread
    // pthread_create(&m_pid_car_generator, NULL, m_generate_car_thread, NULL);

    // get the cars from input
    m_generate_cars(directions);

    // deadlock detector thread, when all the 4 cars arrive at the intersection simultaneously
    //      one of these cars should get an instruction to take action to avoid deadlock
    pthread_create(&m_pid_deadlock_detector, NULL, m_deadlock_detector_thread, NULL);

    // leave car thread collector
    // pthread_create(&m_pid_leaving_car_collector, NULL, m_leaving_car_collector_thread, NULL);

    pthread_join(m_pid_deadlock_detector, NULL);
}

void* Traffic::m_deadlock_detector_thread()
{
    if(m_is_all_first_cars_arrived()) {
        // send signal to the north car to go first
        // maybe later set priority in turns
        m_roads[Direction::north]->set_first_priority();
    }
}

bool Traffic::m_is_all_first_cars_arrived()
{
    for(Road* road : m_roads)
    {
        if(!road->is_first_car_arrived())
        {
            return false;
        }
    }

    return true;
}

void Traffic::m_push_car(Car* car)
{
    auto direction = car->get_direction();

    m_roads[direction]->push_car(*car);
}

Road& Traffic::get_road(Direction direction)
{
    return m_road[direction];
}
// void* Traffic::m_leaving_car_collector_thread()
// {

// }