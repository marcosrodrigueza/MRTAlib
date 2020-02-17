cmake_minimum_required(VERSION 2.8.3)
project(MRTAlib)

## Compile as C++11
add_compile_options(-std=c++11)


set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
 include
)

## Declare a C++ library
add_library(${PROJECT_NAME}
  src/allocation.cpp
  src/bid.cpp
  src/helper_functions.cpp
  src/robot.cpp
  src/task.cpp
  src/taskmanager.cpp
 )

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide


#SAMPLE TESTS
add_executable(mono_test examples/test_monoround.cpp)
add_executable(mission_test examples/mission_test.cpp)
add_executable(mission_performance examples/mission_performance.cpp)


## Specify libraries to link a library or executable target against

target_link_libraries(mono_test
   ${PROJECT_NAME}
 )
target_link_libraries(mission_test
   ${PROJECT_NAME}
 )
target_link_libraries(mission_performance
   ${PROJECT_NAME}
 )
