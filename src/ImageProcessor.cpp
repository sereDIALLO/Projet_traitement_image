#include "ImageProcessor.hpp"
#include <iostream>

using namespace cv;
using namespace std;

ImageProcessor::ImageProcessor(const string& imagePath) {
    img = imread(imagePath);
    if (img.empty()) {
        cerr << "Erreur: Impossible de charger l'image!" << endl;
        exit(-1);
    }
    img_hough = img.clone();
}

void ImageProcessor::processImage(const std::string& outputPath) { //!Marwane 
    cvtColor(img, hsvImage, COLOR_BGR2HSV);
    createLaserMask();
    detectContours();
    createPlantAndWeedMasks();
    cv::imwrite(outputPath, img);
}

void ImageProcessor::findIntersections() {
    detectContours();
}

void ImageProcessor::createLaserMask() {
    Scalar lowerGreen(40, 60, 60);
    Scalar upperGreen(90, 255, 255);
    inRange(hsvImage, lowerGreen, upperGreen, laser);
    GaussianBlur(laser, laser_blur, Size(5, 5), 0);
}

void ImageProcessor::detectContours() {
    Canny(laser_blur, img_canny, 50, 150, 3);
    vector<Vec4i> lines;
    HoughLinesP(img_canny, lines, 1, CV_PI / 180, 30, 30, 5);

    for (size_t i = 0; i < lines.size(); ++i) {
        Vec4i line1 = lines[i];
        line(img_hough, Point(line1[0], line1[1]), Point(line1[2], line1[3]), Scalar(0, 255, 0), 1, LINE_AA);

        for (size_t j = i + 1; j < lines.size(); ++j) {
            Vec4i line2 = lines[j];
            Point intersection;
            if (findIntersection(Point(line1[0], line1[1]), Point(line1[2], line1[3]),
                                 Point(line2[0], line2[1]), Point(line2[2], line2[3]),
                                 intersection)) {
                intersections.push_back(intersection);
            }
        }
    }
    calculateAverageIntersection();
}

bool ImageProcessor::findIntersection(Point p1, Point p2, Point p3, Point p4, Point& intersection) {
    float a1 = p2.y - p1.y;
    float b1 = p1.x - p2.x;
    float c1 = a1 * p1.x + b1 * p1.y;

    float a2 = p4.y - p3.y;
    float b2 = p3.x - p4.x;
    float c2 = a2 * p3.x + b2 * p3.y;

    float delta = a1 * b2 - a2 * b1;

    if (abs(delta) < 1e-6) {
        return false;
    }

    intersection.x = (b2 * c1 - b1 * c2) / delta;
    intersection.y = (a1 * c2 - a2 * c1) / delta;

    return !(intersection.x < min(p1.x, p2.x) || intersection.x > max(p1.x, p2.x) ||
             intersection.x < min(p3.x, p4.x) || intersection.x > max(p3.x, p4.x) ||
             intersection.y < min(p1.y, p2.y) || intersection.y > max(p1.y, p2.y) ||
             intersection.y < min(p3.y, p4.y) || intersection.y > max(p3.y, p4.y));
}

void ImageProcessor::calculateAverageIntersection() {
    if (!intersections.empty()) {
        Point avgIntersection(0, 0);
        for (const auto& pt : intersections) {
            avgIntersection.x += pt.x;
            avgIntersection.y += pt.y;
        }
        avgIntersection.x /= intersections.size();
        avgIntersection.y /= intersections.size();
        circle(img_hough, avgIntersection, 8, Scalar(0, 0, 255), -1);
        cout << "Point d'intersection moyen: (" << avgIntersection.x << ", " << avgIntersection.y << ")" << endl;
    }
}

void ImageProcessor::createPlantAndWeedMasks() {
    vector<Mat> hsv_planes;
    split(hsvImage, hsv_planes);
    img_h = hsv_planes[0];
    img_s = hsv_planes[1];
    img_v = hsv_planes[2];

    inRange(img_h, Scalar(100, 120, 255), Scalar(121, 255, 255), mask_plants);
    medianBlur(mask_plants, dilatedLaser, 39);

    int erosionSize = 2;
    Mat erosionElement = getStructuringElement(MORPH_RECT, Size(2 * erosionSize + 1, 2 * erosionSize + 1));
    erode(dilatedLaser, dilatedLaser, erosionElement);
    int dilationSize = 14;
    Mat dilationElement = getStructuringElement(MORPH_RECT, Size(2 * dilationSize + 1, 2 * dilationSize + 1));
    dilate(dilatedLaser, dilatedLaser, dilationElement);
}

void ImageProcessor::displayResults() {
    displayImage("Image d'origine", img);
    displayImage("Laser", laser);
    displayImage("Gaussian Blur", laser_blur);
    displayImage("Image de contour", img_canny);
    displayImage("Hough", img_hough);
    waitKey(0);
}

void ImageProcessor::displayImage(const std::string& windowName, const cv::Mat& img) {
    namedWindow(windowName, WINDOW_AUTOSIZE);
    imshow(windowName, img);
}

void ImageProcessor::saveResult(const std::string& outputPath) {
    imwrite(outputPath, img_hough);
}
