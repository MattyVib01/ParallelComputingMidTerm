#pragma once
#include <vector>
#include "Point.h"

// Questa singola struttura sostituisce il vector<Point>
struct DatasetSoA {
    int num_points;
    int dimensions;
    
    // Un unico, enorme blocco di memoria contigua per tutti i numeri.
    // Nessuna frammentazione. Massima felicità per la Cache L1/L2.
    std::vector<double> coords; 
    
    // Un array separato solo per le etichette dei cluster
    std::vector<int> clusters;  
};

// Funzione di utilità per convertire il vecchio formato nel nuovo
// (Così non devi riscrivere il dataLoader.h)
inline DatasetSoA convertToSoA(const std::vector<Point>& aos_dataset) {
    DatasetSoA soa;
    if (aos_dataset.empty()) return soa;

    soa.num_points = aos_dataset.size();
    soa.dimensions = aos_dataset[0].values.size();
    
    soa.coords.resize(soa.num_points * soa.dimensions);
    soa.clusters.resize(soa.num_points, -1); // Inizializza a -1

    // Riempiamo l'array piatto in formato Row-Major (Punto 1, Punto 2, ...)
    for (int i = 0; i < soa.num_points; ++i) {
        soa.clusters[i] = aos_dataset[i].cluster;
        for (int d = 0; d < soa.dimensions; ++d) {
            // Indice piatto: (riga * numero_colonne) + colonna
            soa.coords[i * soa.dimensions + d] = aos_dataset[i].values[d];
        }
    }
    
    return soa;
}