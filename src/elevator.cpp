#include <thread>

#include "elevator.hpp"


namespace ElevatorSimLib {

bool Elevator::IsIdle() const
{
    std::lock_guard<std::mutex> guard(m_isIdleMutex);
    return m_isIdle;
}

int Elevator::GetCurrentFloor() const
{
    std::lock_guard<std::mutex> guard(m_floorMutex);
    return m_currentFloor;
}

void Elevator::ServePassenger(int srcFloor, int dstFloor)
{
    m_isIdleMutex.lock();
    m_isIdle = false;
    m_isIdleMutex.unlock();

    // Time it takes for the passenger to get on/off the elevator
    constexpr std::chrono::seconds PASSENGER_BOARDING_DELAY{ 2 };

    Move(srcFloor);
    std::this_thread::sleep_for(PASSENGER_BOARDING_DELAY);
    Move(dstFloor);
    std::this_thread::sleep_for(PASSENGER_BOARDING_DELAY);

    m_isIdleMutex.lock();
    m_isIdle = true;
    m_isIdleMutex.unlock();
}

void Elevator::Move(int floor)
{
    if (floor == m_currentFloor || floor > m_numFloors) {
        return;
    }

    // Time it takes for the elevator to move on floor
    constexpr std::chrono::milliseconds TIME_PER_FLOOR{ 500 };

    // Up: +1 | Down: -1
    int direction = (floor > m_currentFloor) ? 1 : -1;
    while (m_currentFloor != floor) {
        std::this_thread::sleep_for(TIME_PER_FLOOR);
        m_floorMutex.lock();
        m_currentFloor += direction;
        m_floorMutex.unlock();
    }
}

} // namespace ElevatorSimLib