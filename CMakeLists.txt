# 1. Versione minima richiesta di CMake
cmake_minimum_required(VERSION 3.10)

# 2. Nome del tuo progetto
project(Ex1)

find_package(OpenMP REQUIRED)

# 3. Specifica lo standard C++ (consigliato C++17 o superiore)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 4. Elenca tutti i file sorgente (.cpp) necessari
# Nota: Gli header (.h) non vanno elencati qui solitamente, 
# a meno che tu non voglia vederli in alcuni IDE come Visual Studio.
set(SOURCES 
    main.cpp 
    kMeansSeq.cpp 
    kMeansPar.cpp
    Point.cpp 
    calculateDistanceSquare.cpp
    dataLoader.cpp
)


# 5. Crea l'eseguibile partendo dai sorgenti
add_executable(my_kmeans ${SOURCES})


if(OpenMP_CXX_FOUND)
    target_link_libraries(my_kmeans PUBLIC OpenMP::OpenMP_CXX)
endif()

# 6. (Opzionale ma consigliato) Se hai gli header in una cartella specifica
# target_include_directories(my_kmeans PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})