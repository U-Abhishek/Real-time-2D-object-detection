# Real-time-2D-object-detection

#### Team member's Names: _Aaron Pan, Abhishek Uddaraju_
#### Operating System / IDE: Windows / VSCode.
#### Time Travel days: 1.

## Table of Contents

1. [Intoduction](#introduction)
2. [Installation](#installation)
3. [Contributing](#contributing)
4. [License](#license)

## Introduction
This project introduces methods to compare image frames within a video sequence for object recognition. We use region-based object recognition methods such as moments, percent fill, etc to compare objects. The program takes a training database of at least 5 objects of multiple points to then compare to new objects (known or unknown), if the objects are known the system will classify the object. With comparison, multiple distance metrics can be used to compare objects, such as L2 distance and KNN classification, which we will use in this project.

## Installation
### Cloning git repository 
```
$ git clone https://github.com/username/project.git
$ cd project
```

### OpenCV Installation (Pre-configured OpenCV for Python and C++ from GitHub releases)
1. Download opencv-4.5.1-vc14_vc15.exe from the latest release: 
   https://github.com/opencv/opencv/releases/tag/4.5.1
1. unpack to `C:\opencv`
1. edit environment variables. Win search "Environment variables"
1. append to Path the path to OpenCV: `C:\opencv\bin`
1. restart computer
1. open cmd
1. `echo %Path%` output should contain a new path

## Usage

### Step 1:  Creating the .csv files
- Our recommendation is to download all the .csv files provided in the submission and add your paths to the main function.
- you can create one employ .csv file and re-use it by deleting all the content inside it while testing.

### Step 2: Command line arguments required
```
.\executable_name "target_image_path" "database_directory" "task number"
```
-- The top ten common images will be printed in the terminal along with the distance to the target image.
