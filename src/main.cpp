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

for(;;) {
        *capdev >> frame; // get a new frame from the camera, treat as a stream
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

        int num_regions;
        num_regions = region_growth(cleanup, region_map);
        std::cout<< "num_regions = " << num_regions << std::endl;
        cv::Mat colorMap;
        cv::applyColorMap(region_map, colorMap, cv::COLORMAP_JET); 


        //////////////////////// TASK 4 ////////////////////////
        std::vector<float> feature_vector;
        int region_id = 1;
        feature_extraction(region_map, region_id,feature_vector);


        //////////////////////// TASK 5 ////////////////////////
        bool data_collection= true;
        std::string csv_path = "C:/Users/aaron/Documents/Coding/Real-time-2D-object-detection/data/features_dir/features_csv.csv";
        // For data collection and else if for 
        if (data_collection){
                image_labeling(csv_path, feature_vector);

        }


        //////////////////////// TASK 6 //////////////////////// 
        if (!data_collection){
                std::vector<char *> labels;
                std::vector<std::vector<float>> features;
                char* csv_path_ptr = const_cast<char*>(csv_path.c_str());
                read_image_data_csv(csv_path_ptr, labels, features, 0);
        }


        //////////////////////// TASK 7 ////////////////////////




        //////////////////////// TASK 8 ////////////////////////        




        /////////////////////////////////////////////////////////
        // imshow("Original Video", frame);
        imshow("Video", binary_frame);
        imshow("Task2", cleanup);
        imshow("Task3", colorMap);


        // see if there is a waiting keystroke
        char key = cv::waitKey(10);
        if( key == 'q') {
                break;
        }
}

delete capdev;
return(0);
}
