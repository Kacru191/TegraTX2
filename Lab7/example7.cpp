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
#include <fstream>

//#include <stdio.h> 		// Standard input/output definitions
#include <fcntl.h>		/*file cotrol definitions*/
#include <termios.h>		/*POSIX Terminal Control Definitions */
#include <unistd.h>		/*UNIX Standard Definitions */
#include <errno.h>		/* Error Number Definitions*/


// Select Video Source
// The MP4 demo uses a ROI for better tracking of the moving object
#define TEST_LIVE_VIDEO

typedef unsigned char BYTE;

using namespace cv;
using namespace std;

int theObject[2] = {0,0};
int OldObject[8] = {0,0,0,0,0,0,0,0};
//using four frames for each x and y 
int xt = 389;//389;//x left 394
int yt = 339;// y top 314
int xt2 =930; //x right 926
int yt2 =626; //y bottom 625
int xw = xt2-xt; //hidth
int yh = yt2-yt; //width

int Xa = 651; 
int Ya = 238;
int Xb = 987;
int Yb = 233;
int Xc = 984;
int Yc = 427;
int Xd = 644;
int Yd = 421; 

char write_buffer[3];
char read_buffer[15];
int MAXCHAR = 15;
int bytes_written;
int byte_read;
int fd;				//File descriptor for the port

//void Serial_Test();
//bounding rectangle of the object, we will use the center of this as its position.
cv::Rect objectBoundingRectangle = cv::Rect(0,0,0,0);

cv::Rect roi2(xt, yt, xw,yh);//Rect roi( x left, y top, width, height);
LeastSquares LSF(roi2);
//////////////////////////////////////////////////////////////////////////////////
// Color Filter
//////////////////////////////////////////////////////////////////////////////////
int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 1;
int V_MAX = 256;
//default capture width and height
const int FRAME_WIDTH = 1280;//640
const int FRAME_HEIGHT = 720;//480
//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS=50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;
//names that will appear at the top of each window
const string windowName = "Original Image";
const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";
const string windowName3 = "After Morphological Operations";
const string trackbarWindowName = "Trackbars";

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////Left Paddle/////////////////////////////////////

int H_MIN_L = 0;//24;
int H_MAX_L = 26;//37;
int S_MIN_L = 67;//26;
int S_MAX_L = 256;//75;
int V_MIN_L = 69;//225;
int V_MAX_L = 232;//256;

int xroi_L = xt-35;//325;
int yroi_L = yt;//300;//314;
int xroi2_L = xt-5;//355;//393;  // x coordinate of the opposite corner of the drawn rectangle
int yroi2_L = yt2;//415;//625 ; // y coordinate of the oposite corner of the drawn rectangle 
int wroi_L = xroi2_L-xroi_L; //310;    // width of the roi rectangle =  ( xroi2-xroi)
int hroi_L = yroi2_L-yroi_L;
cv::Rect roi_L(xroi_L,yroi_L,wroi_L,hroi_L);   // define roi for the left paddle 
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////Right Paddle//////////////////////////////////

int H_MIN_R = 31;//54;
int H_MAX_R = 79;//105;
int S_MIN_R = 120;//132;
int S_MAX_R = 256;//256;
int V_MIN_R = 48;//138;
int V_MAX_R = 256;

int xroi_R = xt2+1;
int yroi_R = yt;
int xroi2_R = xt2+30;  // x coordinate of the opposite corner of the drawn rectangle
int yroi2_R = yt2; // y coordinate of the oposite corner of the drawn rectangle 
int wroi_R = xroi2_R-xroi_R; //310;    // width of the roi rectangle =  ( xroi2-xroi)
int hroi_R = yroi2_R-yroi_R;
cv::Rect roi_R(xroi_R,yroi_R,wroi_R,hroi_R);   // define roi for the left paddle 

////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////Field/////////////////////////////////////

int H_MIN_C = 0;
int H_MAX_C = 201;
int S_MIN_C = 0;
int S_MAX_C = 256;
int V_MIN_C = 91;
int V_MAX_C = 256;

///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////

void on_trackbar( int, void* )
{//This function gets called whenever a
	// trackbar position is changed
}


