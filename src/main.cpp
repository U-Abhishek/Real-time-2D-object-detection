/*
Abhishek Uddaraju | Aaron Pan
18/02/2024
This file is the main file for project 3 that runs the loop for video display
*/
#include <iostream>
#include "../include/preprocess_classification.h"
#include "../include/process_featureExtract.h"
#include "../include/csv_util.h"
#include <cmath>

using namespace std;
using namespace cv;



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

        //cv::namedWindow("Video", 1); // identifies a window
        cv::Mat frame;

        char key_flag = 'o';
        char key_flag_distance = 'l';

        // Feature vectors for task 6 : labels and features
        std::string csv_path;
        if (argc < 2) {
                std::cerr << "Provid the loction of database CSV File" << std::endl;
                return 1;
        }
        else{
                // accessing command line arguments
                csv_path = argv[1];  
        }
        // std::vector<char *> labels;
        // std::vector<std::vector<float>> features;
        // char* csv_path_ptr = const_cast<char*>(csv_path.c_str());
        // read_image_data_csv(csv_path_ptr, labels, features, 0);

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
                threshold(gray_frame, binary_frame, 100, 255, THRESH_BINARY_INV);

                ////////////////////// TASK 2 ////////////////////////
                Mat cleanup;
                task2(binary_frame, cleanup);

        
                //////////////////////// TASK 3 ////////////////////////
                cv::Mat region_map = cv::Mat::zeros(cleanup.rows, cleanup.cols, CV_8UC1);

                int max_region_id;
                max_region_id = region_growth(cleanup, region_map);
                if(max_region_id==-1){
                        putText(frame, "Place an Object No objects detected ", Point(10,70), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0,0,255), 2);
                }
                //////////////////////// TASK 4 ////////////////////////

                std::vector<float> feature_vector;
                std::vector<int> centeroidXY;
                cv::Mat bin_image_max_reg;
                int region_id = max_region_id;
                std::vector<double> min_box;
                feature_extraction(region_map, region_id, bin_image_max_reg, feature_vector, centeroidXY, min_box);
                // Plotting centroid on image
                cv::circle(frame, cv::Point(centeroidXY[0], centeroidXY[1]), 5, cv::Scalar(100, 50, 100), 3);
                // plottiing rectangle on image
                float width = static_cast<float>(min_box[0]);
                float height = static_cast<float>(min_box[1]);
                float theta = static_cast<float>(min_box[2]);
                cv::Point center = cv::Point(centeroidXY[0], centeroidXY[1]);
                float theta_deg = static_cast<float>(theta * (180.0 / 3.141));
                cv::RotatedRect rotatedRect(center, cv::Size2f(width, height), theta_deg);
                cv::Point2f vertices[4];
                rotatedRect.points(vertices);
                for (int i = 0; i < 4; i++) {
                        cv::line(frame, vertices[i], vertices[(i + 1) % 4], cv::Scalar(0, 20, 230), 2); 
                }
                cv::Point2f arrowEnd(center.x + 0.8 * height * std::cos(theta- 3.14/2),
                                center.y + 0.8 * height * std::sin(theta- 3.14/2));
                cv::arrowedLine(frame, center, arrowEnd, cv::Scalar(100, 10, 40), 1, cv::LINE_AA, 0, 0.3);

                cv::Mat task4_img = frame.clone();
                //////////////////////// TASK 5 ////////////////////////
                char key = cv::waitKey(1);
                if(key == 'q') {
                        break;
                }else if(key == 'n'){
                        image_labeling(csv_path, feature_vector);
                }

                //////////////////////// TASK 6 ////////////////////////
                if(key == 'l' || key_flag_distance == 'l'){
                        key_flag_distance = 'l';
                        task6(frame, csv_path, feature_vector);
                }
                
                //////////////////////// TASK 9 ////////////////////////
                if(key == 'k' || key_flag_distance == 'k'){
                        key_flag_distance = 'k';
                        knn_classification(frame, csv_path, feature_vector, 3);
                }

                /////////////////////////////////////////////////////////
                imshow("Task 1: Threshold Image ", binary_frame);
                imshow("Task 2: Cleaned image", cleanup);

                // Task 3 required images: Converting the region map to rgb colour map for display 
                cv::Mat normalizedMap;
                cv::normalize(region_map, normalizedMap, 0, 255, cv::NORM_MINMAX, CV_8UC1);
                cv::Mat color_map;
                cv::applyColorMap(normalizedMap, color_map, cv::COLORMAP_COOL);
                imshow("Task 3: Region Map with different colours", color_map);

                // Task 4 required images: adding features computed on the task4_img frame
                std::string per_fill_str = "Percent Fill : " + std::to_string(feature_vector[0]);
                std::string w_h_ratio_str = "W/H ration : " + std::to_string(feature_vector[1]);
                putText(task4_img, per_fill_str, Point(10,30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255,255,255), 2);
                putText(task4_img, w_h_ratio_str, Point(10, 60), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255,255,255), 2);
                imshow("Task 4: Least central moment & Oriented Box & Features", task4_img);

                imshow("Final Video", frame);
                //imshow("Video", bin_image_max_reg);
                
        }

        delete capdev;
        return(0);
}
