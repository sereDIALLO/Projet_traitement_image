#ifndef IMAGE_PROCESSING_HPP
#define IMAGE_PROCESSING_HPP

#include <opencv2/opencv.hpp>
#include <vector>

class ImageProcessing {
public:
    ImageProcessing(const std::string& imagePath);
    void processImage();
    void displayResults() const;
    void saveResults(const std::string& outputPath) const;
    void saveDataToCSV(const std::string& outputPath) const;

    

private:
    cv::Mat img;
    cv::Mat hsvImage;
    cv::Mat laser;
    cv::Mat laser_blur;
    cv::Mat img_canny;
    cv::Mat img_hough;
    cv::Mat dilatedLaser;
    cv::Mat dilated_mask_adventices;
    cv::Mat mask_plants;
    std::vector<cv::Point> intersections;

    bool findIntersection(cv::Point p1, cv::Point p2, cv::Point p3, cv::Point p4, cv::Point& intersection);
    void detectLaser();
    void detectLinesAndIntersections();
    void processPlantsAndWeeds();
    void calculateAverageIntersection();
};

#endif // IMAGE_PROCESSING_HPP
