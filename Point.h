#ifndef POINT_H 
#define POINT_H  

#include <vector>

struct Point {
    int cluster;
    std::vector<double> values;

    Point(std::vector<double> v);
};

#endif