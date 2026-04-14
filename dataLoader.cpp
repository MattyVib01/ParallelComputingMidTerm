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
        if (line.empty()) continue; 
        
        std::stringstream ss(line);
        std::string token;
        std::vector<double> features;

    
        for (int i = 0; i < 4; ++i) {
            if (std::getline(ss, token, ',')) {
                try { 
                    features.push_back(std::stod(token)); 
                } catch (...) { 
                    break; 
                }
            }
        }
       
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

        std::getline(ss, token, ','); 


        for (int i = 0; i < 13; ++i) {
            if (std::getline(ss, token, ',')) {
                try { 
                    features.push_back(std::stod(token)); 
                } catch (...) { 
                    break; 
                }
            }
        }
        

        if (features.size() == 13) {
            dataset.push_back(Point(features));
        }
    }
    
    return dataset;
}


std::vector<Point> loadDigitDataset() {
    std::vector<Point> dataset;
    std::ifstream file("datasets/mnist.csv"); 
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Errore: Impossibile aprire datasets/mnist.csv\n";
        return dataset;
    }

    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue; 
        
        std::stringstream ss(line);
        std::string token;
        std::vector<double> features;
        std::getline(ss, token, ','); 

        for (int i = 0; i < 784; ++i) {
            if (std::getline(ss, token, ',')) {
                try { 
                    features.push_back(std::stod(token)); 
                } catch (...) { 
                    break; 
                }
            }
        }
        
        if (features.size() == 784) {
            dataset.push_back(Point(features));
        }
    }
    
    return dataset;
}



std::vector<Point> loadGeneratedDataset() {
    std::vector<Point> dataset;
    std::ifstream file("datasets/k500.csv"); 
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Errore: Impossibile aprire datasets/generatedDataset.csv\n";
        return dataset;
    }

    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue; 
        
        std::stringstream ss(line);
        std::string token;
        std::vector<double> features;

        while (std::getline(ss, token, ',')) {
            try { 
                features.push_back(std::stod(token)); 
            } catch (...) { 

            }
        }
        

        if (!features.empty()) {
            dataset.push_back(Point(features));
        }
    }
    
    return dataset;
}
