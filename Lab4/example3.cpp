//-----------------------------------------------------------------------------------------------------------------
// File:   example2.cpp
// Author: Allan A. Douglas
//-----------------------------------------------------------------------------------------------------------------

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string> 
#include <sstream>
#include <stdio.h>

#include <opencv2/cudacodec.hpp>

// Select Video Source
// The MP4 demo uses a ROI for better tracking of the moving object
//#define TEST_LIVE_VIDEO


using namespace cv;
using namespace std;

int theObject[2] = {0,0};
int OldObject[8] = {0,0,0,0,0,0,0,0};
//using four frames for each x and y 
int xt = 525;//x left
int yt = 90;// y top
int xt2 = 823; //x right
int yt2 = 628; //y bottom
int xw = xt2-xt; //hidth
int yh = yt2-yt; //width
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

    cv:Rect roi(xt, yt, xw,yh);
	//Rect roi( x left, y top, width, height);

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

/*    int x = theObject[0]+xt; //creates origin so offset is to set this to (0,0)
    int y = theObject[1]+yt; //Top left corner
    int x1 = OldObject[0]+xt;//previous position of x + offset 568
    int y1 = OldObject[1]+yt;//previous position of y + offset 140
    int x2 = OldObject[2]+xt;//previous position of x + offset 568
    int y2 = OldObject[3]+yt;//previous position of y + offset 140
    int x3 = OldObject[4]+xt;//previous position of x + offset 568
    int y3 = OldObject[5]+yt;//previous position of y + offset 140
    int x4 = OldObject[6]+xt;//previous position of x + offset 568
    int y4 = OldObject[7]+yt;//previous position of y + offset 140


        double xpos1 = (x-x1);
        double ypos1 = (y-y1);
        double dist = sqrt(pow(xpos1,2)+pow(ypos1,2));
        double Vavg = dist*0.33;
        double V0 = ypos1/xpos1;

        cout << "X position: " << xpos1 << endl;
        cout << "Y Position: " << ypos1 << endl;
        cout << "Distance: " << dist << endl;

        cout << "Average velocity: " << Vavg << endl;
        cout << "Velocity: " << V0 << endl;
*/


#else

	int x = theObject[0]+xt; //creates origin so offset is to set this to (0,0)
	int y = theObject[1]+yt; //Top left corner
	int x1 = OldObject[0]+xt;//previous position of x + offset 568
	int y1 = OldObject[1]+yt;//previous position of y + offset 140
	int x2 = OldObject[2]+xt;//previous position of x + offset 568
	int y2 = OldObject[3]+yt;//previous position of y + offset 140
	int x3 = OldObject[4]+xt;//previous position of x + offset 568
	int y3 = OldObject[5]+yt;//previous position of y + offset 140

	cout << x << "," << y << endl; 

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


//	cout << "Change of X: " << xpos1 << endl;
//	cout << "Change of Y: " << ypos1 << endl;
//	cout << "Distance(magnitude): " << dist << endl;
//	cout << "average V" << avg << endl;
//	cout << "Average velocity: " << Vavg << endl;
//	cout << "Velocity (Slope Y/X) : " << V0 << endl;

//        char msg[18];
//        sprintf(msg,"Velocity %0.5f\n\r",V0);
 
#endif

    //draw some crosshairs around the object
    circle(cameraFeed,Point(x,y),10,cv::Scalar(0,255,0),2);
    line(cameraFeed,Point(x,y),Point(x,y-25),Scalar(0,255,0),2);
    line(cameraFeed,Point(x,y),Point(x,y+25),Scalar(0,255,0),2);
    line(cameraFeed,Point(x,y),Point(x-25,y),Scalar(0,255,0),2);
    line(cameraFeed,Point(x,y),Point(x+25,y),Scalar(0,255,0),2);

//	int x4 = OldObject[6]+xt;//previous position of x + offset 568
//	int y4 = OldObject[7]+yt;//previous position of y + offset 140


	rectangle(cameraFeed,Point(xt,yt),Point(xt2,yt2),Scalar(255,0,0),2);
	//Rectangle created for region of interest
//****************************DRAWING VECTORS****************************
//*******************************part for going down*********************
// drawing the vector line:
//double xleft, yleft,yleftd,xright,xrightd, yrightd,xleftd;

