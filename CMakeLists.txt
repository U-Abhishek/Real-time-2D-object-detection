cmake_minimum_required(VERSION 3.5.0)
project(real_time_2d_object_detection VERSION 0.1.0 LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

#set(HEADER_FILES include/preprocess_classification.h include/process_featureExtract.h include/kmeans.h)

add_executable(real_time_2d_object_detection src/main.cpp src/preprocess_classification.cpp src/kmeans.cpp src/process_featureExtract.cpp utils/csv_util.cpp) # ${HEADER_FILES})
#add_executable(real_time_2d_object_detection src/reading_dnn.cpp)

target_link_libraries(real_time_2d_object_detection ${OpenCV_LIBS})
target_include_directories(real_time_2d_object_detection PRIVATE include)
