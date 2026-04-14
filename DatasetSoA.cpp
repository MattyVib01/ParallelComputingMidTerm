#pragma once
#include <vector>
#include "Point.h"


struct DatasetSoA {
    int num_points;
    int dimensions;
    
    std::vector<double> coords; 
    std::vector<int> clusters;  
};

inline DatasetSoA convertToSoA(const std::vector<Point>& aos_dataset) {
    DatasetSoA soa;
    if (aos_dataset.empty()) return soa;

    soa.num_points = aos_dataset.size();
    soa.dimensions = aos_dataset[0].values.size();
    
    soa.coords.resize(soa.num_points * soa.dimensions);
    soa.clusters.resize(soa.num_points, -1); 

    for (int i = 0; i < soa.num_points; ++i) {
        soa.clusters[i] = aos_dataset[i].cluster;
        for (int d = 0; d < soa.dimensions; ++d) {
            soa.coords[i * soa.dimensions + d] = aos_dataset[i].values[d];
        }
    }
    
    return soa;
}