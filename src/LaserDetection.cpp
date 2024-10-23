#include "LaserDetection.hpp"
#include <iostream>
#include <fstream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

ImageProcessing::ImageProcessing(const std::string& imagePath) {
    img = imread(imagePath);
    if (img.empty()) {
        cerr << "Erreur: Impossible de charger l'image!" << endl;
        throw std::runtime_error("Image not loaded");
    }
}

void ImageProcessing::processImage() 
{
    detectLaser();
    detectLinesAndIntersections();
    processPlantsAndWeeds();
    calculateAverageIntersection();
}

bool ImageProcessing::findIntersection(Point p1, Point p2, Point p3, Point p4, Point& intersection) 
{
    float a1 = p2.y - p1.y;
    float b1 = p1.x - p2.x;
    float c1 = a1 * p1.x + b1 * p1.y;
    float a2 = p4.y - p3.y;
    float b2 = p3.x - p4.x;
    float c2 = a2 * p3.x + b2 * p3.y;
    float delta = a1 * b2 - a2 * b1;

    if (abs(delta) < 1e-6) return false;

    intersection.x = (b2 * c1 - b1 * c2) / delta;
    intersection.y = (a1 * c2 - a2 * c1) / delta;

    if (intersection.x < min(p1.x, p2.x) || intersection.x > max(p1.x, p2.x) ||
        intersection.x < min(p3.x, p4.x) || intersection.x > max(p3.x, p4.x) ||
        intersection.y < min(p1.y, p2.y) || intersection.y > max(p1.y, p2.y) ||
        intersection.y < min(p3.y, p4.y) || intersection.y > max(p3.y, p4.y)) {
        return false;
    }

    return true;
}

void ImageProcessing::detectLaser() 
{
    cvtColor(img, hsvImage, COLOR_BGR2HSV);
    Scalar lowerGreen(60, 50, 60), upperGreen(100, 255, 255);
    inRange(hsvImage, lowerGreen, upperGreen, laser);
    GaussianBlur(laser, laser_blur, Size(7 ,7), 1,9);
    Canny(laser_blur, img_canny, 50, 150, 3);
}


void ImageProcessing::detectLinesAndIntersections() 
{
    std::vector<cv::Vec4i> lines;
    HoughLinesP(img_canny, lines, 1, CV_PI / 180, 50, 50, 10);
    img_hough = img.clone();

    // Point de référence
    Point referencePoint(846, 522);
    int threshold = 200; // seuil de distance

    for (size_t i = 0; i < lines.size(); ++i) {
        Vec4i line1 = lines[i];
        line(img_hough, Point(line1[0], line1[1]), Point(line1[2], line1[3]), Scalar(0, 255, 0), 1.5, LINE_AA);

        for (size_t j = i + 1; j < lines.size(); ++j) 
        {
            Vec4i line2 = lines[j];
            Point intersection;
            if (findIntersection(Point(line1[0], line1[1]), Point(line1[2], line1[3]),
                                 Point(line2[0], line2[1]), Point(line2[2], line2[3]), intersection)) 
            {
                // Calculer la distance entre l'intersection et le point de référence
                double distance = norm(intersection - referencePoint);
                if (distance <= threshold) {
                    // Garder l'intersection
                    intersections.push_back(intersection);
                    //circle(img_hough, intersection, 7, Scalar(0, 0, 255), -1); // Cercle pour marquer l'intersection
                    //cout << "Point d'intersection proche: (" << intersection.x << ", " << intersection.y << ")" << endl;
                }
            }
        }
    }
}