string intToString(int number){


	std::stringstream ss;
	ss << number;
	return ss.str();
}
void createTrackbars(){
	//create window for trackbars


    namedWindow(trackbarWindowName,0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
//	sprintf( TrackbarName, "H_MIN", H_MIN);
//	sprintf( TrackbarName, "H_MAX", H_MAX);
//	sprintf( TrackbarName, "S_MIN", S_MIN);
//	sprintf( TrackbarName, "S_MAX", S_MAX);
//	sprintf( TrackbarName, "V_MIN", V_MIN);
//	sprintf( TrackbarName, "V_MAX", V_MAX);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->      
    createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
    createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
    createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
    createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
    createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
    createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );


}
void createTrackbarsLEFT(){
	//create window for trackbars


    namedWindow(trackbarWindowName,0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
//	sprintf( TrackbarName, "H_MIN", H_MIN);
//	sprintf( TrackbarName, "H_MAX", H_MAX);
//	sprintf( TrackbarName, "S_MIN", S_MIN);
//	sprintf( TrackbarName, "S_MAX", S_MAX);
//	sprintf( TrackbarName, "V_MIN", V_MIN);
//	sprintf( TrackbarName, "V_MAX", V_MAX);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->      
    createTrackbar( "H_MIN", trackbarWindowName, &H_MIN_L, H_MAX, on_trackbar );
    createTrackbar( "H_MAX", trackbarWindowName, &H_MAX_L, H_MAX, on_trackbar );
    createTrackbar( "S_MIN", trackbarWindowName, &S_MIN_L, S_MAX, on_trackbar );
    createTrackbar( "S_MAX", trackbarWindowName, &S_MAX_L, S_MAX, on_trackbar );
    createTrackbar( "V_MIN", trackbarWindowName, &V_MIN_L, V_MAX, on_trackbar );
    createTrackbar( "V_MAX", trackbarWindowName, &V_MAX_L, V_MAX, on_trackbar );


}
void createTrackbarsRIGHT(){
	//create window for trackbars


    namedWindow(trackbarWindowName,0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
//	sprintf( TrackbarName, "H_MIN", H_MIN);
//	sprintf( TrackbarName, "H_MAX", H_MAX);
//	sprintf( TrackbarName, "S_MIN", S_MIN);
//	sprintf( TrackbarName, "S_MAX", S_MAX);
//	sprintf( TrackbarName, "V_MIN", V_MIN);
//	sprintf( TrackbarName, "V_MAX", V_MAX);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->      
    createTrackbar( "H_MIN", trackbarWindowName, &H_MIN_R, H_MAX, on_trackbar );
    createTrackbar( "H_MAX", trackbarWindowName, &H_MAX_R, H_MAX, on_trackbar );
    createTrackbar( "S_MIN", trackbarWindowName, &S_MIN_R, S_MAX, on_trackbar );
    createTrackbar( "S_MAX", trackbarWindowName, &S_MAX_R, S_MAX, on_trackbar );
    createTrackbar( "V_MIN", trackbarWindowName, &V_MIN_R, V_MAX, on_trackbar );
    createTrackbar( "V_MAX", trackbarWindowName, &V_MAX_R, V_MAX, on_trackbar );


}

void drawObject(int x, int y,Mat &frame){

	//use some of the openCV drawing functions to draw crosshairs
	//on your tracked image!

    //UPDATE:JUNE 18TH, 2013
    //added 'if' and 'else' statements to prevent
    //memory errors from writing off the screen (ie. (-25,-25) is not within the window!)

	circle(frame,Point(x,y),20,Scalar(0,255,0),2);
    if(y-25>0)
    line(frame,Point(x,y),Point(x,y-25),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(x,0),Scalar(0,255,0),2);
    if(y+25<FRAME_HEIGHT)
    line(frame,Point(x,y),Point(x,y+25),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(x,FRAME_HEIGHT),Scalar(0,255,0),2);
    if(x-25>0)
    line(frame,Point(x,y),Point(x-25,y),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(0,y),Scalar(0,255,0),2);
    if(x+25<FRAME_WIDTH)
    line(frame,Point(x,y),Point(x+25,y),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(FRAME_WIDTH,y),Scalar(0,255,0),2);

	putText(frame,intToString(x)+","+intToString(y),Point(x,y+30),1,1,Scalar(0,255,0),2);

}
void morphOps(Mat &thresh){

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle

	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
    //dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));

	erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);


	dilate(thresh,thresh,dilateElement);
	dilate(thresh,thresh,dilateElement);
}

void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed){

	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if(numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
                if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
					x = moment.m10/area;
					y = moment.m01/area;
					objectFound = true;
					refArea = area;
				}else objectFound = false;


			}
			//let user know you found an object
			if(objectFound ==true){
				putText(cameraFeed,"Tracking Object",Point(0,50),2,1,Scalar(0,255,0),2);
				//draw object location on screen
				drawObject(x,y,cameraFeed);}

		}else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
	}
}


