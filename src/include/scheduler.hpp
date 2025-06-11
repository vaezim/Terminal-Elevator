#pragma once

#include <list>
#include <vector>
#include <thread>
#include <memory>

#include "elevator.hpp"
#include "passenger_process.hpp"


namespace ElevatorSimLib {

class Scheduler {
public:
    Scheduler(int numFloors, int numElevators, float passengersPerMinute);

    void Run();
    void Display();

private:
    void RunPassengerProcess();

    const int m_numFloors;
    const int m_numElevators;
    const float m_passengersPerMinute;

    std::vector<std::shared_ptr<Elevator>> m_elevators;

    PassengerProcess m_passengerProcess;

    // Queue is a better choice but list is used
    // for printing purposes. Queue cannot be iterated
    std::list<Passenger> m_passengerList;
    mutable std::mutex m_passengerListMutex;
};

} // namespace ElevatorSimLib