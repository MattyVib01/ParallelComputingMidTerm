#include "kMeansSeq.h"
#include <iostream>
#include <limits>
#include <ctime>

void kMeansSeq(std::vector<Point>& points, int K, int max_iterations) {     
    int n = points.size();  
    int dimensions = points[0].values.size();   

    std::vector<std::vector<double>> centroids(K, std::vector<double>(dimensions));
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < K; i++) {
        centroids[i] = points[rand() % n].values; 
    }

    int iter = 0;
    bool changed = true;

    while (iter < max_iterations && changed) {
        changed = false;

        
        for (int i = 0; i < n; i++) {   
            double min_dist = std::numeric_limits<double>::max();   
            int best_cluster = -1;  
            for (int j = 0; j < K; ++j) { 
                double dist = calculateDistanceSquare(points[i].values, centroids[j]);  
                if (dist < min_dist) {  
                    min_dist = dist;    
                    best_cluster = j;   
                }
            }

            if (points[i].cluster != best_cluster) {
                points[i].cluster = best_cluster;
                changed = true; 
            }
        }

        if (!changed) break;

        std::vector<std::vector<double>> new_centroids_sum(K, std::vector<double>(dimensions, 0.0));  
        std::vector<int> cluster_counts(K, 0);  

        for (int i = 0; i < n; ++i) {   
            int cluster_id = points[i].cluster;     
            cluster_counts[cluster_id]++;       
            for (int d = 0; d < dimensions; ++d) {  
                new_centroids_sum[cluster_id][d] += points[i].values[d];  
                 }
        }

        for (int j = 0; j < K; ++j) {   
            if (cluster_counts[j] > 0) { 
                for (int d = 0; d < dimensions; ++d) {
                    centroids[j][d] = new_centroids_sum[j][d] / cluster_counts[j]; 
                }
            }
        }

        iter++;
    }

    std::cout << "K-Means ha converso in " << iter << " iterazioni.\n";
}