if (y-y1  > 0 ) {  // going down

	

		if(xpos1 < 0) {//going left

		//	putText(cameraFeed,"going down and left (" + intToString(x+30)+","+intToString(y+30)+")",Point(100,200),1,1,Scalar(255,0,153),2);
			double R;
			//yleftd = ((y3-y)/(x3-3)) *(xt-x3) + y3; //calculate where the rebound will be off the left wall
			yleftd = (ypos1/xpos1)*(xt-x3)+y3; 
			cout << "yleftd calculated: " << yleftd << endl;
		//	R = yt - (yt -y3)/(xleftd - x3)* (xleftd - xt2) ; // x-coordinate on the lower edge of ROI in case the ball never reflects from the right side
			if (yleftd < yt2)//it has not hit the bottom yet
			 {
				line(cameraFeed, Point(x,y),Point(xt,yleftd),Scalar(0,255,0),2);
				//cout << "down Calculated point R" << " " << R << endl;
				//xleftd = ((x3-xt)/(y3-yleftd)) *(yt2 - y3) + x3;//calculate x after bounce
//				
				xleftd = (xpos1/ypos1)*(yleftd-yt2)+xt;
				cout << "xleftd calculated: " << xleftd << "\n\n" << endl; 
				arrowedLine(cameraFeed, Point(xt,yleftd),Point(xleftd,yt2),Scalar(0,255,0),2);
			}
			else {
				//xleftd = ((x3-x)/(y3-y)) *(yt2 - y3) + x3;//calculate x with no bounce 
				xleftd = xt - (xt-x3)/(yleftd-y3)*(yleftd-yt2);
				arrowedLine(cameraFeed, Point(x,y),Point(xleftd,yt2),Scalar(0,255,0),2);
	//			line(cameraFeed, Point(xleftd,yt),Point(xt2,yleftd),Scalar(0,0,255),2);
			}
		}  

		if (xpos1 > 0) {   // if the ball goes to the right
		
			double P;  // point of intercept with yroi line 
					//P = yt2 - (xt2-xrightd)/(x - xrightd)*(yt2 -y);   // x-coordinate on the upper edge of ROI if the ball never reflects off of the right side
			//yrightd = ((y3-y)/(x3-x)) *(xt2-x3) + y3;  //calculate where the rebound will be off the right wall
			yrightd = (ypos1/xpos1)*(xt2-x3)+y3;			
			cout << "yrightd calculated: " << yrightd << endl;
			if ( yrightd < yt2) { //there is a bounce				
				
				line(cameraFeed, Point(x,y),Point(xt2,yrightd),Scalar(0,255,0),2); 
				
				//xrightd = ((x3-xt2)/(y3-yrightd))*(yt2 -y3 ) + x3;
//				cout << " xrightd calculated: " << xrightd << "\n\n" << endl;
				xrightd = (xpos1/ypos1)*(yrightd-yt2)+xt2;
				//if(xrightd > xt2)
				//	xrightd = xt2;
				//else if(xrightd < xt)
				//	xrightd = xt;

				arrowedLine(cameraFeed, Point(xt2,yrightd),Point(xrightd,yt2),Scalar(0,255,0),2);
//				cout << "xt2: " << xt2 << ", yrightd: " << yrightd << ", xrightd: " << xrightd << ", yt2: " << yt2 << endl; 
				//cout << " down Calculated point P" << " " << P << endl;
						}			
			else{
				//putText(cameraFeed,"going up and right (" + intToString(x+30)+","+intToString(y+30)+")",Point(100,200),1,1,Scalar(255,0,153),2);
				xrightd = ((x3-x)/(y3-y))*(yt2 -y3 ) + x3;

				arrowedLine(cameraFeed, Point(x,y),Point(xrightd,yt2),Scalar(0,255,0),2);
	//			line(cameraFeed, Point(xrightd,yt2),Point(xt2,yrightd),Scalar(0,0,255),2);	

				}

				} 
}

////////////////////////////////////////////////////////////part for going UP

