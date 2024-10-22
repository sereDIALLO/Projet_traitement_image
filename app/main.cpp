#include "../include/LaserDetection.hpp"

int main() 
{
    LaserDetection laserDetection("../data/im001.png");
    laserDetection.processImage();
    laserDetection.findIntersections();
    laserDetection.displayResults();
    laserDetection.saveResult("../resultats/test.png");

    return 0;
}
