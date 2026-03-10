#include "dataLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::vector<Point> loadIrisDataset() {
    std::vector<Point> dataset;
    std::ifstream file("datasets/iris.csv");
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Errore: Impossibile aprire il file ";
        return dataset;
    }

    while (std::getline(file, line)) {
        if (line.empty()) continue; // Salta eventuali righe vuote
        
        std::stringstream ss(line);
        std::string token;
        std::vector<double> features;

        // Estraiamo solo le prime 4 colonne (le feature numeriche)
        for (int i = 0; i < 4; ++i) {
            if (std::getline(ss, token, ',')) {
                try { 
                    features.push_back(std::stod(token)); 
                } catch (...) { 
                    // Se fallisce la conversione, interrompiamo la lettura della riga
                    break; 
                }
            }
        }
        
        // Aggiungiamo il punto al dataset solo se abbiamo estratto esattamente 4 numeri
        if (features.size() == 4) {
            dataset.push_back(Point(features));
        }
    }
    
    return dataset;
}

std::vector<Point> loadWineDataset() {
    std::vector<Point> dataset;
    std::ifstream file("datasets/wine.csv");
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Errore: Impossibile aprire il file ";
        return dataset;
    }

    while (std::getline(file, line)) {
        if (line.empty()) continue; 
        
        std::stringstream ss(line);
        std::string token;
        std::vector<double> features;

        // [!] TRUCCO WINE: Leggiamo la prima colonna (la classe 1, 2 o 3) e NON la salviamo
        std::getline(ss, token, ','); 

        // Ora estraiamo le 13 feature chimiche vere e proprie
        for (int i = 0; i < 13; ++i) {
            if (std::getline(ss, token, ',')) {
                try { 
                    features.push_back(std::stod(token)); 
                } catch (...) { 
                    break; 
                }
            }
        }
        
        // Aggiungiamo il punto solo se abbiamo letto esattamente 13 numeri
        if (features.size() == 13) {
            dataset.push_back(Point(features));
        }
    }
    
    return dataset;
}
