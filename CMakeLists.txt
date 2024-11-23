cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(MCTS)

find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

find_package(yaml-cpp REQUIRED)
include_directories(${YAML_CPP_INCLUDE_DIRS})

configure_file("${CMAKE_SOURCE_DIR}/config.yaml" "${CMAKE_BINARY_DIR}/config.yaml" COPYONLY)

include_directories("${CMAKE_SOURCE_DIR}/include")
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(MCTS "${SOURCES}")

target_link_libraries(MCTS ${OpenMP_LIBRARIES})
target_link_libraries(MCTS ${YAML_CPP_LIBRARIES})

set_property(TARGET MCTS PROPERTY CXX_STANDARD 20)