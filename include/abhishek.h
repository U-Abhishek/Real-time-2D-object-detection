#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <opencv2/opencv.hpp>


int region_growth(cv::Mat& src, cv::Mat& dst);

int feature_extraction(cv::Mat& region_map, int region_id, cv::Mat &bin_image_max_reg, std::vector<float> &feature_vector, std::vector<int> &centroid);

void image_labeling(std::string csv_path, std::vector<float> feature_vector);

int feature_extraction(cv::Mat& region_map, int region_id, cv::Mat &bin_image_max_reg, std::vector<float> &feature_vector, std::vector<int> &centroid, std::vector<double> &min_box);
//int connected_component_analysis(cv::Mat region_map, cv::Mat &labels, cv::Mat &stats, cv::Mat &centroids);