#include "kMeansParSoA.h" 
#include <iostream>
#include <limits>
#include <ctime>
#include <omp.h>
#include <cstdlib>

// Funzione helper per calcolare la distanza al quadrato usando gli array piatti (SoA)
inline double calculateDistanceSquareSoA(const std::vector<double>& coords, int point_idx, 
                                         const std::vector<double>& centroids, int centroid_idx, 
                                         int dims) {
    double sum = 0.0;
    for (int d = 0; d < dims; ++d) {
        // Matematica SoA: (riga * colonne) + colonna
        double diff = coords[point_idx * dims + d] - centroids[centroid_idx * dims + d];
        sum += diff * diff;
    }
    return sum;
}

void kMeansParSoA(DatasetSoA& dataset, int K, int max_iterations) {
    int n = dataset.num_points;
    if (n == 0) return;
    int dims = dataset.dimensions;

    // FASE 1: Inizializzazione (Sequenziale)
    // I centroidi ora sono un unico array piatto di dimensione (K * dims)
    std::vector<double> centroids(K * dims);
    srand(static_cast<unsigned>(time(0)));
    
    for (int i = 0; i < K; i++) {
        int random_point_idx = rand() % n;
        for (int d = 0; d < dims; ++d) {
            centroids[i * dims + d] = dataset.coords[random_point_idx * dims + d];
        }
    }

    int iter = 0;
    bool changed = true;

    while (iter < max_iterations && changed) {
        changed = false;

        // FASE 2: Assegnazione parallela
        // Usiamo schedule(static) come default affidabile
        #pragma omp parallel for reduction(||:changed) schedule(static)
        for (int i = 0; i < n; i++) {
            double min_dist = std::numeric_limits<double>::max();
            int best_cluster = -1;

            for (int j = 0; j < K; ++j) {
                double dist = calculateDistanceSquareSoA(dataset.coords, i, centroids, j, dims);
                if (dist < min_dist) {
                    min_dist = dist;
                    best_cluster = j;
                }
            }

            // Leggiamo e scriviamo sull'array separato dei cluster
            if (dataset.clusters[i] != best_cluster) {
                dataset.clusters[i] = best_cluster;
                changed = true; 
            }
        }

        if (!changed) break;

        // FASE 3: Aggiornamento dei centroidi
        // Anche le somme diventano un array piatto!
        std::vector<double> new_centroids_sum(K * dims, 0.0);
        std::vector<int> cluster_counts(K, 0);

        #pragma omp parallel
        {
            // 3.1: Accumulatori LOCALI per ogni thread (piatti)
            std::vector<double> local_centroids_sum(K * dims, 0.0);
            std::vector<int> local_cluster_counts(K, 0);

            // 3.2: Distribuzione del ciclo sui punti
            #pragma omp for schedule(static)
            for (int i = 0; i < n; ++i) {
                int cluster_id = dataset.clusters[i];
                local_cluster_counts[cluster_id]++;
                
                for (int d = 0; d < dims; ++d) {
                    // Accumulo locale con indici SoA
                    local_centroids_sum[cluster_id * dims + d] += dataset.coords[i * dims + d];
                }
            }

            // 3.3: Unione sicura dei dati locali in quelli globali
            #pragma omp critical
            {
                for (int j = 0; j < K; ++j) {
                    cluster_counts[j] += local_cluster_counts[j];
                    for (int d = 0; d < dims; ++d) {
                        new_centroids_sum[j * dims + d] += local_centroids_sum[j * dims + d];
                    }
                }
            }
        } // Fine regione parallela

        // 3.4: Calcolo finale della media (Sequenziale)
        for (int j = 0; j < K; ++j) {
            if (cluster_counts[j] > 0) {
                for (int d = 0; d < dims; ++d) {
                    centroids[j * dims + d] = new_centroids_sum[j * dims + d] / cluster_counts[j];
                }
            }
        }

        iter++;
    }

    std::cout << "K-Means Parallelo (SoA) ha converso in " << iter << " iterazioni.\n";
}