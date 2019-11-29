#include "car.h"
#include "traffic.h"
#include "road.h"
#include <iostream>
#include <pthread.h>

Car::Car()
{
}

Car::Car(int index, Direction direction, Traffic& traffic, pthread_mutex_t* mutexes, State state)
{
    this->m_index = index;
    this->m_direction = direction;
    this->m_state = state;
    this->m_pid = NULL;
    this->m_traffic = &traffic;
    this->m_is_first_priority = false;
    this->m_is_just_arrived = true;
    this->m_mutexes = mutexes;
    this->m_has_entered_m1 = false;

    m_init_required_mutexes();

    m_run();
#if DEBUG
    std::cout << "Car " << m_index << " is created in the direction of " << m_get_str_direction() << std::endl;
#endif
}

Car::Car(int index, char char_dir, Traffic& traffic, pthread_mutex_t* mutexes, State state)
{
    Car(index, m_char2direction(char_dir), traffic, mutexes, state);
}

int Car::m_run()
{
    int res = pthread_create(&m_pid, NULL, static_ptr_car_handler, this);
    // pthread_join(m_pid, NULL);
    return res;
}

std::string Car::m_get_str_direction()
{
    switch(m_direction)
    {
        case Direction::north: return "North"; break;
        case Direction::east: return "East"; break;
        case Direction::south: return "South"; break;
        case Direction::west: return "West"; break;
        default: return "Error: m_get_direction"; break;
    }
}

std::string Car::m_get_str_state()
{
    switch(m_state) 
    {
        case State::arrive: return "arrives"; break;
        case State::leave: return "leaving"; break;
        default: return "Error: m_get_state"; break;
    }
}

State Car::m_get_state()
{
    return m_state;
}

Direction Car::m_char2direction(char char_dir)
{
    return char2direction(char_dir);
}

void Car::m_init_required_mutexes()
{
    switch(m_direction)
    {
        case Direction::north: m_set_mutex12(Mutex::c, Mutex::d); break;
        case Direction::east:  m_set_mutex12(Mutex::b, Mutex::c); break;
        case Direction::south: m_set_mutex12(Mutex::a, Mutex::b); break;
        case Direction::west:  m_set_mutex12(Mutex::d, Mutex::a); break;
        case Direction::count: break;
        default: break;
    }
}

void Car::m_set_mutex12(Mutex mutex1, Mutex mutex2)
{
    m_mutex1 = mutex1;
    m_mutex2 = mutex2;
    m_pthread_mutex1 = m_mutexes[static_cast<int>(mutex1)];
    m_pthread_mutex2 = m_mutexes[static_cast<int>(mutex2)];
}

void Car::m_set_has_entered_m1()
{
    m_has_entered_m1 = true;
}

bool Car::m_is_rhs_empty()
{
    int count = static_cast<int>(Direction::count);
    Direction rhs_direction = static_cast<Direction>((static_cast<int>(m_direction) + count - 1) % count);
    Road& rhs_road = m_traffic->get_road(rhs_direction);
    return rhs_road.is_road_empty();
}

bool Car::m_get_is_first_priority()
{
    return m_is_first_priority;
}

void Car::m_set_is_first_priority()
{
    m_is_first_priority = true;
}

bool Car::m_reset_is_first_priority()
{
    m_is_first_priority = false;
}

Direction Car::get_direction()
{
    return this->m_direction;
}

State Car::get_state()
{
    return this->m_state;
}

State Car::set_state(State state)
{
    State prev_state = this->m_state;
    this->m_state = state;
    return prev_state;
}

bool Car::is_arrived()
{
    return (m_get_state() == State::arrive);
}

bool Car::is_just_arrived()
{
    return m_is_just_arrived;
}

void Car::set_is_just_arrived()
{
    m_is_just_arrived = true;
}

void Car::reset_is_just_arrived()
{
    m_is_just_arrived = false;
}

void Car::display_state()
{
    std::cout << "car " 
        << m_index << " from " 
        << m_get_str_direction() << " "
        << m_get_str_state() << " at crossing\n";
}

void Car::set_first_priority()
{
    m_set_is_first_priority();
}

bool Car::is_first_priority()
{
    return m_get_is_first_priority();
}

bool Car::is_rhs_empty()
{
    return m_is_rhs_empty();
}

void Car::lock_mutex1()
{
    pthread_mutex_lock(&m_pthread_mutex1);
}

void Car::unlock_mutex1()
{
    pthread_mutex_unlock(&m_pthread_mutex1);
}

void Car::lock_mutex2()
{
    pthread_mutex_lock(&m_pthread_mutex2);
}

void Car::unlock_mutex2()
{
    pthread_mutex_unlock(&m_pthread_mutex2);
}

bool Car::get_has_entered_m1()
{
    return m_has_entered_m1;
}

void Car::set_has_entered_m1()
{
    m_set_has_entered_m1();
}

void* Car::static_ptr_car_handler(void* args)
{
    while(true) 
    {
        // firstly waiting(do nothing)
        Car* car = static_cast<Car*>(args);
        // when arrive, see if there's car at the right hand side 
        
        if(car->is_arrived())
        {
#if DEBUG
            std::cout << "Car " << car->get_index() << " is arrived.\n";
            car->display_state();  
#endif
            if(car->is_just_arrived())
            {
                car->display_state();
                car->reset_is_just_arrived();
            }
            
            if(car->is_rhs_empty()) 
            {
                car->set_first_priority();
            }

            if(car->is_first_priority())
            {
                // if no car at the right hand side, try get mutex1
                car->lock_mutex1();
                car->set_state(State::m1);
                car->set_has_entered_m1();
                // then try get mutex2
                car->lock_mutex2();
                car->unlock_mutex1();
                car->set_state(State::m2);
                car->unlock_mutex2();
                car->set_state(State::leave);
                car->display_state();
                break;
            }
        }
    }
    
}

int Car::get_index()
{
    return m_index;
}