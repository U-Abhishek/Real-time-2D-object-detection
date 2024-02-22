# include "abhishek.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <opencv2/opencv.hpp>

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
/// @param src Source Image
/// @param dst dstination image to store the sementation map
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


