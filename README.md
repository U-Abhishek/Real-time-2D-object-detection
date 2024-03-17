# Real-time-2D-object-detection

#### Team Members

- Aaron Pan
- Abhishek Uddaraju
#### Operating System Used: _Windows_
#### IDE: _VScode_
#### Time Travel days: 1

## Tasks and Function Location

| Task Number | Function Location|
|----------|----------|
| Task 1| main.cpp | 
| Task 2| preprocess_classification.cpp | 
| Task 3| process_featureExtract.cpp | 
| Task 4| process_featureExtract.cpp | 
| Task 5| main.cpp | 
| Task 6| preprocess_classification.cpp | 
| Task 9 KNN| preprocess_classification.cpp | 

## Table of Contents

1. [File Structure](#file_structure)
2. [Intoduction](#introduction)
3. [Installation](#installation)
4. [Usage](#usage)

## File_Structure
* bin/
  * Debug/
* include/
  * preprocess_classification.h
  * process_featureExtract.h
  * csv_util.h
  * kmeans.h
* src/
  * preprocess_classification.cpp
  * process_featureExtract.cpp
  * dnnexample.cpp
  * kmeans.cpp
  * main.cpp
  * object_detection.cpp
* utils/
  * csv_util.cpp
* .gitignore
* CMakeLists.txt
* README.md


## Introduction
This project introduces methods to compare image frames within a video sequence for object recognition. We use region-based object recognition methods such as moments, percent fill, etc to compare objects. The program takes a training database of at least 5 objects of multiple points to then compare to new objects (known or unknown), if the objects are known the system will classify the object. With comparison, multiple distance metrics can be used to compare objects, such as L2 distance and KNN classification, which we will use in this project.

## Installation  
### *** (Currently, the Repo is private Please use the .exe provided in the submission to run the project) ***
### 1. Cloning the project
```
$ git clone https://github.com/U-Abhishek/Real-time-2D-object-detection.git
$ cd /project location
```

### 2. Install C++ Compiler

You need a C++ compiler installed on your system to build and run this project. If you don't have one installed, follow the instructions below:

- **Windows**: Install MinGW or Visual Studio with C++ development tools.
- **Mac**: Install Xcode command-line tools or GCC through Homebrew.
- **Linux**: Install GCC through your package manager (e.g., `sudo apt install g++` on Ubuntu).

### 3. Setup Visual Studio Code

Visual Studio Code is a lightweight but powerful source code editor that runs on your desktop. To set up Visual Studio Code for C++ development:

1. [Download](https://code.visualstudio.com/) and install Visual Studio Code.
2. Install the C/C++ extension by Microsoft from the Extensions view (`Ctrl+Shift+X`).
3. Open your project folder in Visual Studio Code.


### 4. OpenCV Installation (Pre-configured OpenCV for Python and C++ from GitHub releases)
1. Download opencv-4.5.1-vc14_vc15.exe from the latest release: 
   https://github.com/opencv/opencv/releases/tag/4.5.1
1. unpack to `C:\opencv`
1. edit environment variables. Win search "Environment variables"
1. append to Path the path to OpenCV: `C:\opencv\bin`
1. restart computer
1. open cmd
1. `echo %Path%` output should contain a new path

## Usage

### 1) Command line arguments required to run the Executable
```
$ cd directory_of_executable
$ .\executable_name.exe "database_csv_path"
```
This will start the program you will be able to see all the results and intermediate processed images. 
### 2) To add a new object to the database:
1) place the object in front of the camera
2) Press "n" after clicking on the window where the frames are displayed
3) Type the name of the object in the terminal

### 3) For switching to KNN classification
- Press "k" after clicking on the window where the frames are displayed

### 3) For switching to L2 distance based NN classification
- Press "l" after clicking on the window where the frames are displayed

