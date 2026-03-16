#ifndef DATASETSOA_H
#define DATASETSOA_H

#include <vector>
#include "Point.h" // Necessario per leggere la vecchia struttura durante la conversione

// La nostra nuova struttura Structure of Arrays
struct DatasetSoA {
    int num_points;
    int dimensions;
    
    // Array 1D piatto per massimizzare le prestazioni della cache (Row-major layout)
    std::vector<double> coords; 
    
    // Array separato per i cluster
    std::vector<int> clusters;  
};

// Funzione helper per convertire il dataset dal vecchio formato (AoS) al nuovo (SoA)
inline DatasetSoA convertToSoA(const std::vector<Point>& aos_dataset) {
    DatasetSoA soa;
    if (aos_dataset.empty()) {
        soa.num_points = 0;
        soa.dimensions = 0;
        return soa;
    }

    soa.num_points = aos_dataset.size();
    soa.dimensions = aos_dataset[0].values.size();
    
    // Pre-allochiamo esattamente la memoria necessaria in un blocco unico contiguo
    soa.coords.resize(soa.num_points * soa.dimensions);
    soa.clusters.resize(soa.num_points, -1);

    for (int i = 0; i < soa.num_points; ++i) {
        soa.clusters[i] = aos_dataset[i].cluster;
        for (int d = 0; d < soa.dimensions; ++d) {
            // Matematica dell'indice piatto: (riga * numero_colonne) + colonna
            soa.coords[i * soa.dimensions + d] = aos_dataset[i].values[d];
        }
    }
    
    return soa;
}

#endif // DATASETSOA_H