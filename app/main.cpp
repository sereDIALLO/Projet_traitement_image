#include "../include/ImageProcessor.hpp"

int main() 
{
    ImageProcessor ImageProcessor("../data/im001.png");
    ImageProcessor.processImage();
    ImageProcessor.findIntersections();
    ImageProcessor.displayResults();
    ImageProcessor.saveResult("../resultats/test.png");

    return 0;
}ImageProcessor