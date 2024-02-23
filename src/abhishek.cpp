#include "abhishek.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
//#include <opencv2/core/eigen.hpp>

using namespace cv;
using namespace std;

//////////////// Task 3 /////////////////////////////////

const cv::Point connected_points[4] =
{
    cv::Point(1, 0),
    cv::Point(0, -1),
    cv::Point(-1, 0),
    cv::Point(0, 1)
};


/// @brief This fuction is subpart of region growth algorithm
/// @param src 
/// @param dst 
/// @param seed_point The point which should be start of region growth
/// @param region_id The unique ientifies of regions 
void growth(cv::Mat& src, cv::Mat& dst, cv::Point seed_point, int region_id){
    // Step 1: Creating stack
    stack<cv::Point> point_stack;
    point_stack.push(seed_point);
    // Step 2: loop till the stack is empty 
    while (!point_stack.empty())
    {
        cv::Point center = point_stack.top();
        dst.at<uchar>(center) = region_id;
        point_stack.pop();
        // Find the nighbors of the center pointH
        for (int i=0; i<4; ++i) {
            cv::Point neighboring_point = center + connected_points[i];
            if (neighboring_point.x < 0 || neighboring_point.x > src.cols-1 || neighboring_point.y < 0 || neighboring_point.y > src.rows-1) {
                continue;
            }
            else{
                if (src.at<uchar>(neighboring_point) == 255 && dst.at<uchar>(neighboring_point) == 0){
                    point_stack.push(neighboring_point);
                }
            }
        }
    }
    }


    void growth(cv::Mat& src, cv::Mat& dst, cv::Point seed_point, int region_id);

    /// @brief Applies region growth algorithm to find segments in the image
    /// @param src binary filtered image
    /// @param dst image to store the sementation map (region IDs are stored as pixel value starting form 1 to n regions)
    int region_growth(cv::Mat& src, cv::Mat& dst){
    // Step 1: Iterate throught image rows and cols
    int region_id = 1;
    for (int y=0; y<src.rows; y++) {
        for (int x=0; x<src.cols; x++) {
            // Step 2: Find the seed pixel
            // Seed pixel -> Pixel == Foreground pixel and pixel is not part of any segment created before
            if (src.at<uchar>(cv::Point(x, y)) == 255 && dst.at<uchar>(cv::Point(x, y)) == 0) {
                // Step 3: After seed pixel is found pass it through growth
                growth(src, dst, cv::Point(x, y), region_id);
                region_id +=1;
            }
        }
    }
    return region_id - 1;
    }

    //////////////// Task 4 /////////////////////////////////




    /// @brief 
    /// @param region_map is the region map
    /// @param region_id The region id of segment the features need to be extracted
    /// @return 0 on success
    int feature_extraction(cv::Mat& region_map, int region_id){
    // Step 1: Creating binary image from Region Map and Region ID
    Mat bin_image = (region_map == region_id);
    cv::imshow("Bin Image", bin_image);
    cv::waitKey(0);
    // Step 2: Extracting Features form the binary image
    cv::Moments m = cv::moments(bin_image, true);

    /////////////////////////////////////////////////// Eigen Vector calculation /////////////////////////////////

    // Assuming u02, u11, u20, and m00 are defined
    double u02 = m.mu02;
    double u11 = m.mu11;
    double u20 = m.mu20;
    double m00 = m.m00;

    // Calculate the elements of the second order central moment tensor
    double mu[2][2] = {
        {u02 / m00, u11 / m00},
        {u11 / m00, u20 / m00}
    };

    // Convert the 2x2 matrix to an OpenCV matrix
    cv::Mat muMat(2, 2, CV_64FC1, mu);

    // Compute eigenvalues and eigenvectors
    cv::Mat eigenvalues, eigenvectors;
    cv::eigen(muMat, eigenvalues, eigenvectors);

    // Extract eigenvalues
    double l1 = eigenvalues.at<double>(0);
    double l2 = eigenvalues.at<double>(1);

    // Extract eigenvectors
    double v1x = eigenvectors.at<double>(0, 0);
    double v1y = eigenvectors.at<double>(1, 0);
    double v2x = eigenvectors.at<double>(0, 1);
    double v2y = eigenvectors.at<double>(1, 1);

    // Compute the orientation (phi)
    double phi = std::atan2(v1y, v1x);

    // Output the results
    std::cout << "Eigenvalues:" << std::endl;
    std::cout << "l1: " << l1 << std::endl;
    std::cout << "l2: " << l2 << std::endl;
    std::cout << "Orientation (phi): " << phi << std::endl;

    // Calculate w and h
    double w = std::sqrt(12 * l1);
    double h = std::sqrt(12 * l2);

    // Output the results
    std::cout << "Height (H): " << h << std::endl;
    std::cout << "Width (W): " << w << std::endl;
    std::cout << "Area of the segment: " << m.m00 << std::endl;
    std::cout << "Area of the min box: " << w*h << std::endl;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // 2.1) Feature 1:  % filled
    /*
    i/p : m_00 or mu_00 = Area of the segment, min bounding box Height, min bounding box width
    */
    double percent_filled = m.m00/ (w*h);
    std::cout << "percent filled : " << percent_filled << std::endl;
    // 2.2) Feature 2: Bounding box Hight/Width ratio
    /*
    i/p : min bounding box Height, min bounding box width
    */
    double HW_ratio = h/w;
    std::cout << "H W ratio: " << HW_ratio << std::endl;


    return 0;
    }