else if (y-y1  < 0 ) {  // going up	

		if(xpos1 < 0) {//going left

		//	putText(cameraFeed,"going down and left (" + intToString(x+30)+","+intToString(y+30)+")",Point(100,200),1,1,Scalar(255,0,153),2);
			double R;
			//yleftd = ((y3-y)/(x3-3)) *(xt-x3) + y3; //calculate where the rebound will be off the left wall
			yleft = (ypos1/xpos1)*(xt-x3)+y3; 
			cout << "yleft calculated: " << yleft << endl;
		//	R = yt - (yt -y3)/(xleftd - x3)* (xleftd - xt2) ; // x-coordinate on the lower edge of ROI in case the ball never reflects from the right side
			if (yleft > yt)//it has not hit the bottom yet
			 {
				line(cameraFeed, Point(x,y),Point(xt,yleft),Scalar(0,0,255),2);
				//cout << "down Calculated point R" << " " << R << endl;
				//xleftd = ((x3-xt)/(y3-yleftd)) *(yt2 - y3) + x3;//calculate x after bounce
//				
				xleft = (xpos1/ypos1)*(yleft-yt)+xt;
				cout << "xleft calculated: " << xleft << "\n\n" << endl; 
				arrowedLine(cameraFeed, Point(xt,yleft),Point(xleft,yt),Scalar(0,0,255),2);
			}
			else {
				//xleftd = ((x3-x)/(y3-y)) *(yt2 - y3) + x3;//calculate x with no bounce 
				xleft = xt - (xt-x3)/(yleft-y3)*(yleft-yt);
				arrowedLine(cameraFeed, Point(x,y),Point(xleft,yt),Scalar(0,0,255),2);
	//			line(cameraFeed, Point(xleftd,yt),Point(xt2,yleftd),Scalar(0,0,255),2);
			}
		}  

		if (xpos1 > 0) {   // if the ball goes to the right
		
			double P;  // point of intercept with yroi line 
					//P = yt2 - (xt2-xrightd)/(x - xrightd)*(yt2 -y);   // x-coordinate on the upper edge of ROI if the ball never reflects off of the right side
			//yrightd = ((y3-y)/(x3-x)) *(xt2-x3) + y3;  //calculate where the rebound will be off the right wall
			yright = (ypos1/xpos1)*(xt2-x3)+y3;			
			cout << "yright calculated: " << yright << endl;
			if ( yright > yt) { //there is a bounce				
				
				line(cameraFeed, Point(x,y),Point(xt2,yright),Scalar(0,0,255),2); 
				
				//xrightd = ((x3-xt2)/(y3-yrightd))*(yt2 -y3 ) + x3;
//				cout << " xrightd calculated: " << xrightd << "\n\n" << endl;
				xright = (xpos1/ypos1)*(yright-yt)+xt2;
				//if(xrightd > xt2)
				//	xrightd = xt2;
				//else if(xrightd < xt)
				//	xrightd = xt;

				arrowedLine(cameraFeed, Point(xt2,yright),Point(xright,yt),Scalar(0,0,255),2);
//				cout << "xt2: " << xt2 << ", yrightd: " << yrightd << ", xrightd: " << xrightd << ", yt2: " << yt2 << endl; 
				//cout << " down Calculated point P" << " " << P << endl;
						}			
			else{
				//putText(cameraFeed,"going up and right (" + intToString(x+30)+","+intToString(y+30)+")",Point(100,200),1,1,Scalar(255,0,153),2);
				//xright = ((x3-x)/(y3-y))*(yt -y3 ) + x3;
				xright = xt2 - (yt-yright)/(y-yt)*(xt2-x);
				arrowedLine(cameraFeed, Point(x,y),Point(xright,yt),Scalar(0,0,255),2);
	//			line(cameraFeed, Point(xrightd,yt2),Point(xt2,yrightd),Scalar(0,0,255),2);	

				}

				} 
}



///////////////////////////////////////////////////////////////////////////
//
		//calculating going down 
		
