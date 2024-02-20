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