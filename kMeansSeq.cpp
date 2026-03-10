// Nota: omettiamo la radice quadrata (sqrt) perché per trovare il minimo
// è sufficiente confrontare i quadrati delle distanze. Rende il codice più veloce.
#include "kMeansSeq.h"
#include <iostream>
#include <limits>
#include <ctime>

void kMeansSeq(std::vector<Point>& points, int K, int max_iterations) { //ho un vettore di points, il k di k means e un max_iter
    int n = points.size();  //numero di punti
    int dimensions = points[0].values.size();   //dimensione (2D,3D)

    // FASE 1: Inizializzazione dei centroidi
    // Inizializza i centroidi
    std::vector<std::vector<double>> centroids(K, std::vector<double>(dimensions));
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < K; i++) {
        centroids[i] = points[rand() % n].values;   //inzializzo i centroidi prendendoli da points
    }

    int iter = 0;
    bool changed = true; // Flag per verificare se ci sono stati spostamenti

    while (iter < max_iterations && changed) {
        changed = false;

        // FASE 2: Assegnazione dei punti ai cluster
        // [!] Questo ciclo 'for(int i = 0; i < n; ...)' sarà il candidato perfetto per OpenMP
        for (int i = 0; i < n; i++) {   //prendo un punto
            double min_dist = std::numeric_limits<double>::max();   // imposto min_dist al più grande valore possibile
            int best_cluster = -1;  //imposto best_cluster a -1

            // Trova il centroide più vicino
            for (int j = 0; j < K; ++j) { //fissato un punto per ogni centroide
                double dist = calculateDistanceSquare(points[i].values, centroids[j]);  //calcolo la distanza fra punto e centroide
                if (dist < min_dist) {  // se la distanza fra punto e centroide è minore delle precedenti
                    min_dist = dist;    //aggiorno min_dist
                    best_cluster = j;   //aggiorno il centroide migliore
                }
            }

            // Se il punto ha cambiato cluster, aggiorniamo il flag
            if (points[i].cluster != best_cluster) {
                points[i].cluster = best_cluster;
                changed = true; 
            }
        }

        // Se nessun punto si è mosso, abbiamo raggiunto la convergenza
        if (!changed) break;

        // FASE 3: Aggiornamento dei centroidi
        // Creiamo accumulatori azzerati per le somme e per il conteggio dei punti
        std::vector<std::vector<double>> new_centroids_sum(K, std::vector<double>(dimensions, 0.0));    //Matrice [num.cluster x dimensione]
        std::vector<int> cluster_counts(K, 0);  //vettore che mi conterà quanti punti ci sono nel cluster

        // Sommiamo le coordinate di tutti i punti per ogni cluster
        // [!] Anche questo ciclo si parallelizza, ma occhio alle race conditions sugli array di somma!
        for (int i = 0; i < n; ++i) {   //prendo un punto
            int cluster_id = points[i].cluster;     //il suo cluster diventa cluster_id
            cluster_counts[cluster_id]++;       //cluster_count relativo al quel cluster aumenta di uno
            for (int d = 0; d < dimensions; ++d) {  //per ogni dimensione
                new_centroids_sum[cluster_id][d] += points[i].values[d];  
                //alla matrice in pos [clusterid, dimensione] viene aggiunto il valore del punto in quella dimensione
                 }
        }

        // Calcoliamo la media per ottenere le nuove coordinate del centroide
        for (int j = 0; j < K; ++j) {   //prendo un cluster
            if (cluster_counts[j] > 0) { // Evitiamo divisioni per zero per cluster vuoti
                for (int d = 0; d < dimensions; ++d) {
                    centroids[j][d] = new_centroids_sum[j][d] / cluster_counts[j]; //faccio la media su una dimensione
                }
            }
        }

        iter++;
    }

    std::cout << "K-Means ha converso in " << iter << " iterazioni.\n";
}