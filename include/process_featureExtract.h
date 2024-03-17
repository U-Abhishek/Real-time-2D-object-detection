/*
Abhishek Uddaraju | Aaron Pan
18/02/2024
This is the .h for abhishek.cpp file it contains all the usable function from abhishek.cpp
*/
#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <opencv2/opencv.hpp>

/// @brief Applies region growth algorithm to find segments in the image
/// @param src binary filtered image
/// @param dst image to store the sementation map (region IDs are stored as pixel value starting form 1 to n regions)
/// @return regioin id of maximum area
int region_growth(cv::Mat& src, cv::Mat& dst);

/// @brief This function extracts the % filled and Width/Height of minimum bounding box
/// @param region_map is the region map
/// @param region_id The region id of segment the features need to be extracted
/// @param feature_vector (percent filled, Width/Height)
/// @return 0 on success
int feature_extraction(cv::Mat& region_map, int region_id, cv::Mat &bin_image_max_reg, std::vector<float> &feature_vector, std::vector<int> &centroid, std::vector<double> &min_box);

/// @brief Takes input form user add uses that as label and appends the new data point created to the database
/// @param csv_path Database CSV file path
/// @param feature_vector feature vector of the current video frame
void image_labeling(std::string csv_path, std::vector<float> feature_vector);