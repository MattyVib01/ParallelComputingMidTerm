#ifndef KMEANS_PAR_H
#define KMEANS_PAR_H

#include <vector>
#include "Point.h"
#include "calculateDistanceSquare.h"

void kMeansPar(std::vector<Point>& points, int K, int max_iterations);

#endif