void ImageProcessing::processPlantsAndWeeds() {
    // Extraction des canaux H, S, V
    std::vector<cv::Mat> hsv_planes;
    split(hsvImage, hsv_planes);
    Mat img_h = hsv_planes[0];
    Mat img_s = hsv_planes[1];
    Mat img_v = hsv_planes[2];

    // Masque des plantes
    Mat plants;
    inRange(img_h, Scalar(100, 120, 255), Scalar(121, 255, 255), plants);
    Mat mask_plants;
    medianBlur(plants, mask_plants, 39);
    cv::namedWindow("masque plante", WINDOW_AUTOSIZE);
    cv::imshow("masque plante", mask_plants);

    // Érosion et dilatation pour affiner le masque des plantes
    Mat erodedLaser;
    int erosionSize = 2;
    Mat erosionElement = getStructuringElement(MORPH_RECT, Size(2 * erosionSize + 1, 2 * erosionSize + 1));
    erode(mask_plants, erodedLaser, erosionElement);

    Mat dilatedLaser;
    int dilationSize = 14;
    Mat dilationElement = getStructuringElement(MORPH_RECT, Size(2 * dilationSize + 1, 2 * dilationSize + 1));
    dilate(erodedLaser, dilatedLaser, dilationElement);

    // Masque des adventices
    Mat petit_mask;
    medianBlur(plants, petit_mask, 15);
    bitwise_not(petit_mask, petit_mask);
    Mat mask_adventices = dilatedLaser + petit_mask;
    
   

    // Réduction du bruit dans l'image
    Mat mask_adventices_blur;
    medianBlur(mask_adventices, mask_adventices_blur, 19);
    Mat mask_adventices_inverse;
    bitwise_not(mask_adventices_blur, mask_adventices_inverse);
    cv::namedWindow("masque adventice", WINDOW_AUTOSIZE);
    cv::imshow("masque adventice", mask_adventices_inverse);

    // Dilatation pour affiner le masque des adventices
    Mat dilated_mask_adventices;
    int dilatation_size_adventices = 12;
    Mat dilatation_adventices = getStructuringElement(MORPH_RECT, Size(2 * dilatation_size_adventices + 1, 2 * dilatation_size_adventices + 1));
    dilate(mask_adventices_inverse, dilated_mask_adventices, dilatation_adventices);

    // Détection des composants connexes pour les adventices
    Mat label_adventices, stats_adventices, centroids_adventices;
    int n_components_adventices = connectedComponentsWithStats(dilated_mask_adventices, label_adventices, stats_adventices, centroids_adventices, 8, CV_32S);
    for (int i = 1; i < n_components_adventices; ++i) {
        double x = centroids_adventices.at<double>(i, 0);
        double y = centroids_adventices.at<double>(i, 1);
        circle(img_hough, Point(static_cast<int>(x), static_cast<int>(y)), 5, Scalar(0, 0, 255), -1);
        cout << "Adventice " << i << ": Centre de gravité = (" << x << ", " << y << ")" << endl;
    }

    // Détection des composants connexes pour les plantes
    Mat label, stats, centroids;
    int nComponents = connectedComponentsWithStats(dilatedLaser, label, stats, centroids, 8, CV_32S);
    for (int i = 1; i < nComponents; ++i) {
        double x = centroids.at<double>(i, 0);
        double y = centroids.at<double>(i, 1);
        circle(img_hough, Point(static_cast<int>(x), static_cast<int>(y)), 5, Scalar(255), -1);
        cout << "Plante " << i << ": Centre de gravité = (" << x << ", " << y << ")" << endl;
    }
}


void ImageProcessing::calculateAverageIntersection() 
{
    if (!intersections.empty()) 
    {
        Point avgIntersection(0, 0);
        for (const auto& pt : intersections) {
            avgIntersection.x += pt.x;
            avgIntersection.y += pt.y;
        }
        avgIntersection.x /= intersections.size();
        avgIntersection.y /= intersections.size();
        circle(img_hough, avgIntersection, 8, Scalar(0, 0, 255), -1);
        cout << "Point d'intersection proche: (" << avgIntersection.x << ", " << avgIntersection.y << ")" << endl;
    }
}

void ImageProcessing::displayResults() const 
{
    namedWindow("Image d'origine", WINDOW_AUTOSIZE);
    imshow("Image d'origine", img);
    namedWindow("Hough", WINDOW_AUTOSIZE);
    imshow("Hough", img_hough);

    namedWindow("hsv", WINDOW_AUTOSIZE);
    imshow("hsv", hsvImage);


    //namedWindow("masque plant", WINDOW_AUTOSIZE);
    //imshow("masque plant", dilatedLaser);

    //namedWindow("masque adventice", WINDOW_AUTOSIZE);
    //imshow("masque adventice", dilated_mask_adventices);
    waitKey(0);
}

void ImageProcessing::saveResults(const std::string& outputPath) const 
{
    imwrite(outputPath, img_hough);
}

void ImageProcessing::saveDataToCSV(const std::string& outputPath) const 
{
    std::ofstream csvFile(outputPath);
    if (!csvFile.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier CSV pour écrire!" << std::endl;
        return;
    }

    // Écrire l'en-tête
    csvFile << "Type,Centre de gravité X,Centre de gravité Y\n";

    // Écrire les centres de gravité des adventices
    for (size_t i = 0; i < intersections.size(); ++i) {
        csvFile << "Adventice " << (i + 1) << "," << intersections[i].x << "," << intersections[i].y << "\n";
    }

    // Écrire les centres de gravité des plantes
    for (size_t i = 0; i < intersections.size(); ++i) {
        csvFile << "Plante " << (i + 1) << "," << intersections[i].x << "," << intersections[i].y << "\n";
    }

    csvFile.close();
}
