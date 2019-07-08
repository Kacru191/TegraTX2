/*
* File:   test.cpp
* Author: Allan Douglas
* School: Oregon Tech
*/

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string> 
#include <sstream>
#include <stdio.h>

// Function to build a GStreamer pipeline
std::string get_tegra_pipeline(int width, int height, int fps) {
 
    std::string w_string, h_string, fps_string;
    std::stringstream temp1, temp2, temp3;

    temp1 << width;
    w_string = temp1.str();
    temp2 << height;
    h_string = temp2.str();
    temp3 << fps;
    fps_string = temp3.str();

    return "nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)" + w_string + ", height=(int)" +
           h_string + ", format=(string)I420, framerate=(fraction)" + fps_string +
           "/1 ! nvvidconv flip-method=0 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}


int main() {

    // Options
    int WIDTH = 1280;
    int HEIGHT = 720;
    int FPS = 30;

    // Video frame matrices
    cv::Mat frame;

     // Check OpenCV Version
    std::cout << CV_VERSION << std::endl;

    // Define the gstream pipeline
    std::string pipeline = get_tegra_pipeline(WIDTH, HEIGHT, FPS);
    std::cout << "Using pipeline: \n\t" << pipeline << "\n";
 
    // Create OpenCV capture object, ensure it works.
    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);

    // Check the video connection
    if (!cap.isOpened()) {
        std::cout << "Connection failed";
        return -1;
    }
 
    // View video
    while (1) {

	// Get a new frame from the camera
        cap >> frame;

	// Display video in a window
        imshow("Original Frame", frame);

	// Wait 1ms for key press
        cv::waitKey(1);
    }
}
