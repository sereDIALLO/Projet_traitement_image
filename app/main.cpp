#include "../include/ImageProcessor.hpp"
#include <filesystem> // Pour parcourir le répertoire des images
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
int main(int argc, char** argv) 
{
    // Initialisation du répertoire d'entrée et de sortie
    std::string inputDir = "/Users/mar1/HOME/Github/data1/input";
    std::string outputDir = "/Users/mar1/HOME/Github/data1/output";


        if (!std::__fs::filesystem::exists(inputDir)) {
        std::cerr << "Le répertoire d'entrée n'existe pas : " << inputDir << std::endl;
        return -1;
    } else {
        std::cout << "Répertoire d'entrée trouvé. Contenu : " << std::endl;
        for (const auto& entry : std::__fs::filesystem::directory_iterator(inputDir)) {
            std::cout << entry.path() << std::endl;
        }
    }

    for (const auto& entry : std::__fs::filesystem::directory_iterator(inputDir)) {
        std::string inputImagePath = entry.path().string();
        std::string fileName = entry.path().filename().string();
        std::string outputImagePath = outputDir + "/" + fileName.substr(0, fileName.find_last_of('.')) + "_result.png";

        // Charger l'image
        Mat img = imread(inputImagePath);
        if (img.empty()) {
            std::cerr << "Erreur: Impossible de charger l'image : " << inputImagePath << std::endl;
            continue;
        }

        // Créer une instance de ImageProcessor pour traiter l'image
        ImageProcessor processor(inputImagePath);

        // Traiter l'image et sauvegarder le résultat
        processor.processImage(outputImagePath);
    }

    return 0;
}
