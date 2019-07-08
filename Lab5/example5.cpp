//-----------------------------------------------------------------------------------------------------------------
// File:   example5.cpp
// Author: Allan A. Douglas/Krystal Cruz
//-----------------------------------------------------------------------------------------------------------------

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string> 
#include <sstream>
#include <stdio.h>
#include "LeastSquares.h"
#include <opencv2/cudacodec.hpp>

// Select Video Source
// The MP4 demo uses a ROI for better tracking of the moving object
//#define TEST_LIVE_VIDEO


using namespace cv;
using namespace std;

int theObject[2] = {0,0};
int OldObject[8] = {0,0,0,0,0,0,0,0};
//using four frames for each x and y 
int xt = 394;//x left
int yt = 314;// y top
int xt2 = 926; //x right
int yt2 = 625; //y bottom
int xw = xt2-xt; //hidth
int yh = yt2-yt; //width
//bounding rectangle of the object, we will use the center of this as its position.
cv::Rect objectBoundingRectangle = cv::Rect(0,0,0,0);

cv::Rect roi2(xt, yt, xw,yh);//Rect roi( x left, y top, width, height);
LeastSquares LSF(roi2);

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

    cv::Rect roi(xt, yt, xw,yh);
	//Rect roi( x left, y top, width, height);

    cv::Mat roi_temp = temp(roi); 
    
   // LeastSquares LSF(roi);//declare class

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
////////////////////////////////////////////////////////////////////////////////////////////
// COMPUTE REBOUNDS USING LSF METHOD 
////////////////////////////////////////////////////////////////////////////////////////////
 
        //update the objects positions by changing the 'theObject' array values
	Point2i CurrentPnt[2];
	CurrentPnt[0] = Point2i(xpos+xt,ypos+yt);
	cout << "Current Point: " << CurrentPnt[0] << endl;
	Point2i ImpactPnt[8];

	LSF.addPoint(CurrentPnt[0]);
	//cout << "size of array = " << LSF.getSize() <<endl;
	 

	if(!LSF.testMonotonic())
		cout << "NOT monotonic" << endl;
	
	else{
		cout << "Monotonic" << endl;
		LSF.computeLSF();

		cout << "Number of Frames: " << LSF.getFrameCount() << endl;
		cout << "Tbar(avg of t's): " << LSF.getTbar() << endl;
		cout << "Sum of (t-Tbar)^2: " <<LSF.getSqTbar() << endl;
		cout << "Slope Mx: " << LSF.GetMx() << endl;
		cout << "Slope My: " << LSF.GetMy() << endl;
		cout << "Average of X: " << LSF.GetSumX() << endl;
		cout << "Average of Y: " << LSF.GetSumY() << endl;	

		LSF.computeRebounds();
		ImpactPnt[0] = LSF.getRebound(1);//m_rebounds[1] is the first bounce
		cout << "Impact Point: " << ImpactPnt[0] << endl; 
		line(cameraFeed, CurrentPnt[0],ImpactPnt[0],Scalar(0,255,0),2);
		line(cameraFeed, ImpactPnt[0],LSF.getGoal(),Scalar(0,255,0),2);//from bounce to paddle

		circle(cameraFeed,LSF.getGoal(),10,cv::Scalar(0,0,255),2);//the expected goal of ball
//////////////////////////////////////////////////////////////////////////////////////////

	}
	for (int i= 7;i>=3;i=i-2)
	{
		OldObject[i-1]=OldObject[i-3];
		OldObject[i]=OldObject[i-2];
	}


	//assigning the previous position to the current position
        OldObject[0] = theObject[0];
        OldObject[1] = theObject[1];



        theObject[0] = xpos , theObject[1] = ypos;
    }

    //make some temp x and y variables so we dont have to type out so much

#ifdef TEST_LIVE_VIDEO
#else

	int x = theObject[0]+xt; //creates origin so offset is to set this to (0,0)
	int y = theObject[1]+yt; //Top left corner
	int x1 = OldObject[0]+xt;//previous position of x + offset 568
	int y1 = OldObject[1]+yt;//previous position of y + offset 140
	int x2 = OldObject[2]+xt;//previous position of x + offset 568
	int y2 = OldObject[3]+yt;//previous position of y + offset 140
	int x3 = OldObject[4]+xt;//previous position of x + offset 568
	int y3 = OldObject[5]+yt;//previous position of y + offset 140



	double xpos1 = (x-x3);//the distance from the current to the oldest-x
	double ypos1 = (y-y3);//the distance from the current to the oldest-y
	double dist = sqrt(pow(xpos1,2)+pow(ypos1,2));//magnitude of vector

	double Vavg = dist/0.033;//isnt necessary because capturing each frame
				// is in reference to time with 33fps
				//calculates velocity distance per second

	double V0 = ypos1/xpos1;//velocity
	double V1 = -1*V0;

	double yleft,yright,xleft,xright; //creating prediction off left wall 
	double yleftd,yrightd,xleftd,xrightd; //creating prediction off right wall	


	//cout << "X position: " << xpos1 << endl;
//	cout << "Y Position: " << ypos1 << endl;
//	cout << "Distance(magnitude): " << dist << endl;
//	cout << "average V" << avg << endl;
//	cout << "Average velocity: " << Vavg << endl;
//	cout << "Velocity: " << V0 << endl;


#endif

    //draw some crosshairs around the object
    //circle(cameraFeed,Point(x,y),10,cv::Scalar(0,255,0),2);
    //line(cameraFeed,Point(x,y),Point(x,y-25),Scalar(0,255,0),2);
    //line(cameraFeed,Point(x,y),Point(x,y+25),Scalar(0,255,0),2);
    //line(cameraFeed,Point(x,y),Point(x-25,y),Scalar(0,255,0),2);
    //line(cameraFeed,Point(x,y),Point(x+25,y),Scalar(0,255,0),2);


	rectangle(cameraFeed,Point(xt,yt),Point(xt2,yt2),Scalar(255,0,0),2);
	//Rectangle created for region of interest

