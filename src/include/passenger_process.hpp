#pragma once

#include <chrono>
#include <random>


namespace ElevatorSimLib {

struct Passenger {
    int srcFloor{ 0 };
    int dstFloor{ 0 };
    std::chrono::milliseconds millisecondsBeforeArrival;
};

class PassengerProcess {
public:
    PassengerProcess() : m_numFloors(10), m_passengersPerMinute(40.0) {}
    PassengerProcess(int numFloors, float passengersPerMinute) :
        m_numFloors(numFloors), m_passengersPerMinute(passengersPerMinute) {}

    // Generate a random passenger with arrival times based on Poisson distribution
    Passenger Next() const;

private:
    const int m_numFloors;
    const float m_passengersPerMinute;
};

} // ElevatorSimLib