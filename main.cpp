#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include "Point.h"
#include "calculateDistanceSquare.h"
#include "kMeansSeq.h"
#include "kMeansPar.h"
#include "dataLoader.h"

int main() {
    
    // Carichiamo il dataset
    std::vector<Point> dataset = loadWineDataset();
    
    if (dataset.empty()) {
        return 1; // Uscita con errore se il file non viene trovato
    }

    std::cout << "Caricati " << dataset.size() << " punti dal dataset\n\n";

    // 2. Parametri del K-Means
    int K = 3; 
    int max_iters = 200;

    // Creiamo una copia del dataset per fare un confronto equo, 
    // altrimenti la versione parallela partirebbe con i cluster già assegnati!
    std::vector<Point> dataset_seq = dataset;
    std::vector<Point> dataset_par = dataset;

    // 3. Esecuzione e misurazione della versione SEQUENZIALE
    std::cout << "--- Esecuzione K-Means Sequenziale ---\n";
    double start_seq = omp_get_wtime();
    kMeansSeq(dataset_seq, K, max_iters);
    double end_seq = omp_get_wtime();
    std::cout << "Tempo impiegato: " << (end_seq - start_seq) * 1000.0 << " ms\n\n";

    // 4. Esecuzione e misurazione della versione PARALLELA
    std::cout << "--- Esecuzione K-Means Parallelo ---\n";
    double start_par = omp_get_wtime();
    kMeansPar(dataset_par, K, max_iters);
    double end_par = omp_get_wtime();
    std::cout << "Tempo impiegato: " << (end_par - start_par) * 1000.0 << " ms\n\n";

    // 5. Stampa dei risultati finali (usando i dati della versione parallela)
    std::vector<int> cluster_counts(K, 0);
    for (size_t i = 0; i < dataset_par.size(); ++i) {
        if (dataset_par[i].cluster >= 0 && dataset_par[i].cluster < K) {
            cluster_counts[dataset_par[i].cluster]++;
        }
    }

    std::cout << "--- Risultati del raggruppamento ---\n";
    for (int i = 0; i < K; ++i) {
        std::cout << "Cluster " << i << ": " << cluster_counts[i] << " elementi\n";
    }

    return 0;
}