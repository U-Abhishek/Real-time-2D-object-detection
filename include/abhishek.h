#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <opencv2/opencv.hpp>


int region_growth(cv::Mat& src, cv::Mat& dst);

int feature_extraction(cv::Mat& region_map, int region_id, std::vector<float> &feature_vector);

void image_labeling(std::string csv_path, std::vector<float> feature_vector);