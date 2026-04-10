#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <omp.h>
#include <numeric>     
#include <algorithm>   
#include "Point.h"
#include "kMeansSeq.h"
#include "kMeansPar.h"
#include "DatasetSoA.h"   
#include "kMeansParSoA.h" 
#include "dataLoader.h"

/**
 * Funzione di stampa statistica
 * Calcola media, mediana, min, max e deviazione standard.
 * Restituisce la MEDIANA per il calcolo dello speedup.
 */
double printStats(const std::string& label, std::vector<double> times) {
    if (times.empty()) return 0.0;

    // Ordiniamo la copia locale per calcolare mediana e statistiche di posizione
    std::sort(times.begin(), times.end());
    
    double sum = std::accumulate(times.begin(), times.end(), 0.0);
    double mean = sum / times.size();

    // Calcolo Mediana
    double median;
    size_t size = times.size();
    if (size % 2 == 0) {
        median = (times[size / 2 - 1] + times[size / 2]) / 2.0;
    } else {
        median = times[size / 2];
    }

    double min_t = times.front();
    double max_t = times.back();
    
    // Deviazione Standard
    double sq_sum = std::inner_product(times.begin(), times.end(), times.begin(), 0.0);
    double std_dev = std::sqrt(std::max(0.0, sq_sum / times.size() - mean * mean));

    std::cout << label << "\n";
    std::cout << "  Media:   " << mean * 1000.0 << " ms\n";
    std::cout << "  Mediana: " << median * 1000.0 << " ms\n";
    std::cout << "  Min:     " << min_t * 1000.0 << " ms\n";
    std::cout << "  Max:     " << max_t * 1000.0 << " ms\n";
    std::cout << "  StDev:   " << std_dev * 1000.0 << " ms\n\n";

    return median; 
}

int main() {
    // Caricamento dataset
    std::vector<Point> dataset_original = loadGeneratedDataset();
    if (dataset_original.empty()) return 1;

    // Preparazione SoA
    DatasetSoA dataset_soa_original = convertToSoA(dataset_original);

    std::cout << "Dataset caricato: " << dataset_original.size() << " punti.\n";

    int K = 500; 
    int max_iters = 200;
    int num_runs = 5; 
    int num_threads = 8; 
    omp_set_num_threads(num_threads);

    std::vector<double> seq_wall_times, par_wall_times, soa_wall_times;

    std::cout << "Esecuzione di " << num_runs << " run (Seq vs Par AoS vs Par SoA con " << num_threads << " thread)...\n\n";

    for (int r = 0; r < num_runs; ++r) {
        // Reset dei dati per ogni run
        std::vector<Point> dataset_seq = dataset_original;
        std::vector<Point> dataset_par = dataset_original;
        DatasetSoA dataset_soa = dataset_soa_original;

        // 1. MISURAZIONE SEQUENZIALE
        double start_seq = omp_get_wtime();
        kMeansSeq(dataset_seq, K, max_iters);
        seq_wall_times.push_back(omp_get_wtime() - start_seq);

        // 2. MISURAZIONE PARALLELA (AoS)
        double start_par = omp_get_wtime();
        kMeansPar(dataset_par, K, max_iters);
        par_wall_times.push_back(omp_get_wtime() - start_par);

        // 3. MISURAZIONE PARALLELA (SoA)
        double start_soa = omp_get_wtime();
        kMeansParSoA(dataset_soa, K, max_iters);
        soa_wall_times.push_back(omp_get_wtime() - start_soa);
        
        std::cout << "Run " << r + 1 << "/" << num_runs << " completata." << std::endl;
    }

    std::cout << "\n" << std::string(40, '=') << "\n";
    
    // Stampa Risultati e recupero delle Mediane
    std::cout << "=== RISULTATI SEQUENZIALE ===\n";
    double med_seq = printStats("Wall-clock time:", seq_wall_times);

    std::cout << "=== RISULTATI PARALLELO AoS (" << num_threads << " THREAD) ===\n";
    double med_par = printStats("Wall-clock time:", par_wall_times);

    std::cout << "=== RISULTATI PARALLELO SoA (" << num_threads << " THREAD) ===\n";
    double med_soa = printStats("Wall-clock time:", soa_wall_times);

    // ANALISI SPEEDUP BASATA SULLA MEDIANA
    std::cout << "=== ANALISI SPEEDUP (Basata sulle Mediane) ===\n";
    if (med_par > 0)
        std::cout << "Speedup Parallelo Base (AoS):  " << med_seq / med_par << "x\n";
    
    if (med_soa > 0)
        std::cout << "Speedup Parallelo Ott. (SoA):  " << med_seq / med_soa << "x\n";

    if (med_soa > 0)
        std::cout << "Efficienza SoA vs AoS:         " << med_par / med_soa << "x (guadagno memoria)\n";

    std::cout << std::string(40, '=') << "\n\n";

    return 0;
}