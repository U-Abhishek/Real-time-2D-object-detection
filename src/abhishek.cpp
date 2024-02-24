#include <filesystem>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include "abhishek.h"
#include "csv_util.h"
//#include <opencv2/core/eigen.hpp>

using namespace cv;
using namespace std;

namespace fs = std::filesystem;

/// @brief Takes Folder path as input and return a vector of all file paths
/// @param directory_path 
/// @return file_path vector of strings 
std::vector<std::string> getFilesInDirectory(const std::string& directory_path) {
    std::vector<std::string> file_paths;
    // This structure would distinguish a file from a directory
    struct stat sb;
    // Loop through the directory
    for(const auto& entry : fs::directory_iterator(directory_path)) {
        // Convert the path to a string
        std::filesystem::path outfilename = entry.path();
        std::string outfilename_str = outfilename.string();
        // Convert the string path to a const char*
        const char* path_c_str = outfilename_str.c_str();
        // Check if it's a file and not a directory
        if (stat(path_c_str, &sb) == 0 && !(sb.st_mode & S_IFDIR)) {
            // Add the path to the vector
            std::string extension = outfilename.extension().string();
            if (extension == ".jpg" || extension == ".png") {
                // Add the path to the vector
                file_paths.push_back(outfilename_str);
            }
        }
    }
    return file_paths;
}

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
void growth(cv::Mat& src, cv::Mat& dst, cv::Point seed_point, int region_id, int region_area){
    // Step 1: Creating stack
    stack<cv::Point> point_stack;
    point_stack.push(seed_point);
    // Step 2: loop till the stack is empty 
    //int region_area = 1;
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
        //region_area++;
    }
    }


void growth(cv::Mat& src, cv::Mat& dst, cv::Point seed_point, int region_id);

/// @brief Applies region growth algorithm to find segments in the image
/// @param src binary filtered image
/// @param dst image to store the sementation map (region IDs are stored as pixel value starting form 1 to n regions)
/// @return regioin id of maximum area
int region_growth(cv::Mat& src, cv::Mat& dst){
    // Step 1: Iterate throught image rows and cols
    int region_id = 1;
    int region_area;
    int max_area = -1;
    int max_region_id = -1;
    for (int y=0; y<src.rows; y++) {
        for (int x=0; x<src.cols; x++) {
            // Step 2: Find the seed pixel
            // Seed pixel -> Pixel == Foreground pixel and pixel is not part of any segment created before
            if (src.at<uchar>(cv::Point(x, y)) == 255 && dst.at<uchar>(cv::Point(x, y)) == 0) {
                // Step 3: After seed pixel is found pass it through growth step to find regionos
                // Step 3.1: Creating stack
                cv::Point seed_point = cv::Point(x, y);
                stack<cv::Point> point_stack;
                point_stack.push(seed_point);
                // Step 3.2: loop till the stack is empty 
                region_area = 1;
                while (!point_stack.empty())
                {
                    cv::Point center = point_stack.top();
                    dst.at<uchar>(center) = region_id;
                    point_stack.pop();
                    // Find the nighbors of the center point
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
                    region_area++;
                }
                if (region_area >= max_area)
                {
                    max_area = region_area;
                    max_region_id = region_id;
                }
                region_id +=1;
            }
        }
    }
    return max_region_id;
}

////////////////////////////////////// Task 3.2 connected component analysis ///////////////////////////////////////

// void displayRegions(cv::Mat& image, cv::Mat& labels, int num_regions_to_display) {
//     // Generate random colors for coloring regions
//     std::vector<cv::Vec3b> colors;
//     for (int i = 0; i < num_regions_to_display; ++i) {
//         colors.push_back(cv::Vec3b(rand() % 256, rand() % 256, rand() % 256));
//     }

//     // Loop over each region, ignoring the background label (0)
//     for (int label = 1; label < num_regions_to_display + 1; ++label) {
//         // Extract region mask
//         cv::Mat region_mask = (labels == label);

//         // Find contours of the region
//         std::vector<std::vector<cv::Point>> contours;
//         cv::findContours(region_mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

//         // Draw contours of the region on the original image
//         cv::drawContours(image, contours, -1, cv::Scalar(colors[label - 1]), 2);
//     }

//     // Display the image with regions highlighted
//     cv::imshow("Regions", image);
//     cv::waitKey(0);
// }

