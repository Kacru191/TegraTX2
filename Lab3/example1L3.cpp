//-----------------------------------------------------------------------------------------------------------------
// File:   example1.cpp
// Author: Allan A. Douglas
//-----------------------------------------------------------------------------------------------------------------

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string> 
#include <sstream>
#include <stdio.h>


// Select Video Source
// The MP4 demo uses a ROI for better tracking of the moving object
//#define TEST_LIVE_VIDEO


using namespace cv;
using namespace std;

int theObject[2] = {0,0};
//bounding rectangle of the object, we will use the center of this as its position.
cv::Rect objectBoundingRectangle = cv::Rect(0,0,0,0);

//-----------------------------------------------------------------------------------------------------------------
// int to string helper function
//-----------------------------------------------------------------------------------------------------------------
string intToString(int number){
 
    //this function has a number input and string output
    std::stringstream ss;
    ss << number;
    return ss.str();
}
 
//-----------------------------------------------------------------------------------------------------------------
// Search for Moving Object
//-----------------------------------------------------------------------------------------------------------------
void searchForMovement(cv::Mat thresholdImage, cv::Mat &cameraFeed){
    //notice how we use the '&' operator for objectDetected and cameraFeed. This is because we wish
    //to take the values passed into the function and manipulate them, rather than just working with a copy.
    //eg. we draw to the cameraFeed to be displayed in the main() function.

    bool objectDetected = false;
    int xpos, ypos;

    //these two vectors needed for output of findContours
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;

    cv::Mat temp;

    thresholdImage.copyTo(temp);

#ifdef TEST_LIVE_VIDEO

    //find contours of filtered image using openCV findContours function
    cv::findContours(temp,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE );// retrieves external contours

#else

    cv::Rect roi(565, 140, 332,430);

    cv::Mat roi_temp = temp(roi); 

    //find contours of filtered image using openCV findContours function
    cv::findContours(roi_temp,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE );// retrieves external contours

#endif

    //if contours vector is not empty, we have found some objects
    if(contours.size()>0)
	objectDetected = true;
    else 
	objectDetected = false;
 
    if(objectDetected){

        //the largest contour is found at the end of the contours vector
        //we will simply assume that the biggest contour is the object we are looking for.
        vector< vector<Point> > largestContourVec;
        largestContourVec.push_back(contours.at(contours.size()-1));

        //make a bounding rectangle around the largest contour then find its centroid
        //this will be the object's final estimated position.
        objectBoundingRectangle = boundingRect(largestContourVec.at(0));

        xpos = objectBoundingRectangle.x+objectBoundingRectangle.width/2;
        ypos = objectBoundingRectangle.y+objectBoundingRectangle.height/2;
 
        //update the objects positions by changing the 'theObject' array values
        theObject[0] = xpos , theObject[1] = ypos;
    }

    //make some temp x and y variables so we dont have to type out so much

#ifdef TEST_LIVE_VIDEO

    int x = theObject[0];
    int y = theObject[1];

#else

    int x = theObject[0]+565;
    int y = theObject[1]+140;

#endif
     
    //draw some crosshairs around the object
    circle(cameraFeed,Point(x,y),10,cv::Scalar(0,255,0),2);
    line(cameraFeed,Point(x,y),Point(x,y-25),Scalar(0,255,0),2);
    line(cameraFeed,Point(x,y),Point(x,y+25),Scalar(0,255,0),2);
    line(cameraFeed,Point(x,y),Point(x-25,y),Scalar(0,255,0),2);
    line(cameraFeed,Point(x,y),Point(x+25,y),Scalar(0,255,0),2);

   rectangle(cameraFeed,Point(565,140),Point(897,570),Scalar(0,255,0),2);	
  
 
    //write the position of the object to the screen
    putText(cameraFeed,"(" + intToString(x)+","+intToString(y)+")",Point(x,y),1,1,Scalar(255,0,0),2);
 
    // Print the object position
    cout << xpos << " " << ypos << endl;
}




//-----------------------------------------------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------------------------------------------
int main() {


    // OpenCV frame matrices
    cv::Mat frame0, frame1, grayImage0, grayImage1, differenceImage, thresholdImage;
    int toggle;

#ifdef TEST_LIVE_VIDEO

    // Camera video pipeline
    std::string pipeline = "nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720, format=(string)I420, framerate=(fraction)30/1 ! nvvidconv flip-method=0 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";

#else

    // MP4 file pipeline
    std::string pipeline = "filesrc location=/home/nvidia/Downloads/pong_video.mp4 ! qtdemux name=demux ! h264parse ! omxh264dec ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";

#endif

    std::cout << "Using pipeline: " << pipeline << std::endl;
 
    // Create OpenCV capture object, ensure it works.
    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);

    if (!cap.isOpened()) {
        std::cout << "Connection failed" << std::endl;
        return -1;
    }
     
    // Capture the first frame with GStreamer
    cap >> frame0;

    // Convert the frames to gray scale (monochrome)
    cv::cvtColor(frame0,grayImage0,cv::COLOR_BGR2GRAY);

    // Initialize 
    toggle = 0;

    while (1) {

        if (toggle == 0) {
           // Get a new frame from file
           cap >> frame1; 
           // Convert the frames to gray scale (monochrome)    
	   cv::cvtColor(frame1,grayImage1,cv::COLOR_BGR2GRAY);
           toggle = 1;
        } 
        else {
	   cap >> frame0;
     	   cv::cvtColor(frame0,grayImage0,cv::COLOR_BGR2GRAY);
           toggle = 0;
	}
 
	// Compute the absolte value of the difference
	cv::absdiff(grayImage0, grayImage1, differenceImage);

	// Threshold the difference image
	cv::threshold(differenceImage, thresholdImage, 50, 255, cv::THRESH_BINARY);

	// Find the location of any moving object and show the final frame
	if (toggle == 0) {
		searchForMovement(thresholdImage,frame0);
	        imshow("Frame", frame0);
	}
	else {
		searchForMovement(thresholdImage,frame1);
	        imshow("Frame", frame1);
	}

        cv::waitKey(1); //needed to show frame
    }
}
