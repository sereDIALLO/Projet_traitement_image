#include "../include/LaserDetection.hpp"
#include <vector>
#include <string>
#include <fstream>

int main(int argc, char** argv) 
{
    try {
        // Liste des chemins des images à traiter
        std::vector<std::string> imagePaths = {
            "../data/images/im001.png",
            "../data/images/im002.png",
            "../data/images/im003.png",
            "../data/images/im004.png",
            "../data/images/im005.png",
            "../data/images/im006.png"
        };

        // Parcourir chaque image dans la liste
        for (const auto& imagePath : imagePaths) {
            std::cout << "Processing: " << imagePath << std::endl;

            // Initialiser le processeur d'image avec le chemin de l'image actuelle
            ImageProcessing imageProcessor(imagePath);

            // Traiter l'image
            imageProcessor.processImage();

            // Afficher les résultats
            imageProcessor.displayResults();

            // Définir le chemin de sauvegarde pour les résultats
            std::string resultPath = "../resultats/result_" + imagePath.substr(imagePath.find_last_of("/") + 1);
            imageProcessor.saveResults(resultPath);

            // Sauvegarder les données dans un fichier CSV
            std::string csvPath = "../data/donnee/donne.csv";
            imageProcessor.saveDataToCSV(csvPath);

            std::cout << "Result saved to: " << resultPath << std::endl;
            std::cout << "Data saved to: " << csvPath << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
