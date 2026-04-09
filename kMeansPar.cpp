#include "kMeansPar.h" 
#include <iostream>
#include <limits>
#include <ctime>
#include <omp.h>

void kMeansPar(std::vector<Point>& points, int K, int max_iterations) {
    int n = points.size();
    if (n == 0) return;
    int dimensions = points[0].values.size();

    // FASE 1: Inizializzazione (Sequenziale)
    std::vector<std::vector<double>> centroids(K, std::vector<double>(dimensions));
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < K; i++) {
        centroids[i] = points[rand() % n].values;
    }

    int iter = 0;
    bool changed = true;

    while (iter < max_iterations && changed) {
        changed = false;

        // FASE 2: Assegnazione parallela
        // [!] Usiamo reduction(||:changed) per evitare race conditions sul flag
        #pragma omp parallel for reduction(||:changed)
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

        // FASE 3: Aggiornamento dei centroidi
        std::vector<std::vector<double>> new_centroids_sum(K, std::vector<double>(dimensions, 0.0));
        std::vector<int> cluster_counts(K, 0);

        // [!] Apriamo una regione parallela per gestire le somme locali
        #pragma omp parallel
        {
            // 3.1: Accumulatori LOCALI per ogni thread
            std::vector<std::vector<double>> local_centroids_sum(K, std::vector<double>(dimensions, 0.0));
            std::vector<int> local_cluster_counts(K, 0);

            // 3.2: Distribuiamo il ciclo sui punti tra i thread
            #pragma omp for
            for (int i = 0; i < n; ++i) {
                int cluster_id = points[i].cluster;
                local_cluster_counts[cluster_id]++;
                for (int d = 0; d < dimensions; ++d) {
                    local_centroids_sum[cluster_id][d] += points[i].values[d];
                }
            }

            
            // 3.3: Unione sicura dei dati locali in quelli globali
            // critical assicura che un solo thread alla volta aggiorni gli array globali
            #pragma omp critical
            {
                for (int j = 0; j < K; ++j) {
                    cluster_counts[j] += local_cluster_counts[j];
                    for (int d = 0; d < dimensions; ++d) {
                        new_centroids_sum[j][d] += local_centroids_sum[j][d];
                    }
                }
            }
        } // Fine regione parallela

        // 3.4: Calcolo finale della media (Sequenziale, velocissimo)
        for (int j = 0; j < K; ++j) {
            if (cluster_counts[j] > 0) {
                for (int d = 0; d < dimensions; ++d) {
                    centroids[j][d] = new_centroids_sum[j][d] / cluster_counts[j];
                }
            }
        }

        iter++;
    }

    std::cout << "K-Means Parallelo ha converso in " << iter << " iterazioni.\n";
}