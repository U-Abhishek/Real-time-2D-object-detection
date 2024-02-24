// #include <iostream>
// #include "../include/aaron.h"
// #include "../include/abhishek.h"
// #include "../include/csv_util.h"



// /// @brief MAIN LOOP
// /// @param argc 
// /// @param argv 
// /// @return 
// int main(int argc, char *argv[]) {

//         //////////////////////////////////////////// Task 3/////////////////////////////////////////
//         cv::Mat filtered_binary_image = imread("C:/Users/uabhi/my_cpp/prcv/Real-time-2D-object-detection/data/bin_img_examples/binary_image5.png", cv::IMREAD_GRAYSCALE);
//         cv::Mat region_map = cv::Mat::zeros(filtered_binary_image.rows, filtered_binary_image.cols, CV_8UC1);

//         int max_region_id;
//         max_region_id = region_growth(filtered_binary_image, region_map);
//         std::cout<< "max_region_id = " << max_region_id << std::endl;
//         // cv::Mat colorMap;
//         // cv::applyColorMap(region_map, colorMap, cv::COLORMAP_JET); 

//         // // Get the size of the segmentation map
//         // cv::Size size = filtered_binary_image.size(); // Get size as a cv::Size object

//         // // Extract rows and columns from the size
//         // int rows = size.height; // Number of rows
//         // int cols = size.width;  // Number of columns

//         // // Get the number of channels
//         // int channels = filtered_binary_image.channels(); // Number of channels

//         // // Print the size and number of channels
//         // std::cout << "Rows: " << rows << std::endl;
//         // std::cout << "Columns: " << cols << std::endl;
//         // std::cout << "Channels: " << channels << std::endl;

//         // // Display the color-mapped segmentation map
//         //cv::imshow("colorMap", colorMap);
//         //cv::waitKey(0);

//         /// Connected components 
//         // cv::Mat labels, stats, centroids;
//         // int max_region_lable = connected_component_analysis(region_map, labels, stats, centroids);


//         ///////////////////////////////////////// Task 4  ////////////////////////////////////////////////////////
 
//         std::vector<float> feature_vector;
//         std::vector<int> centeroidXY;
//         int region_id = max_region_id;
//         feature_extraction(region_map, region_id,feature_vector, centeroidXY);


//         ////////////////////////////////////////// Task 5 ////////////////////////////////////////////////////////

//         bool data_collection= true;
//         std::string csv_path = "../../data/feaures_dir/features_csv.csv";
//         // For data collection and else if for 
//         if (data_collection){
//                 image_labeling(csv_path, feature_vector);

//         }

//         ////////////////////////////////////////// Task 6 ////////////////////////////////////////////////////////

//         if (!data_collection){
//                 std::vector<char *> labels;
//                 std::vector<std::vector<float>> features;
//                 char* csv_path_ptr = const_cast<char*>(csv_path.c_str());
//                 read_image_data_csv(csv_path_ptr, labels, features, 0);
//         }
//         return(0);


// }


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
capdev = new cv::VideoCapture(0);
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
std::string csv_path = "../../data/feaures_dir/features_csv.csv";
std::vector<char *> labels;
std::vector<std::vector<float>> features;
char* csv_path_ptr = const_cast<char*>(csv_path.c_str());
read_image_data_csv(csv_path_ptr, labels, features, 0);

for(;;) {
        *capdev >> frame; 
        if( frame.empty() ) {
                printf("frame is empty\n");
                break;
        }
        ////////// place object recognition code below //////////
        
        //////////////////////// TASK 1 ////////////////////////

        //apply gaussian blur to the image
        Mat blured_frame;
        GaussianBlur(frame, blured_frame, Size(3,3), 1, 1);

        //apply ISODATA alg
        Mat dominant_frame, binary_frame;
        vector<Vec3b> means;
        isodata(blured_frame, dominant_frame, means);
        make_binary_img(dominant_frame, binary_frame, means);

        //////////////////////// TASK 2 ////////////////////////
        Mat cleanup;
        task2(binary_frame, cleanup);

        //////////////////////// TASK 3 ////////////////////////
        cv::Mat region_map = cv::Mat::zeros(cleanup.rows, cleanup.cols, CV_8UC1);

        int max_region_id;
        max_region_id = region_growth(cleanup, region_map);
        std::cout<< "max_region_id = " << max_region_id << std::endl;

        // cv::Mat colorMap;
        // cv::applyColorMap(region_map, colorMap, cv::COLORMAP_JET); 


        // ///////////////////////// Task 3.2 /////////////////////////////

        // // Connected components 
        // cv::Mat labels, stats, centroids;
        // connected_component_analysis(region_map, labels, stats, centroids);

        //////////////////////// TASK 4 ////////////////////////

        std::vector<float> feature_vector;
        std::vector<int> centeroidXY;
        Mat bin_image_max_reg;
        int region_id = max_region_id;
        feature_extraction(region_map, region_id, bin_image_max_reg, feature_vector, centeroidXY);
        // Plotting centroid on image
        cv::circle(bin_image_max_reg, cv::Point(centeroidXY[0], centeroidXY[1]), 5, cv::Scalar(0, 0, 255), 3);

        //////////////////////// TASK 5 ////////////////////////
        char key = cv::waitKey(1);
        if(key == 'q') {
                break;
        }else if(key == 'n'){
                image_labeling(csv_path, feature_vector);
        }

        //////////////////////// TASK 6 //////////////////////// 



        //////////////////////// TASK 7 ////////////////////////




        //////////////////////// TASK 8 ////////////////////////        




        /////////////////////////////////////////////////////////
        // imshow("Original Video", frame);
        imshow("Video", bin_image_max_reg);
        // imshow("Task2", cleanup);


        
}

delete capdev;
return(0);
}

