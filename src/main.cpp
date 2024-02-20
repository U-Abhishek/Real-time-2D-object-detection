#include <iostream>
#include "aaron.h"

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


        //apply kmeans
        vector<Vec3b> means;
        std::vector<cv::Vec3b> data;
        int B = 4; // setting it to sample 1/16 of the pixels?
        for(int i=0;i<blured_frame.rows - B;i += B) {
                for(int j=0;j<blured_frame.cols - B;j += B) {
                int jx = rand() % B;
                int jy = rand() % B;
                data.push_back( blured_frame.at<cv::Vec3b>(i+jy, j+jx) );
                }
        }

        int *labels = new int[data.size()];
        int ncolors = 2; // I think this means k = 2? Apparently this means ISODATA algo

        if(kmeans( data, means, labels, ncolors ) ) {
                printf("Erro using kmeans\n");
                return(-1);
        }


        //print means just to see values
        for (Vec3b i:means){
                for (int j = 0; j < 3; j++){

                        cout << static_cast<float>(i.val[j]) << endl;
                }
        }

        //closest color
        Mat dst;
        dst.create( blured_frame.size(), blured_frame.type() );
        for(int i=0;i<blured_frame.rows;i++) {
                for(int j=0;j<blured_frame.cols;j++) {
                dst.at<cv::Vec3b>(i,j) = closestColor(blured_frame.at<cv::Vec3b>(i,j), means);
                }
        }

        //output video
        cv::imshow("clustered", dst );

        //////////////////////// TASK 2 ////////////////////////



        /////////////////////////////////////////////////////////
        // cv::imshow("Video", frame);

        // see if there is a waiting keystroke
        char key = cv::waitKey(10);
        if( key == 'q') {
                break;
        }
}

delete capdev;
return(0);
}