// /// @brief 
// /// @param region_map 
// /// @param labels 
// /// @param stats 
// /// @param centroids 
// /// @return region label with max area
// int connected_component_analysis(cv::Mat region_map, cv::Mat &labels, cv::Mat &stats, cv::Mat &centroids){
//     int num_labels = cv::connectedComponentsWithStats(region_map, labels, stats, centroids);   
//     // Find the region with the largest area
//     int max_area = -1;
//     int max_area_label = -1;
//     for (int label = 1; label < num_labels; ++label) {
//         int area = stats.at<int>(label, cv::CC_STAT_AREA);
//         if (area > max_area) {
//             max_area = area;
//             max_area_label = label;
//         }
//     }
//     // if (max_area_label != -1) {
//     //     cv::Rect bounding_box(stats.at<int>(max_area_label, cv::CC_STAT_LEFT),
//     //                           stats.at<int>(max_area_label, cv::CC_STAT_TOP),
//     //                           stats.at<int>(max_area_label, cv::CC_STAT_WIDTH),
//     //                           stats.at<int>(max_area_label, cv::CC_STAT_HEIGHT));
//     //     cv::Point centroid(centroids.at<double>(max_area_label, 0),
//     //                        centroids.at<double>(max_area_label, 1));
//     //     std::cout << "Bounding box: " << bounding_box << std::endl;
//     //     std::cout << "Centroid: " << centroid << std::endl;
//     // }
//     return 0;
// }




//////////////// Task 4 /////////////////////////////////

/// @brief This function extracts the % filled and Width/Height of minimum bounding box
/// @param region_map is the region map
/// @param region_id The region id of segment the features need to be extracted
/// @param feature_vector (percent filled, Width/Height)
/// @return 0 on success
int feature_extraction(cv::Mat& region_map, int region_id, Mat &bin_image_max_reg, std::vector<float> &feature_vector, std::vector<int> &centroid, std::vector<double> &min_box){
    // Step 1: Creating binary image from Region Map and Region ID
    bin_image_max_reg = (region_map == region_id);
    // cv::imshow("Bin Image", bin_image_max_reg);
    // cv::waitKey(0);
    // Step 2: Extracting Features form the binary image
    cv::Moments m = cv::moments(bin_image_max_reg, true);

    /*
    The calculations applied below are taken form rsearch paper "Image Moments-Based Structuring and Tracking of Objects - LOURENA ROCHA, LUIZ VELHO, PAULO CEZAR P. CARVALHO"
    */
    double Cx = static_cast<double>(m.m10 / m.m00);
    double Cy = static_cast<double>(m.m01 / m.m00);
    double a = static_cast<double>((m.m20 / m.m00) - (Cx * Cx));
    double b = static_cast<double>(2 * ((m.m11 / m.m00) - (Cx * Cy)));
    double c = static_cast<double>((m.m02 / m.m00) - (Cy * Cy));
    double w = static_cast<double>(sqrt(6 * (a + c - sqrt(b*b + (a - c)*(a - c)))));
    double h = static_cast<double>(sqrt(6 * (a + c + sqrt(b*b + (a - c)*(a - c)))));
    double theta = 0.5 * std::atan2(b,a - c) - 3.14/2;
    double x = Cx - 0.5;
    double y = Cy - (w * 0.5);

    // Output the extracted parameters
    // std::cout << "Height (H): " << h << std::endl;
    // std::cout << "Width (W): " << w << std::endl;
    // std::cout << "Area of the segment: " << m.m00 << std::endl;
    // std::cout << "Area of the min box: " << w*h << std::endl;

    // 2.1) Feature 1:  % filled
    float percent_filled = static_cast<float>(m.m00/ (w*h));
    //std::cout << "percent filled : " << percent_filled << std::endl;

    // 2.2) Feature 2: Bounding box Hight/Width ratio
    float wh_ratio = static_cast<float>(w/h);
    //std::cout << "W / H ratio: " << wh_ratio << std::endl;

    // 3) appending the features into feature vector
    feature_vector.push_back(percent_filled);
    feature_vector.push_back(wh_ratio); 
    int cx = static_cast<int>(Cx);
    int cy = static_cast<int>(Cy);
    centroid.push_back(cx);
    centroid.push_back(cy);
    std::vector<double> elements_to_add = {w, h, theta};
    min_box.insert(min_box.end(), elements_to_add.begin(), elements_to_add.end());
  
    // rectangle(bin_image_max_reg, Point(static_cast<int>(Cx), static_cast<int>(Cy)), Point(static_cast<int>(Cx) + 10, static_cast<int>(Cy) + 10), Scalar(100,100,100), -1);
    // imshow("Image", bin_image_max_reg);
    // waitKey(0);
    // destroyAllWindows();


    //////////
  
    return 0;
}


//////////////////////////////////////////////////// Task 6 ///////////////////////////////////////////////////////////

void image_labeling(std::string csv_path, std::vector<float> feature_vector){
    // Step 1: Ask used for 
    std::string category; 
    std::cout << "Enter the lable (only lower case letters) : ";
    std::cin >> category;
    char* category_ptr = const_cast<char*>(category.c_str());
    char* csv_path_ptr = const_cast<char*>(csv_path.c_str());
    // Step 2: Extract features for the image
    append_image_data_csv(csv_path_ptr, category_ptr, feature_vector, 0);
}
