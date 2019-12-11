cmake_minimum_required(VERSION 3.0.0)
project(registers_rw VERSION 0.1.0)

#Use C++11
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

#----------------------------------------------------------------------------
include_directories(${PROJECT_SOURCE_DIR}/include 
    ${PROJECT_SOURCE_DIR}/jsonpkg/json/single_include/nlohmann)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hpp 
    ${PROJECT_SOURCE_DIR}/jsonpkg/json/single_include/nlohmann/json.hpp)

#----------------------------------------------------------------------------
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost REQUIRED COMPONENTS system filesystem thread) 


if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    add_executable(registers_rw main.cpp ${sources} ${headers}) 
    target_link_libraries(registers_rw ${Boost_LIBRARIES})
endif()
