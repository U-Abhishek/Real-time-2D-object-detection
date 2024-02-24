/*
Abhishek Uddaraju | Aaron Pan
19/02/2024
This file is the main file for project 3 that runs the loop for video display
*/
#include <iostream>
#include "../include/aaron.h"
#include "../include/abhishek.h"
#include "../include/csv_util.h"

/// @brief MAIN LOOP
/// @param argc 
/// @param argv 
/// @return 
int main(int argc, char *argv[]) {
cv::VideoCapture *capdev;

// open the video device
capdev = new cv::VideoCapture(1);
if( !capdev->isOpened() ) {
        printf("Unable to open video device\n");
        return(-1);
}

// get some properties of the image
cv::Size refS( (int) capdev->get(cv::CAP_PROP_FRAME_WIDTH ),
                (int) capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
printf("Expected size: %d %d\n", refS.width, refS.height);

cv::namedWindow("Video", 1); // identifies a window
cv::Mat frame;

char key_flag = 'o';

// Feature vectors for task 6 : labels and features
std::string csv_path = "C:/Users/aaron/Documents/Coding/Real-time-2D-object-detection/data/features_dir/features_csv.csv";
std::vector<char *> labels;
std::vector<std::vector<float>> features;
char* csv_path_ptr = const_cast<char*>(csv_path.c_str());
read_image_data_csv(csv_path_ptr, labels, features, 0);

for(;;) {
        *capdev >> frame; 
        if( frame.empty() ) {
                printf("frame is empty/n");
                break;
        }
        ////////// place object recognition code below //////////
        
        //////////////////////// TASK 1 ////////////////////////

        //apply gaussian blur to the image
        Mat blured_frame;
        GaussianBlur(frame, blured_frame, Size(3,3), 1, 1);
        Mat gray_frame;
        cvtColor(blured_frame, gray_frame, COLOR_BGR2GRAY);

        Mat binary_frame;
        threshold(gray_frame, binary_frame, 120, 255, THRESH_BINARY);

        ////////////////////// TASK 2 ////////////////////////
        Mat cleanup;
        task2(binary_frame, cleanup);

        ////////////////////// TASK 3 ////////////////////////
        cv::Mat region_map = cv::Mat::zeros(cleanup.rows, cleanup.cols, CV_8UC1);

        int num_regions;
        num_regions = region_growth(cleanup, region_map);
        std::cout<< "num_regions = " << num_regions << std::endl;
        cv::Mat colorMap;
        cv::applyColorMap(region_map, colorMap, cv::COLORMAP_JET); 
        //////////////////////// TASK 4 ////////////////////////

        std::vector<float> feature_vector;
        std::vector<float> centeroidXY;
        int region_id = 1;
        feature_extraction(region_map, region_id,feature_vector, centeroidXY);
        // Plotting centroid on image
        cv::circle(cleanup, cv::Point(centeroidXY[0], centeroidXY[1]), 5, cv::Scalar(0, 0, 255), 3);

        //////////////////////// TASK 5 ////////////////////////
        char key = cv::waitKey(1);
        if(key == 'q') {
                break;
        }
        else if(key == 'n'){
                image_labeling(csv_path, feature_vector);
        }

        //////////////////////// TASK 6 //////////////////////// 




        //////////////////////// TASK 7 ////////////////////////




        //////////////////////// TASK 8 ////////////////////////        




        /////////////////////////////////////////////////////////
        imshow("Original Video", frame);
        imshow("Video", binary_frame);
        imshow("Task2", cleanup);
        // imshow("Task2", cleanup);


        
}

delete capdev;
return(0);
}
