cmake_minimum_required(VERSION 3.10)
project(Ex1)

find_package(OpenMP REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES 
    main.cpp 
    kMeansSeq.cpp 
    kMeansPar.cpp
    Point.cpp 
    DatasetSoA.cpp
    kMeansParSoA.cpp
    calculateDistanceSquare.cpp
    dataLoader.cpp
)

add_executable(my_kmeans ${SOURCES})


if(OpenMP_CXX_FOUND)
    target_link_libraries(my_kmeans PUBLIC OpenMP::OpenMP_CXX)
endif()
