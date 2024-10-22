 #ifndef LASERDETECTION_HPP
#define LASERDETECTION_HPP

#include <opencv2/opencv.hpp>
#include <vector>

class LaserDetection
{
public:
    LaserDetection(const std::string& imagePath);
    void processImage();
    void findIntersections();
    void displayResults();
    void saveResult(const std::string& outputPath);

private:
    cv::Mat img, hsvImage, laser, laser_blur, img_canny, img_hough;
    cv::Mat img_h, img_s, img_v, mask_plants, dilatedLaser;
    cv::Mat dilated_mask_adventices;
    std::vector<cv::Point> intersections;

    bool findIntersection(cv::Point p1, cv::Point p2, cv::Point p3, cv::Point p4, cv::Point& intersection);
    void createLaserMask();
    void detectContours();
    void calculateAverageIntersection();
    void createPlantAndWeedMasks();
    void displayImage(const std::string& windowName, const cv::Mat& img);
};

#endif // LASERDETECTION_HPP
