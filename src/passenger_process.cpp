#include "passenger_process.hpp"


namespace ElevatorSimLib {

// Random generator used for all distributions
std::random_device rd;
std::mt19937 gen(rd());

Passenger PassengerProcess::Next() const
{
    std::uniform_int_distribution<> floorDist(1, m_numFloors);
    std::poisson_distribution<int> arrivalDist(m_passengersPerMinute);

    Passenger passenger;

    // With probability 0.5 passenger arrives at floor 0 or at another floor
    if (rand() % 2 == 0) {
        passenger.srcFloor = floorDist(gen);
    } else {
        passenger.dstFloor = floorDist(gen);
    }
    passenger.millisecondsBeforeArrival =
        std::chrono::milliseconds(static_cast<int>(1.0 / arrivalDist(gen) * 60'000));
    
    return passenger;
}

} // ElevatorSimLib