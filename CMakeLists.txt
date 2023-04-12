cmake_minimum_required(VERSION 3.17)

project(ElasticTask)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/tools/cmake/")
find_package(SimGrid REQUIRED)
include_directories("${SimGrid_INCLUDE_DIR}" SYSTEM)

MESSAGE("${SimGrid_INCLUDE_DIR}")

configure_file(Elasticity/ElasticConfig.hpp.in ${PROJECT_SOURCE_DIR}/Elasticity/ElasticConfig.hpp)

add_subdirectory(Elasticity)
add_subdirectory(energy_analysis)
