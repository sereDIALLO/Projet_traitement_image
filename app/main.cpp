#include "../include/ImageProcessor.hpp"
#include <filesystem> // Pour parcourir le répertoire des images
#include <iostream>


int main(int argc, char** argv) {
    // Initialisation du répertoire d'entrée et de sortie
    std::string inputDir = "../data/input";
    std::string outputDir = "../data/output";




    // Parcourir toutes les images du répertoire d'entrée
    for (const auto& entry : std::__fs::filesystem::directory_iterator(inputDir)) {

        std::string imagePath = entry.path().string();

        // Créer une instance de ImageProcessor pour traiter l'image
        ImageProcessor processor(imagePath);

        // Traiter l'image et sauvegarder le résultat dans data/output
        std::string outputFileName = outputDir + "/" + entry.path().filename().string();
        processor.processImage(outputFileName);
    }

    return 0;
}

/*
    ImageProcessor ImageProcessor("../data/input/im003.png");
    ImageProcessor.processImage();
    ImageProcessor.findIntersections();
    ImageProcessor.displayResults();
    ImageProcessor.saveResult("../resultats/test.png");

    return 0;
}
*/