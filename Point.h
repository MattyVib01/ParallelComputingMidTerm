#ifndef POINT_H  // Se POINT_H non è ancora stato definito...
#define POINT_H  

#include <vector>

struct Point {
    int cluster;
    std::vector<double> values;

    // Qui mettiamo solo la signature (firma)
    Point(std::vector<double> v);
};

#endif // Fine della protezione