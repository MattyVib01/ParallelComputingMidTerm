#include "calculateDistanceSquare.h"

double calculateDistanceSquare(const std::vector<double>& p1, const std::vector<double>& p2) {
    double sum = 0.0;
    for (int i = 0; i < p1.size(); i++) {
        sum += (p1[i] - p2[i]) * (p1[i] - p2[i]);
    }
    return sum;
}