///////////////////////////////////////////////////////////////////////////////////////
//SEARCH FOR PADDLES
/////////////////////////////////////////////////////////////////////////////////////////
void searchForPaddles(cv::Rect roi, cv::Mat thresholdImage,  cv::Mat &cameraFeed)//,cv::Mat HSV,cv::Mat threshold) {
{// roi: passing the Paddle ROI  
// vectors are sequence containers representing arrays that can change in size
	//Mat HSV; 
      	//Mat threshold; 
	//cv::inRange(HSV,Scalar(H_MIN_L,S_MIN_L,V_MIN_L),Scalar(H_MAX_L,S_MAX_L,V_MAX_L),threshold);

	vector< vector<Point> > contours;  // creates a vector of vectors ov cv::Points. The name is contours
        vector <Vec4i> hierarchy;          // creates a vector of vectors containing four integers. The name is hierarchy
        cv:: Rect Pad = cv::Rect(0,0,0,0);
        cv::Mat temp;

        thresholdImage.copyTo(temp);

        cv::Mat roi_temp = temp(roi);


        cv::findContours(roi_temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); // retrieves external contours; CV_RETR_CCOMP finds the contours


        if(contours.size() >0) {     // if we have some contours in ROI
        vector< vector<Point> >  largestContourVec;  // new vector
        largestContourVec.push_back(contours.at(contours.size()-1)); // add the last contour(highest in the hierarchy)
        Pad = boundingRect(largestContourVec.at(0));  // finding the bounding rectangle
        }

        // draw the ROI

        line(cameraFeed,Point(roi.x,roi.y),Point(roi.x+roi.width, roi.y),Scalar(255,0,255),2);
        line(cameraFeed,Point(roi.x,roi.y),Point(roi.x, roi.y+roi.height),Scalar(255,0,255),2);
        line(cameraFeed,Point(roi.x+roi.width,roi.y),Point(roi.x+roi.width, roi.y+roi.height),Scalar(255,0,255),2);
        line(cameraFeed,Point(roi.x,roi.y+roi.height),Point(roi.x+roi.width, roi.y+roi.height),Scalar(255,0,255),2);

        Pad.x = Pad.x +roi.x;
        Pad.y = Pad.y + roi.y;

      // draw the Paddle
        if(contours.size() >0) {
        line(cameraFeed,Point(Pad.x,Pad.y-5),Point(Pad.x+Pad.width, Pad.y-5),Scalar(0,0,255),2);
        line(cameraFeed,Point(Pad.x,Pad.y),Point(Pad.x, Pad.y+Pad.height),Scalar(0,0,255),2);
        line(cameraFeed,Point(Pad.x+Pad.width,Pad.y),Point(Pad.x+Pad.width, Pad.y+Pad.height),Scalar(0,0,255),2);
        line(cameraFeed,Point(Pad.x,Pad.y+Pad.height),Point(Pad.x+Pad.width, Pad.y+Pad.height),Scalar(0,0,255),2);
        putText(cameraFeed,"x,y (" + intToString(Pad.x)+","+intToString(Pad.y)+")",Point(Pad.x,Pad.y),1,1,Scalar(255,0,0),2);
        }
}
////////////////////////////////////////////////////////////////////////////////////////////
/////////////SEARCH FOR CORNERS
////////////////////////////////////////////////////////////////////////////////////////
void findCorners(cv::Mat src_gray,cv::Mat &cameraFeed){

	cv::Mat dst, dst_norm;
	dst=cv::Mat::zeros(src_gray.size(),CV_32FC1); //initialize to zero

	// Detector parameters
	int blockSize = 2; 
	int apertureSize = 9; //Started with a value of 3
	double k = 0.04;
	int thresh = 200;


	//Detecting corners
	cv::cornerHarris(src_gray,dst,blockSize,apertureSize,k,BORDER_DEFAULT);

	//Normalizing
	cv::normalize(dst,dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());

	//Drawing a circle around corners
	for( int j =0; j < dst_norm.rows; j++){
		for ( int i = 0; i < dst_norm.cols; i++){
			if( (int) dst_norm.at<float>(j,i) > thresh ) {
				cv::circle(cameraFeed, Point(i,j), 5, Scalar(0,255,0),2,8,0);
				//for ( int l = 0;l<=4;l++){ 
				//	cout << "Corner point: " << Point(i,j);
				//	l++;
				//}
			}
		}
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
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
	cv::Rect roi(xt, yt, xw,yh);
	//Rect roi( x_left, y_top, width, height);

	cv::Mat roi_temp = temp(roi); 
    //find contours of filtered image using openCV findContours function
	cv::findContours(roi_temp,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE );// retrieves external contours

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

		//cout << "Number of Frames: " << LSF.getFrameCount() << endl;
		//cout << "Tbar(avg of t's): " << LSF.getTbar() << endl;
		//cout << "Sum of (t-Tbar)^2: " <<LSF.getSqTbar() << endl;
		//cout << "Slope Mx: " << LSF.GetMx() << endl;
		//cout << "Slope My: " << LSF.GetMy() << endl;
		//cout << "Average of X: " << LSF.GetSumX() << endl;
		//cout << "Average of Y: " << LSF.GetSumY() << endl;	

		LSF.computeRebounds();
		ImpactPnt[0] = LSF.getRebound(1);//m_rebounds[1] is the first bounce
		cout << "Impact Point: " << ImpactPnt[0] << endl; 
		line(cameraFeed, CurrentPnt[0],ImpactPnt[0],Scalar(0,255,0),2);
		if(ImpactPnt[0] != LSF.getGoal()){
			line(cameraFeed, ImpactPnt[0],LSF.getGoal(),Scalar(0,255,0),2);//from bounce to paddle
			circle(cameraFeed,LSF.getGoal(),10,cv::Scalar(0,0,255),2);//the expected goal of ball
			cout << "Goal Point: " << LSF.getGoal() << endl;
		}
		else
			circle(cameraFeed,ImpactPnt[0],10,cv::Scalar(0,0,255),2);//the expected goal of ball

//////////////////////////////////////////////////////////////////////////////////////////	/////////////////////////////////////////////////////////////////////////////////////////////

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


	rectangle(cameraFeed,Point(xt,yt),Point(xt2,yt2),Scalar(0,255,0),2);
	//Rectangle created for region of interest
}
///////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////
////////////// moving potentiometer//////////////////
void Move_Pot(){

	
		bytes_written = 0;
		float Paddle = LSF.getGoal().y;
		int P_Min = 95;
		int P_Max = 225; 
		int P_width = P_Max-P_Min;
		int Py = 0;
		//int Serve = 0; 

		//double Y_paddle = ((Paddle-yt)/yh)*P_width + P_Min; //conversion of y value to potentiometer value
		double Y_paddle = P_Max - ((Paddle-yt)/yh)*P_width;
		cout << "Move Paddle to Position: "<< Y_paddle << endl;
		if(Y_paddle > P_Max or Y_paddle < P_Min)
		{	Py = Py;
			Py = Py +2; 
			Py = Py -2;
			Py = Py;
			cout << "Py = Py " << Py << endl;
		}
		else{
			Py = Y_paddle;
			Py = Y_paddle +2 ;
			Py = Y_paddle -2;
			Py = Y_paddle;
			cout  << "Py = Y_Paddle" << Y_paddle << endl;
		}
		//Py = 110;
		
		BYTE a;
		//BYTE Serve; 

		a = (Py&0xFF); // extract bytes from integer (Hex)
		printf("a value is = %02X\n",a);
		printf("a= %d\n",a);

		write_buffer[0] = a;
		printf("Write: %c\n", write_buffer[0]);

		bytes_written = 0;
		bytes_written = write(fd, write_buffer, 1);
		printf("In buffer:%c\n", write_buffer[0]);		

		read(fd, read_buffer, 1);
		printf("Read R= %c\n", read_buffer[0]);
}
/////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------------------------------------------
int main() {

///////////////////////////////////////////////////////////////////////////////////////
	struct termios options; 	// Terminal Options
		

	fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);	//Open tty device for RD and WR ( make sure tty USB1 matches the opened USB port
	//usleep(1000000);
	if(fd == 1) {
		printf("\n Error! in Opening ttyACM0\n");
	}
	else
		printf("\n ttyUSB0 Opened Successfully\n");
	
	tcgetattr(fd, &options);	// Get the current options for the port
	cfsetspeed(&options, B115200);  // Set the baud rates to 115200
	cfsetspeed(&options, B115200);
	options.c_cflag |= (CLOCAL |CREAD) ;   // enable the reciever  and set local mode
	options.c_cflag &= ~PARENB;		// NO PARITY
	options.c_cflag &= ~CSTOPB;		//1 stop bit
	options.c_cflag &= ~CSIZE;		// Mask data size
	options.c_cflag |= CS8;		// 8 bits
	options.c_cflag &= ~CRTSCTS;		// Disable hardware flow control


//Enable data to be processed as raw input
	options.c_lflag &= ~(ICANON | ECHO | ISIG);
	
	tcsetattr(fd,  TCSANOW , &options);	//Apply options immediately
	fcntl(fd, F_SETFL, FNDELAY);
	usleep(2000000);
/////////////////////////////////////////////////////////////////////////////////////////

	//some boolean variables for different functionality within this
	//program
    	bool trackObjects = false;
    	bool useMorphOps = false;
	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;
	//matrix storage for HSV image
	Mat HSV;
	//matrix storage for binary threshold image
	Mat threshold;
	//x and y values for the location of the object
	int x=0, y=0;
	//create slider bars for HSV filtering
	createTrackbars();
	createTrackbarsLEFT();
	createTrackbarsRIGHT();
	//video capture object to acquire webcam feed
	

    // OpenCV frame matrices
    cv::Mat frame0, frame1, frame1_warped, frame0_warped, result;

    cv::cuda::GpuMat gpu_frame0, gpu_frame1, gpu_grayImage0, gpu_grayImage1, gpu_differenceImage, gpu_thresholdImage,gpu_frame0_warped,gpu_frame1_warped;

    int toggle, frame_count;

#ifdef TEST_LIVE_VIDEO

    // Camera video pipeline
std::string pipeline = "nvcamerasrc auto-exposure=off flicker=auto tnr-mode=NoiseReduction_Fast ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720, format=(string)I420, framerate=(fraction)30/1 ! nvvidconv flip-method=0 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
#else

    // MP4 file pipeline
    std::string pipeline = "filesrc location=/home/nvidia/Downloads/pong.m4v ! qtdemux name=demux ! h264parse ! omxh264dec ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";

#endif

    std::cout << "Using pipeline: " << pipeline << std::endl;
 
    // Create OpenCV capture object, ensure it works.
    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);

    if (!cap.isOpened()) {
        std::cout << "Connection failed" << std::endl;
        return -1;
    }

	// These lines work with live video, but not with the MP4 video stream???
        //Capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
	//Capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);

	//Input Quadilateral or Image plane coordinates
	Point2f inputQuad[4];
	//Output Quadilateral or World plane coordinates
	Point2f outputQuad[4];
	//Lambda Matrix
	cv::Mat lambda( 2, 4, CV_32FC1);
	
	// The 4 points that select quadilateral on the input, from top-left in clockwise order
	// These four points are the sides of the rect box used as input
	
	//OpenCV coordinate system is based on rows and then columns.
	inputQuad[1] = Point2f(Xa,Ya);//523,79 
	inputQuad[2] = Point2f(Xb,Yb);//880, 78
	inputQuad[3] = Point2f(Xc,Yc); // 924,669 
	inputQuad[0] = Point2f(Xd,Yd);//473,655
	
	// The 4 points where the mapping is to be done, from top-left in clockwise order
	outputQuad[1] = Point2f(300,265);//300,233
	outputQuad[2] = Point2f(1018,265);//1018,233
	outputQuad[3] = Point2f(1018,642);//1018,642
	outputQuad[0] = Point2f(300,642);//300,642
	
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

    //while (frame_count < 4000) {