///////////////////////////////////////////////////////////////////////////


}




//-----------------------------------------------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------------------------------------------
int main() {


    // OpenCV frame matrices
    cv::Mat frame0, frame1, frame1_warped, frame0_warped, result;

    cv::cuda::GpuMat gpu_frame0, gpu_frame1, gpu_grayImage0, gpu_grayImage1, gpu_differenceImage, gpu_thresholdImage,gpu_frame0_warped,gpu_frame1_warped;

    int toggle, frame_count;

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

    //Input Quadilateral or Image plane coordinates
    Point2f inputQuad[4];
    //Output Quadilateral or World plane coordinates
    Point2f outputQuad[4];
    //Lambda Matrix
    cv::Mat lambda( 2, 4, CV_32FC1);

    // The 4 points that select quadilateral on the input, from top-left in clockwise order
    // These four points are the sides of the rect box used as input

    //OpenCV coordinate system is based on rows and then columns.
    inputQuad[1] = Point2f(523,79); 
    inputQuad[2] = Point2f(880, 78);
    inputQuad[3] = Point2f(924, 669);   
    inputQuad[0] = Point2f(473, 655);

    // The 4 points where the mapping is to be done, from top-left in clockwise order
    outputQuad[0] = Point2f(300,233);
    outputQuad[1] = Point2f(1018,233);
    outputQuad[2] = Point2f(1018,642);
    outputQuad[3] = Point2f(300,642);

    //get the perspective transform matrix i.e. lambda
    lambda = cv::getPerspectiveTransform(inputQuad,outputQuad);


    // Capture the new frame with GStreamer
    cap >> frame0;
 
    //upload the GPU memory
    gpu_frame0.upload(frame0);

    // Warp perspective 
    cv::cuda::warpPerspective(gpu_frame0,gpu_frame0_warped,lambda,gpu_frame0.size());

    gpu_frame0_warped.download(frame0_warped);

    // Convert the frames to gray scale (monochrome)
    //cv::cvtColor(frame0,grayImage0,cv::COLOR_BGR2GRAY);
    cv::cuda::cvtColor(gpu_frame0_warped,gpu_grayImage0,cv::COLOR_BGR2GRAY);

    // Initialize 
    toggle = 0;
    frame_count = 0;

    while (frame_count < 4000) {

        if (toggle == 0) {
           // Get a new frame from file
           cap >> frame1;
	   gpu_frame1.upload(frame1);

    // Warp perspective 
    cv::cuda::warpPerspective(gpu_frame1,gpu_frame1_warped,lambda,gpu_frame1.size());

    gpu_frame1_warped.download(frame1_warped);


           // Convert the frames to gray scale (monochrome)    
	   //cv::cvtColor(frame1,grayImage1,cv::COLOR_BGR2GRAY);
           cv::cuda::cvtColor(gpu_frame1_warped,gpu_grayImage1,cv::COLOR_BGR2GRAY);
           toggle = 1;
        } 
        else {
	   cap >> frame0;
           gpu_frame0.upload(frame0);

    // Warp perspective 
    cv::cuda::warpPerspective(gpu_frame0,gpu_frame0_warped,lambda,gpu_frame0.size());

    gpu_frame0_warped.download(frame0_warped);


     	   //cv::cvtColor(frame0,grayImage0,cv::COLOR_BGR2GRAY);
           cv::cuda::cvtColor(gpu_frame0_warped,gpu_grayImage0,cv::COLOR_BGR2GRAY);
	   

           toggle = 0;
	}
 
	// Compute the absolte value of the difference
	//cv::absdiff(grayImage0, grayImage1, differenceImage);
	cv::cuda::absdiff(gpu_grayImage0, gpu_grayImage1, gpu_differenceImage);


	// Threshold the difference image
	//cv::threshold(differenceImage, thresholdImage, 50, 255, cv::THRESH_BINARY);
        cv::cuda::threshold(gpu_differenceImage, gpu_thresholdImage, 50, 255, cv::THRESH_BINARY);

        gpu_thresholdImage.download(result);

	// Find the location of any moving object and show the final frame
	if (toggle == 0) {
		//searchForMovement(thresholdImage,frame0);
                searchForMovement(result,frame0_warped);
	        imshow("Frame", frame0_warped);

		cv::line(frame0,inputQuad[0],inputQuad[1],Scalar(0,255,0),2);
		cv::line(frame0,inputQuad[1],inputQuad[2],Scalar(0,255,0),2);
		cv::line(frame0,inputQuad[2],inputQuad[3],Scalar(0,255,0),2);
		cv::line(frame0,inputQuad[3],inputQuad[0],Scalar(0,255,0),2);

//		imshow("Original",frame0);
	}
	else {
		//searchForMovement(thresholdImage,frame1);
                searchForMovement(result,frame1_warped);
	        imshow("Frame", frame1_warped);

		cv::line(frame1,inputQuad[0],inputQuad[1],Scalar(0,255,0),2);
		cv::line(frame1,inputQuad[1],inputQuad[2],Scalar(0,255,0),2);
		cv::line(frame1,inputQuad[2],inputQuad[3],Scalar(0,255,0),2);
		cv::line(frame1,inputQuad[3],inputQuad[0],Scalar(0,255,0),2);

//		imshow("Original", frame1);

	}

	frame_count++;

        cv::waitKey(1); //needed to show frame
    }
}
