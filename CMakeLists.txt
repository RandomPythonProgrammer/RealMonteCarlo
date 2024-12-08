cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(MCTS)

include(FetchContent)

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG master
)
FetchContent_MakeAvailable(yaml-cpp)

configure_file("${CMAKE_SOURCE_DIR}/config.yaml" "${CMAKE_BINARY_DIR}/config.yaml" COPYONLY)

include_directories("${CMAKE_SOURCE_DIR}/include")
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(MCTS "${SOURCES}")

target_link_libraries(MCTS PRIVATE yaml-cpp::yaml-cpp)

set_property(TARGET MCTS PROPERTY CXX_STANDARD 20)