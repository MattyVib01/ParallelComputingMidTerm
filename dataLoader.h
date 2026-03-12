#ifndef DATALOADER_H
#define DATALOADER_H

#include <vector>
#include <string>
#include "Point.h"
#include "kMeansSeq.h" 
#include "kMeansPar.h"

std::vector<Point> loadIrisDataset();
std::vector<Point> loadWineDataset();
std::vector<Point> loadDigitDataset();
std::vector<Point> loadGeneratedDataset();

#endif