#include <iostream>
#include "../include/aaron.h"

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




        //////////////////////// TASK 4 ////////////////////////




        //////////////////////// TASK 5 ////////////////////////




        //////////////////////// TASK 6 //////////////////////// 




        //////////////////////// TASK 7 ////////////////////////




        //////////////////////// TASK 8 ////////////////////////        




        /////////////////////////////////////////////////////////
        // imshow("Original Video", frame);
        imshow("Video", binary_frame);
        imshow("Task2", cleanup);

        // see if there is a waiting keystroke
        char key = cv::waitKey(10);
        if( key == 'q') {
                break;
        }
}

delete capdev;
return(0);
}
