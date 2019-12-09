cmake_minimum_required(VERSION 3.0.0)
project(wp3_tcp_register VERSION 0.1.0)

#----------------------------------------------------------------------------
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hpp)

#----------------------------------------------------------------------------
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost REQUIRED COMPONENTS system filesystem thread) 


if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    add_executable(wp3register main.cpp ${sources}) 
    target_link_libraries(wp3register ${Boost_LIBRARIES})
endif()
