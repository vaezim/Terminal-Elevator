#include <ncurses.h>

#include "scheduler.hpp"


namespace ElevatorSimLib {

Scheduler::Scheduler(int numFloors, int numElevators, float passengersPerMinute) :
    m_numFloors(numFloors),
    m_numElevators(numElevators),
    m_passengersPerMinute(passengersPerMinute),
    m_passengerProcess(numFloors, passengersPerMinute)
{
    for (int i{ 0 }; i < m_numElevators; i++) {
        m_elevators.emplace_back(std::make_shared<Elevator>(numFloors));
    }
}

void Scheduler::Run()
{
    // Run passenger arrival process
    std::thread passengerThread(&Scheduler::RunPassengerProcess, this);

    while (true) {

        // This wait is to let the elevator thread which is running
        // Elevator::ServePassenger() to set Elevator::isIdle to false
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Check passenger list size
        m_passengerListMutex.lock();
        size_t passengerListSize = m_passengerList.size();
        m_passengerListMutex.unlock();
        if (passengerListSize == 0) {
            continue;
        }

        // Get the top passenger (FIFO)
        m_passengerListMutex.lock();
        auto passenger = m_passengerList.front();
        m_passengerList.pop_front();
        m_passengerListMutex.unlock();

        // Serve that passenger
        size_t servingElevatorIndex{ 0 };
        while (true) {
            if (m_elevators[servingElevatorIndex]->IsIdle()) {
                break;
            }
            servingElevatorIndex = (servingElevatorIndex + 1) % m_elevators.size();
        }
        std::thread elevatorThread(&Elevator::ServePassenger,
            m_elevators[servingElevatorIndex], passenger.srcFloor, passenger.dstFloor);
        elevatorThread.detach();
    }

    passengerThread.join();
}

void Scheduler::RunPassengerProcess()
{
    while (true) {
        const Passenger &passenger = m_passengerProcess.Next();
        std::this_thread::sleep_for(passenger.millisecondsBeforeArrival);
        m_passengerListMutex.lock();
        m_passengerList.push_back(passenger);
        m_passengerListMutex.unlock();
    }
}

void Scheduler::Display()
{
    initscr();
    constexpr int START_X{ 5 }, START_Y{ 10 };
    const int width = m_numElevators*4 + 4;
    WINDOW *window = newwin(m_numFloors + 4, width, START_X, START_Y);

    while (true) {
        erase();
        werase(window);
        box(window, 0, 0);

        // Floor numbers
        for (int i{ m_numFloors }; i >= 0; i--) {
            mvwprintw(window, m_numFloors - i + 2, 1, "%d", i);
        }

        // Elevator current floors
        for (size_t i{ 0 }; i < m_elevators.size(); i++) {
            const int y{ 4*(int)i + 3 };
            mvwprintw(window, 1, y, " E%d ", (int)i+1);
            mvwprintw(window, m_numFloors + 2 - m_elevators[i]->GetCurrentFloor(), y, " ## ");
        }

        // Waiting passengers
        m_passengerListMutex.lock();
        int passengerListSize = m_passengerList.size();
        int x{ START_X + 1 }, y{ START_Y + width + 2 };
        mvprintw(x++, y, "%d passengers are waiting", passengerListSize);
        for (const auto &p : m_passengerList) {
            mvprintw(x++, y, "(%d --> %d)", p.srcFloor, p.dstFloor);
        }
        m_passengerListMutex.unlock();

        refresh();
        wrefresh(window);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    endwin();
}

} // namespace ElevatorSimLib