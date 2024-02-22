#include <iostream>
#include "../include/aaron.h"
#include "../include/abhishek.h"

/// @brief MAIN LOOP
/// @param argc 
/// @param argv 
/// @return 
int main(int argc, char *argv[]) {
        cv::Mat filtered_binary_image = imread("C:/Users/uabhi/my_cpp/prcv/Real-time-2D-object-detection/data/binary_image4.png", cv::IMREAD_GRAYSCALE);
        cv::Mat segmentation_mask = cv::Mat::zeros(filtered_binary_image.rows, filtered_binary_image.cols, CV_8UC1);

        int num_regions;
        num_regions = region_growth(filtered_binary_image, segmentation_mask);
        std::cout<< "num_regions = " << num_regions << std::endl;
        cv::Mat colorMap;
        cv::applyColorMap(segmentation_mask, colorMap, cv::COLORMAP_JET); 

        // // Get the size of the segmentation map
        // cv::Size size = filtered_binary_image.size(); // Get size as a cv::Size object

        // // Extract rows and columns from the size
        // int rows = size.height; // Number of rows
        // int cols = size.width;  // Number of columns

        // // Get the number of channels
        // int channels = filtered_binary_image.channels(); // Number of channels

        // // Print the size and number of channels
        // std::cout << "Rows: " << rows << std::endl;
        // std::cout << "Columns: " << cols << std::endl;
        // std::cout << "Channels: " << channels << std::endl;

        // Display the color-mapped segmentation map
        cv::imshow("Video", colorMap);
        cv::waitKey(0);

        return(0);
}
