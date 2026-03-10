#ifndef DATALOADER_H
#define DATALOADER_H

#include <vector>
#include <string>
#include "Point.h"
#include "kMeansSeq.h" 
#include "kMeansPar.h"

// Dichiarazione della funzione di caricamento
std::vector<Point> loadIrisDataset();
std::vector<Point> loadWineDataset();

#endif