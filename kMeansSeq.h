#ifndef KMEANS_SEQ_H
#define KMEANS_SEQ_H

#include <vector>
#include "Point.h"
#include "calculateDistanceSquare.h"

void kMeansSeq(std::vector<Point>& points, int K, int max_iterations);

#endif