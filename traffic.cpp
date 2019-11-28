#include "traffic.h"

#include <stdlib.h>
#include <pthread.h>
// class Traffic;

Traffic::Traffic()
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
    delete [] m_roads;
}


void Traffic::m_generate_cars(std::string directions)
{
    for(char char_dir : directions)
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
        m_roads[i] = new Road(static_cast<Direction>(i));
    }

    // init 4 mutexes
    for(int i = static_cast<int>(Mutex::a); i < static_cast<int>(Mutex::count); i++)
    {
        pthread_mutex_init(&m_mutex[i], NULL);
    }

    // cars generator thread
    // pthread_create(&m_pid_car_generator, NULL, m_generate_car_thread, NULL);

    // get the cars from input
    m_generate_cars(directions);

    // deadlock detector thread, when all the 4 cars arrive at the intersection simultaneously
    //      one of these cars should get an instruction to take action to avoid deadlock
    pthread_create(&m_pid_deadlock_detector, NULL, deadlock_detector_thread, this);

    // leave car thread collector
    // pthread_create(&m_pid_leaving_car_collector, NULL, m_leaving_car_collector_thread, NULL);

    pthread_join(m_pid_deadlock_detector, NULL);
}

void* Traffic::deadlock_detector_thread(void* args)
{
    Traffic* traffic = static_cast<Traffic*>(args);
    if(traffic->is_all_first_cars_arrived()) {
        // send signal to the north car to go first
        // maybe later set priority in turns
        traffic->set_first_priority(Direction::north);
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