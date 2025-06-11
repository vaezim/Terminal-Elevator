#pragma once

#include <mutex>
#include <chrono>


namespace ElevatorSimLib {

class Elevator {
public:
    Elevator(int numFloors) : m_numFloors(numFloors) {}

    bool IsIdle() const;
    int GetCurrentFloor() const;
    void ServePassenger(int srcFloor, int dstFloor);

private:
    // Move from <m_currentFloor> to <floor>
    void Move(int floor);

    bool m_isIdle{ true };
    mutable std::mutex m_isIdleMutex;

    int m_currentFloor{ 0 };
    const int m_numFloors{ 0 };
    mutable std::mutex m_floorMutex;
};

} // namespace ElevatorSimLib