/*		
		yleftd=V0*(xt-x3)+y3;
		xleftd=(-1*V0)*(yleftd-y3)+x3;
		//xleftd=-1*((yleft-yt2)/(yleft-y1))*(xt-x1)+xt;
		//xrightd=-1*((yright-yt2)/(yright-y1))*(xt2-x1)+xt2;
		yrightd= V0*(xt2-x3)+y3;
		xrightd= (1/V0)*(yrightd-y3)+x3;

		//calculating going up
		xleft = (1/V0)*(yleft-yt)+xt; 
		yleft=V0*(xleft-xt2)+yt;
		
		yright = V0*(xt2-x3)+y3;
		xright = (1/V0)*(yt-yright)+xt2; 
			
	
		//calculating going down ***olga's approach***
		//xleftd = V0*(yt-y3)+x3;
		//yleftd = (1/V0)*(xleftd-xt2) + yt;
		//double R = yt - (yt - y3)/(xleftd-xt)*(xleftd - xt2);

///////////////////////////////////////////////////////////////////////////////	
	if (ypos1>0){ //traveling down
			
		//arrowedLine(cameraFeed,Point(xpos+xt,ypos+yt), Point(xpos+xpos1*2+xt,ypos+ypos1*2+yt),Scalar(0,0,255),2);
///////////////////////////////////////////////////////////////////////////////
		if(xpos1 < 0) { //going left --DOWN
        		putText(cameraFeed,"going down and left (" + intToString(x+30)+","+intToString(y+30)+")",Point(100,200),1,1,Scalar(255,0,153),2);
			if( yleftd > yt2)
				yleftd = yt2;
			else if( yrightd > yt2) 
				yrightd = yt2;


			if(xleftd > xt2)
				xleftd = xt2;

			if(ypos1 > 0)
			//if(yleftd < yt2){//if there is a bounce off the left wall 

				line(cameraFeed,Point(x,y), Point(xt,yleftd), Scalar (0,0,255),2);
				arrowedLine(cameraFeed,Point(xt,yleftd), Point(xleftd,yt2), Scalar (0,0,255),2);
				cout << "xleftd: " << xleftd << ", yleftd: " << yleftd << endl;
			}
			else if(ypos1 < 0)
			//else if(yrightd < yt2)//if there is a SECOND  bounce of the right wall
			{	
				line(cameraFeed,Point(x,y), Point(xt,yleftd), Scalar (0,0,255),2);
				line(cameraFeed,Point(xt,yleftd), Point(xt2,yrightd), Scalar (0,0,255),2);
				arrowedLine(cameraFeed,Point(xt,yrightd), Point(xrightd,yt2), Scalar (0,0,255),2);

			}

			else{ //no bounce goes straight down 
				arrowedLine(cameraFeed,Point(x,y), Point(xleftd,yt2), Scalar (0,0,255),2);
				//arrowedLine(cameraFeed,Point(x3,y3), Point(xleftd,yt2), Scalar (0,0,255),2);
	
			}	
        
		}
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		else if(xpos1>0){//going right --DOWN
//        		putText(cameraFeed,"going down and right (" + intToString(x+30)+","+intToString(y+30)+")",Point(100,200),1,1,Scalar(255,0,153),2);

//			if(yrightd < yt2){//if there is a bounce of the right wall
		
//				line(cameraFeed,Point(x3,y3), Point(xt,yrightd), Scalar (0,0,255),2);
//				arrowedLine(cameraFeed,Point(xt,yrightd), Point(xrightd,yt2), Scalar (0,0,255),2);
//			}
//			else if(yleftd < yt2){//if there is a SECOND bounce off the left wall 

//				line(cameraFeed,Point(x3,y3), Point(xt,yleftd), Scalar (0,0,255),2);
//				line(cameraFeed,Point(xt,yleftd), Point(xt2,yrightd), Scalar (0,0,255),2);
//				arrowedLine(cameraFeed,Point(xt2,yrightd), Point(xrightd,yt2), Scalar (0,0,255),2);
//			}

//			else{//there is no bounce, straight down		
//				line(cameraFeed,Point(x3,y3), Point(xrightd,yt2), Scalar (0,0,255),2);
				//arrowedLine(cameraFeed,Point(x3,y3), Point(xleft,yt2), Scalar (255,0,0),2);
				//line(cameraFeed,Point(xpos+xt,ypos+yt), Point(xpos+xpos1*10+xt,ypos+ypos1*10+yt),Scalar(255,0,0),2);
		
//			}	
        	
//		}

//	}
///////////////////////////////////////////////////////////////////////////////
*//*	else if(ypos1<0){ //traveling up 		

	////////////////////////////////////////////////////////////////////////
		if(xpos1<0){//up and bounced of left wall
			//putText(cameraFeed,"going up and left (" + intToString(x+30)+","+intToString(y+30)+")",Point(100,200),1,1,Scalar(255,0,153),2);
			if(V0 < 0){
				line(cameraFeed,Point(x,y), Point(xt,yleft), Scalar (255,0,0),2);
				arrowedLine(cameraFeed,Point(xt,yleft), Point(xleft,yt), Scalar (255,0,0),2);			     

			}
			else if(yright > yt){//going up and right on SECOND bounce 
			//	putText(cameraFeed,"going up and right (" + intToString(x+30)+","+intToString(y+30)+")",Point(100,200),1,1,Scalar(255,0,153),2);
				line(cameraFeed,Point(x,y), Point(xt2,yright), Scalar (255,0,0),2);
				arrowedLine(cameraFeed,Point(xt2,yright), Point(xright,yt), Scalar (255,0,0),2);
			}
	
			else{//doesn't hit any wall, traveling straight 
				line(cameraFeed,Point(x,y), Point(xleft,yt), Scalar (255,0,0),2);
				arrowedLine(cameraFeed,Point(x3,y3), Point(xleft,yt), Scalar (255,0,0),2);
	
			}	
		}
		///////////////////////////////////////////////////////////////////////		
		else if(xpos1>0){//going up and right
	
			if(yright > yt){
				putText(cameraFeed,"going up and right (" + intToString(x+30)+","+intToString(y+30)+")",Point(100,200),1,1,Scalar(255,0,153),2);
				line(cameraFeed,Point(x,y), Point(xt2,yright), Scalar (255,0,0),2);
				arrowedLine(cameraFeed,Point(xt2,yright), Point(xright,yt), Scalar (255,0,0),2);
			}
			else if(yleft > yt){
				line(cameraFeed,Point(x,y), Point(xt,yleft), Scalar (255,0,0),2);
				arrowedLine(cameraFeed,Point(xt,yleft), Point(xleft,yt), Scalar (255,0,0),2);			     

			}
	
			else{
				line(cameraFeed,Point(x,y), Point(xright,yt), Scalar (255,0,0),2);
				arrowedLine(cameraFeed,Point(x3,y3), Point(xright,yt), Scalar (255,0,0),2);
			
			}
		}

	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/
/*

	if (ypos-OldObject[1]>0)
		//arrowedLine(cameraFeed,Point(xpos+xt,ypos+yt), Point(xpos+xpos1*2+xt,ypos+ypos1*2+yt),Scalar(0,0,255),2);
		
	else if(ypos-OldObject[1]<0)
		//arrowedLine(cameraFeed,Point(xpos+xt,ypos+yt), Point(xpos+xpos1*2+xt,ypos+ypos1*2+yt),Scalar(255,0,0),2);
*/


	
    //write the position of the object to the screen
    //putText(cameraFeed,"(" + intToString(x)+","+intToString(y)+")",Point(x,y),1,1,Scalar(0,255,0),2);

  //  putText(cameraFeed,"(" + intToString(x1)+","+intToString(y1)+")",Point(x1,y1),1,1,Scalar(0,255,0),2);

    //putText(cameraFeed,"x3,y3 (" + intToString(x3)+","+intToString(y3)+")",Point(100,500),1,1,Scalar(255,0,0),2);

    //putText(cameraFeed,"(" + intToString(x2)+","+intToString(y2)+")",Point(x2,y2),1,1,Scalar(0,255,0),2);








    // Print the object position
    //cout << xpos << " " << ypos << endl;
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
    inputQuad[0] = Point2f(520, 80); 
    inputQuad[1] = Point2f(880, 77);
    inputQuad[2] = Point2f(923, 672);   
    inputQuad[3] = Point2f(472, 655);

    // The 4 points where the mapping is to be done, from top-left in clockwise order
    outputQuad[0] = Point2f(437, 0);
    outputQuad[1] = Point2f(842, 0);
    outputQuad[2] = Point2f(842, 719);
    outputQuad[3] = Point2f(437, 719);

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

    while (frame_count < 1000) {

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

		//imshow("Original",frame0);
	}
	else {
		//searchForMovement(thresholdImage,frame1);
                searchForMovement(result,frame1_warped);
	        imshow("Frame", frame1_warped);

		cv::line(frame1,inputQuad[0],inputQuad[1],Scalar(0,255,0),2);
		cv::line(frame1,inputQuad[1],inputQuad[2],Scalar(0,255,0),2);
		cv::line(frame1,inputQuad[2],inputQuad[3],Scalar(0,255,0),2);
		cv::line(frame1,inputQuad[3],inputQuad[0],Scalar(0,255,0),2);

	//	imshow("Original", frame1);

	}

	frame_count++;

        cv::waitKey(1); //needed to show frame
    }
}
