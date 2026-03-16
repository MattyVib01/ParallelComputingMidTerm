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
#include "DatasetSoA.h"   // [!] Nuovo Header SoA
#include "kMeansParSoA.h" // [!] Nuovo Header Funzione SoA
#include "dataLoader.h"

// Funzione di stampa statistica (invariata)
double printStats(const std::string& label, const std::vector<double>& times) {
    double sum = std::accumulate(times.begin(), times.end(), 0.0);
    double mean = sum / times.size();
    double min_t = *std::min_element(times.begin(), times.end());
    double max_t = *std::max_element(times.begin(), times.end());
    
    double sq_sum = std::inner_product(times.begin(), times.end(), times.begin(), 0.0);
    double std_dev = std::sqrt(sq_sum / times.size() - mean * mean);

    std::cout << label << "\n";
    std::cout << "  Media: " << mean * 1000.0 << " ms\n";
    std::cout << "  Min:   " << min_t * 1000.0 << " ms\n";
    std::cout << "  Max:   " << max_t * 1000.0 << " ms\n";
    std::cout << "  StDev: " << std_dev * 1000.0 << " ms\n\n";

    return mean; 
}

int main() {
    std::vector<Point> dataset_original = loadGeneratedDataset();
    if (dataset_original.empty()) return 1;

    // [!] FASE 4: Prepariamo subito la versione SoA del dataset originale
    DatasetSoA dataset_soa_original = convertToSoA(dataset_original);

    std::cout << "Caricati " << dataset_original.size() << " punti.\n\n";

    int K = 10; 
    int max_iters = 200;
    int num_runs = 5; 

    // Abbiamo stabilito che 8 è il tuo sweet spot per l'i5
    int num_threads = 8; 
    omp_set_num_threads(num_threads);

    // Vettori per memorizzare i tempi delle 3 versioni
    std::vector<double> seq_wall_times, par_wall_times, soa_wall_times;

    std::cout << "Esecuzione di " << num_runs << " run (Seq vs Par AoS vs Par SoA con " << num_threads << " thread)...\n\n";

    for (int r = 0; r < num_runs; ++r) {
        // Copie pulite per ogni iterazione
        std::vector<Point> dataset_seq = dataset_original;
        std::vector<Point> dataset_par = dataset_original;
        DatasetSoA dataset_soa = dataset_soa_original; // Copia del SoA

        // --- 1. MISURAZIONE SEQUENZIALE ---
        double wall_start_seq = omp_get_wtime();
        kMeansSeq(dataset_seq, K, max_iters);
        double wall_end_seq = omp_get_wtime();
        seq_wall_times.push_back(wall_end_seq - wall_start_seq);

        // --- 2. MISURAZIONE PARALLELA (AoS) ---
        double wall_start_par = omp_get_wtime();
        kMeansPar(dataset_par, K, max_iters);
        double wall_end_par = omp_get_wtime();
        par_wall_times.push_back(wall_end_par - wall_start_par);

        // --- 3. MISURAZIONE PARALLELA (SoA - Ottimizzata in Memoria) ---
        double wall_start_soa = omp_get_wtime();
        kMeansParSoA(dataset_soa, K, max_iters);
        double wall_end_soa = omp_get_wtime();
        soa_wall_times.push_back(wall_end_soa - wall_start_soa);
    }

    // --- STAMPA DEI RISULTATI STATISTICI ---
    std::cout << "=== RISULTATI SEQUENZIALE ===\n";
    double avg_seq = printStats("Wall-clock time:", seq_wall_times);

    std::cout << "=== RISULTATI PARALLELO AoS (" << num_threads << " THREAD) ===\n";
    double avg_par = printStats("Wall-clock time:", par_wall_times);

    std::cout << "=== RISULTATI PARALLELO SoA (" << num_threads << " THREAD) ===\n";
    double avg_soa = printStats("Wall-clock time:", soa_wall_times);

    // --- CALCOLO DEGLI SPEEDUP ---
    std::cout << "=== ANALISI SPEEDUP ===\n";
    std::cout << "Speedup Parallelo Base (AoS):  " << avg_seq / avg_par << "x\n";
    std::cout << "Speedup Parallelo Ott. (SoA):  " << avg_seq / avg_soa << "x\n\n";

    return 0;
}