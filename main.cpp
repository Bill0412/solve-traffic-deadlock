#include <iostream>
#include "traffic.h"

int main(int argc, char** argv)
{
    std::string car_series = argv[1];
    Traffic traffic(car_series);

    return 0;
}