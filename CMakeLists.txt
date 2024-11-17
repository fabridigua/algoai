cmake_minimum_required(VERSION 3.5.0)
project(algoai VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -pthread") 

include_directories(include)

# add_subdirectory(src/core)
# add_subdirectory(src/constraints)
# add_subdirectory(src/learning)
# add_subdirectory(src/search)

add_executable(algoai 
        ./src/algo.cpp
)

include(CTest)
enable_testing()
add_subdirectory(tests)
