#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <opencv2/opencv.hpp>


int region_growth(cv::Mat& src, cv::Mat& dst);