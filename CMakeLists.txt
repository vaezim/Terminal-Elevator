cmake_minimum_required(VERSION 3.0)
project(elevator_sim_lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output/exe)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output/lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS
    "-Wall -Werror -Wextra -Wshadow -Wnon-virtual-dtor -Wunused -pedantic"
)

include_directories(PUBLIC ${CMAKE_SOURCE_DIR}/src/include)
link_directories(PUBLIC ${CMAKE_SOURCE_DIR}/output/lib)

add_subdirectory(src)

add_executable(elevator_sim main.cpp)
target_link_libraries(elevator_sim elevator_sim_lib ncurses)