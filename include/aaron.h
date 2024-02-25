#pragma once
#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "kmeans.h"

using namespace cv;
using namespace std;

cv::Vec3b closestColor( cv::Vec3b &pix, std::vector<cv::Vec3b> &colors );

int isodata(Mat src, Mat& dst, vector<Vec3b>& means);

int find_darkest_pix(vector<Vec3b> means, int& mean_index);

int make_binary_img(Mat src, Mat&dst);

int four_conn_shrink(Mat src, Mat& dst);

int four_conn_grow(Mat src, Mat& dst);

int task2(Mat src, Mat& dst);

vector<pair<string, float>> baseline_feature_matching(string csv_path, vector<float> feature_vector);

bool cmp_least(pair<string, float>& a, pair<string, float>& b);

int sort_vec_ascending(vector<pair<string, float>>& dist_vectors_data, int size);

int task6(Mat& frame, string csv_path, vector<float> feature_vector);