while(1){
//	Serial_Test(LSF.getGoal().y);
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
			           
		cv::cvtColor(frame1_warped,HSV,cv::COLOR_BGR2HSV); //convert frame from BGR to HSV colorspace

		//cv::inRange(HSV,Scalar(H_MIN_R,S_MIN_R,V_MIN_R),Scalar(H_MAX_R,S_MAX_R,V_MAX_R),threshold);//filters right side 
		cv::inRange(HSV,Scalar(H_MIN_L,S_MIN_L,V_MIN_L),Scalar(H_MAX_L,S_MAX_L,V_MAX_L),threshold);//filters left side 
		//cv::inRange(HSV,Scalar(H_MIN_C,S_MIN_C,V_MIN_C),Scalar(H_MAX_C,S_MAX_C,V_MAX_C),threshold);//filters left side 
		//cv::inRange(HSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);//filters left side 
		//findCorners(threshold,frame1_warped);	

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

	  	//cv::inRange(HSV,Scalar(H_MIN_R,S_MIN_R,V_MIN_R),Scalar(H_MAX_R,S_MAX_R,V_MAX_R),threshold);//filters right side 
		cv::inRange(HSV,Scalar(H_MIN_L,S_MIN_L,V_MIN_L),Scalar(H_MAX_L,S_MAX_L,V_MAX_L),threshold);//filters left side 
		//cv::inRange(HSV,Scalar(H_MIN_C,S_MIN_C,V_MIN_C),Scalar(H_MAX_C,S_MAX_C,V_MAX_C),threshold);//filters left side 
		//cv::inRange(HSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);//filters left side 
		//findCorners(threshold,frame0_warped);	

	
           	toggle = 0;
	}
 
	// Compute the absolte value of the difference
	//cv::absdiff(grayImage0, grayImage1, differenceImage);
	cv::cuda::absdiff(gpu_grayImage0, gpu_grayImage1, gpu_differenceImage);


	// Threshold the difference image
	//cv::threshold(differenceImage, thresholdImage, 50, 255, cv::THRESH_BINARY);
        cv::cuda::threshold(gpu_differenceImage, gpu_thresholdImage, 50, 255, cv::THRESH_BINARY);

        gpu_thresholdImage.download(result);

	//findCorners(threshold,cameraFeed);
	// Find the location of any moving object and show the final frame
	if (toggle == 0) {
		//findCorners(threshold,cameraFeed);		  
		searchForMovement(result,frame0_warped);
		//Move_Pot();
		if(trackObjects)
	        	trackFilteredObject(x,y,threshold,frame0_warped);
		//imshow("Frame", frame0_warped);
                searchForPaddles( cv::Rect (xroi_R,yroi_R,wroi_R,hroi_R), threshold, frame0_warped) ; //void searchForPaddles(cv::Rect roi, cv::Mat thresholdImage,  cv::Mat &cameraFeed)
		searchForPaddles( cv::Rect (xroi_L,yroi_L,wroi_L,hroi_L), threshold,  frame0_warped) ; //void searchForPaddles(cv::Rect roi, cv::Mat thresholdImage,  cv::Mat &cameraFeed)
		Move_Pot();
                imshow("Original",frame0);
		imshow("Frame", frame0_warped);
                //imshow("Frame1",HSV);
                //imshow("Frame2",threshold);

		cv::line(frame0,inputQuad[0],inputQuad[1],Scalar(0,255,0),2);
		cv::line(frame0,inputQuad[1],inputQuad[2],Scalar(0,255,0),2);
		cv::line(frame0,inputQuad[2],inputQuad[3],Scalar(0,255,0),2);
		cv::line(frame0,inputQuad[3],inputQuad[0],Scalar(0,255,0),2);
		//imshow("Original",frame0);
	}
	else {
		searchForMovement(result,frame1_warped);
////////////////////////////////////////////////////////////
//Serve the ball //////////////
//////////////////////////////////////////////////////////////
int Serve = 0;

BYTE Srv;

Srv = (Serve&0xFF);

write_buffer[0] = Srv;
bytes_written = 0;
bytes_written = write(fd, write_buffer,1);

/////////////////////////////////////////////////////////////////	
		//Move_Pot();		
		//findCorners(threshold,cameraFeed);
		if(trackObjects)
	        	trackFilteredObject(x,y,threshold,frame1_warped);
     	        //imshow("Frame", frame1_warped);
		searchForPaddles( cv::Rect (xroi_L,yroi_L,wroi_L,hroi_L), threshold,  frame1_warped) ; //void searchForPaddles(cv::Rect roi, cv::Mat thresholdImage,  cv::Mat &cameraFeed)
	      	searchForPaddles( cv::Rect (xroi_R,yroi_R,wroi_R,hroi_R), threshold, frame1_warped) ;  //void searchForPaddles(cv::Rect roi, cv::Mat thresholdImage,  cv::Mat &cameraFeed)
		//findCorners(threshold,cameraFeed);	
		Move_Pot();

		imshow("Original",frame1);
                imshow("Frame", frame1_warped);
                //imshow("Frame1",HSV);
                //imshow("Frame2",threshold);


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
