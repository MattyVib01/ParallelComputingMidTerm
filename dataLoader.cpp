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


std::vector<Point> loadDigitDataset() {
    std::vector<Point> dataset;
    // Assicurati di rinominare il file scaricato in "mnist.csv" e metterlo nella cartella
    std::ifstream file("datasets/mnist.csv"); 
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Errore: Impossibile aprire datasets/mnist.csv\n";
        return dataset;
    }

    // [!] TRUCCO MNIST 1: Leggiamo la prima riga a vuoto per saltare l'intestazione
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue; 
        
        std::stringstream ss(line);
        std::string token;
        std::vector<double> features;

        // [!] TRUCCO MNIST 2: Leggiamo la prima colonna (l'etichetta del numero 0-9) e la ignoriamo
        std::getline(ss, token, ','); 

        // Ora estraiamo i 784 pixel dell'immagine
        for (int i = 0; i < 784; ++i) {
            if (std::getline(ss, token, ',')) {
                try { 
                    features.push_back(std::stod(token)); 
                } catch (...) { 
                    break; 
                }
            }
        }
        
        // Aggiungiamo il punto solo se abbiamo letto esattamente 784 dimensioni
        if (features.size() == 784) {
            dataset.push_back(Point(features));
        }
    }
    
    return dataset;
}



std::vector<Point> loadGeneratedDataset() {
    std::vector<Point> dataset;
    std::ifstream file("datasets/generatedDataset.csv"); 
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Errore: Impossibile aprire datasets/generatedDataset.csv\n";
        return dataset;
    }

    // [!] TRUCCO 1: Leggiamo la primissima riga a vuoto per saltare l'intestazione (X1,X2,...)
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue; 
        
        std::stringstream ss(line);
        std::string token;
        std::vector<double> features;

        // [!] TRUCCO 2: Leggiamo DINAMICAMENTE tutte le colonne presenti.
        // Se domani generi 100 dimensioni, questo ciclo le leggerà tutte in automatico!
        while (std::getline(ss, token, ',')) {
            try { 
                features.push_back(std::stod(token)); 
            } catch (...) { 
                // Se c'è un errore di conversione (es. uno spazio vuoto imprevisto), lo ignoriamo
            }
        }
        
        // Aggiungiamo il punto al dataset solo se abbiamo estratto almeno una coordinata
        if (!features.empty()) {
            dataset.push_back(Point(features));
        }
    }
    
    return dataset;
}
