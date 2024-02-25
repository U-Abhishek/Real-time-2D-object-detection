# Real-time-2D-object-detection

### Team member's Names: Aaron Pan, Abhishek Uddaraju

#### OS/IDE: Windows / VSCode

#### Time Travel days: 1


## Introduction:
This project introduces methods to compare image frames within a video sequence for object recognition. We use region-based object recognition methods such as moments, percent fill, etc to compare objects. The program takes a training database of at least 5 objects of multiple points to then compare to new objects (known or unknown), if the objects are known and the system will classify the object. With comparison, multiple distance metrics can be used to compare objects, such as L2 distance and KNN classification, which we will use in this project.

## Installation:
### OpenCV Installation (Pre-configured OpenCV for Python and C++ from GitHub releases)
1. Download opencv-4.5.1-vc14_vc15.exe from latest release: 
   https://github.com/opencv/opencv/releases/tag/4.5.1
1. unpack to `C:\opencv`
1. edit environment variables. Win search "Environment variables"
1. append to Path the path to opencv: `C:\opencv\bin`
1. restart computer
1. open cmd
1. `echo %Path%` output should contain new path

### Step 1:  Creating the .csv files
- Our recommendation is to download all the .csv files provided in the submission and add your paths to the main function.
- you can create one employ .csv file and re-use it by deleting all the content inside it while testing.

### Step 2: Command line arguments required
```
.\executable_name "target_image_path" "database_directory" "task number"
```
-- The top ten common images will be printed in the terminal along with the distance to the target image.
