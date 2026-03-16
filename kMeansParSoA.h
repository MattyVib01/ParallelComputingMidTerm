#ifndef KMEANSPARSOA_H
#define KMEANSPARSOA_H

#include "DatasetSoA.h"

// Esegue l'algoritmo K-Means in parallelo sfruttando l'architettura SoA
// Nota: Passiamo DatasetSoA per riferimento (&) in modo da poter aggiornare i cluster direttamente
void kMeansParSoA(DatasetSoA& dataset, int K, int max_iterations);

#endif // KMEANSPARSOA_H