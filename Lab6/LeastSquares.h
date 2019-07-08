#ifndef LeastSquares_h
#define LeastSquares_h

//LeastSquares.h
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>

class LeastSquares{

	private:
		cv::Rect roi;
		cv::Point2i m_array[8]; //vector array
		cv::Point2i m_rebounds[8];
		cv::Point2i m_goalPnt[1];
		unsigned int m_time_array[8]; 		
		unsigned int m_size;
		unsigned int m_index;
		unsigned int m_frame;
		double m_Tbar;
		double m_SqTbar;
		double m_x_slope;
		double m_y_slope;
		double m_SumX;
		double m_SumY;
		double m_SumT;		
		double m_mx;
		double m_my;
		int m_xleft;
		int m_xright;
		int m_ytop;
		int m_ybot;
		int m_xb;
		int m_yb;
	
	
	public: 
		LeastSquares(cv::Rect roi); //constructor that sets/initializes parameters
		void addPoint(cv::Point2i point); //Adds new point at index and increments index 
		unsigned int getSize();	// Returns number of elements in the array(size) 
		int testMonotonic();	//Returns >0 if all x and y values monotonic, 0 if they are equal
		double getSqTbar();
		void SumXY();
		double GetMx();
		double GetMy();
		double GetSumX();
		double GetSumY();
		double getTbar();
		unsigned int getFrameCount();
		cv::Point2i ROIbounds(int p, int n, int direction); //keeps the rebounds within the ROI
		void computeLSF();	//Updates x and y slopes 
		void computeRebounds(); //Returns number of rebounds until horizontal ROI boundary 
		cv::Point2i getRebound(int n); //returns a point corresponding to rebound n
		cv::Point2i getGoal();
};

#endif
		
	
