#include <thread>

#include "elevator_sim_lib.hpp"


int main()
{
    constexpr int numFloors{ 30 };
    constexpr int numElevators{ 8 };
    constexpr float passengersPerMinute{ 120.0 };

    ElevatorSimLib::Scheduler scheduler(numFloors, numElevators, passengersPerMinute);

    std::thread displayThread(&ElevatorSimLib::Scheduler::Display, &scheduler);
    scheduler.Run();
    displayThread.join();
}