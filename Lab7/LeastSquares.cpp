//LeastSquares.cpp

#include "LeastSquares.h" 

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>

using namespace std; 


LeastSquares::LeastSquares(cv::Rect roi)
{
	//cv::Rect roi;
	cv::Point2i m_array[8];
	cv::Point2i m_rebounds[8];
	cv::Point2i m_goalPnt[1];
	unsigned int m_time_array[8];
	m_size = 0;
	m_index = 0;
	m_x_slope = 0.0;
	m_y_slope = 0.0; 
	m_frame = 0;
	m_Tbar =0.0;
	m_SqTbar=0;
	m_SumX=0;
	m_SumY=0;
	m_SumT=0;

	m_mx = 0;//slope of x
	m_my = 0;//slope of y
	m_xleft = 389;//xroi
	m_xright = 930;//yroi
	m_ytop = 339;//xroi2
	m_ybot = 626;//yroi2
	m_xb = 0;//x bounce point
	m_yb = 0;//y bounce point
	


//intializes parameters 

}

void LeastSquares::addPoint(cv::Point2i point)
{

	//m_index++;//move to the next index
	
	if( m_index > 7)//previously used the last slot, reset, includes the 7th slot
		m_index=0; //resets index once it reaches the end of the 8 slots 
	else 
		m_index++;
	
	m_array[m_index]=point; //add point to current index 
	m_time_array[m_index]=m_frame;
	m_frame ++;

	if(m_size < 8)
		m_size++;//increments, which will adjust getSize	
	
	//returns nothing 
}
unsigned int LeastSquares::getSize()
{ //Need to keep track of the number of valid points we have stored
//Looking at what m_array is currently sized at

return m_size; //returns an integer 
//once m_size = 8, stays 8 forever
}

unsigned int LeastSquares::getFrameCount()
{
	return m_frame; 
}

int LeastSquares::testMonotonic()
{
	int index = m_index;
	int j =0;
	int k =0;
	int m =0;
	int n =0;
	int monox =0;
	int monoy =0;

	cv::Point2i arraydiff[7];


	for(int i=0;i<=6;i++)
	{

				
		
		//cout << "array: " <<arraydiff[i] << endl;
		//cout << "m_array: " << m_array[index]<< endl;	
		//cout << "m_index: " << m_index << endl;	

		if( index == 0){//previously used the last slot, reset, includes the 7th slot
			arraydiff[i]=m_array[0]-m_array[7];			
			index=7; //resets index once it reaches the end of the 8 slots

		}
		else{
			arraydiff[i] = m_array[index]-m_array[index-1];
			index--;
		}	
	//cout << "j: " << j << endl;
	//cout << "k: " << k << endl;
	//cout << "m: " << m << endl;
	//cout << "n: " << n << endl;
	

	}
	for(int t=0; t<=6;t++)
	{
		if(arraydiff[t].x<0)
			j++;
		else{
			k++;
		}
		if(arraydiff[t].y<0)
			m++;
		else{
			n++;
		}
	}
	
//	cout << "[x]: " <<  arraydiff[0].x << " " << arraydiff[1].x<< " " << arraydiff[2].x<< " " << arraydiff[3].x<< " " << arraydiff[4].x<< " " << arraydiff[5].x<< " " << arraydiff[6].x << endl;
//	cout << "[y]: " <<  arraydiff[0].y << " " << arraydiff[1].y<< " " << arraydiff[2].y<< " " << arraydiff[3].y<< " " << arraydiff[4].y<< " " << arraydiff[5].y<< " " << arraydiff[6].y <<endl;
	
//	cout << endl;

	if(j==7 || k==7)
		monox= 1;
	else
		monox = 0;
	if(m==7 || n==7)
		monoy= 1;
	else 
		monoy = 0;
	if(monox == 1 && monoy ==1)
		return 1; 
	else 
		return 0;
	
//returns true or false  

}

void LeastSquares::computeLSF()
{

	int CountF = getFrameCount();
	double tTbar= 0;
	double sumtTbar=0;
	double t;
	double tx;
	double ty;
	double SumT= 0;
	double sumSqTbar=0;
	double Tbar =4.5;
	double currentX =0;
	double currentY = 0; 
	double SumMx = 0;
	double SumMy = 0;

	SumXY();

	for(int i = 8 ; i>=0; i--)
	{
		SumT += CountF -i; //sum of all t's
	}	
	m_Tbar = SumT/8;//average of t's

	for(int i = 1; i<=8; i++)
	{
		tTbar= m_time_array[i-1] - m_SumT;// t-Tbar
		//sumtTbar += tTbar;// sum of (t-Tbar)
		sumSqTbar += tTbar*tTbar;//sum of (t-Tbar)^2
//		cout << "Current X: " << m_array[i-1].x << ", Current Y: " << m_array[i-1].y << endl;

		SumMx += tTbar*(m_array[i-1].x-m_SumX);
		SumMy += tTbar*(m_array[i-1].y-m_SumY);

		
		
	}
	m_SqTbar = sumSqTbar;	

	m_mx = SumMx/m_SqTbar;
	m_my = SumMy/m_SqTbar;







//return; //returns nothing

}
double LeastSquares::GetMx()
{
	return m_mx;
}
double LeastSquares::GetMy()
{
	return m_my;
}
void LeastSquares::SumXY()
{
	int SumX =0;
	int SumY =0;
	int SumT =0;
	//unsigned int time_array[8];

	for(int i=0;i<=7;i++){
		SumX +=m_array[i].x; //sum of X 
		SumY +=m_array[i].y; //sum of Y
		SumT +=m_time_array[i];
	}
	m_SumX = SumX/8;
	m_SumY = SumY/8;
	m_SumT = SumT/8;
}
double LeastSquares::GetSumX()
{
	return m_SumX;
}
double LeastSquares::GetSumY()
{
	return m_SumY;
}
double LeastSquares::getTbar()
{
	return m_Tbar; 
}

double LeastSquares::getSqTbar()
{
	return m_SqTbar;
}
void LeastSquares::computeRebounds() //number of rebounds
{
	int i=0;
	int j=0;
	int currentX=0;
	int currentY=0;


	int tx =0;
	int tx1=0;
	int ty=0;
	int ty1=0;
	
	int xb1=0;
	int yb1=0;
	
	int xb0=0;
	int yb0=0;

	double tp=0;
	double y_diff =0;
	double x_hit=0;
	double y_hit=0;
	double t_hit=0;

	cv::Point2i ROIbound[2];

	currentX = m_array[m_index].x;
	currentY = m_array[m_index].y;
	
	
	//ty = (m_ytop - currentY)/m_my;
	//ty1 = (m_ybot - currentY)/m_my;

//	cout << "CurrentX: " << currentX << ", CurrentY: " << currentY << endl;
//	cout << "Tx left: " << tx <<endl;
//	cout << "Tx right: " << tx1 <<endl;
//	cout << "Ty top: " << ty << endl;
//	cout << "Ty bottom: " << ty1 << endl;


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
	if(m_mx<0){ //moving left
		tx = (m_xleft - currentX)/m_mx;
		ty = (m_ytop - currentY)/m_my;
		ty1 = (m_ybot - currentY)/m_my;

		xb0 = m_mx*tx + currentX;//calculates first impact
		yb0 = m_my*tx + currentY;

		ROIbound[0]=ROIbounds(xb0,yb0);

		//m_goalPnt[0] = ROIbound[0];
		cout << "Goal PointL: " << m_goalPnt[0].x << "," << m_goalPnt[0].y << endl;

		//////////////////////////////////////////////////////////////////////
		if(m_my<0){//moving up and left
			
			if(ty>tx){ //no bounce 

				xb1 = m_xleft;
				yb1 = m_my*tx + currentY;

				cout << "UP/LEFT: NO HIT" << endl;

				m_rebounds[1] = cv::Point2i(xb1,yb1);

			}//end if	
			///////////////////////////////////////////////////////////////////////
			else if(ty<tx){// hits upper wall

				xb1 = m_mx*ty + currentX;//calculates first impact
				yb1 = m_ytop;
		
				cout << "UP/LEFT: HITS" << endl;
				m_rebounds[1] = cv::Point2i(xb1,yb1);		

			}//end else if
		
		}//end if up/left
///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////		
		
		else if(m_my>0) {//moving down and left
			if(ty1 > tx){ // no bounce  

				xb1 = m_xleft;
				yb1 = m_my*tx + currentY;
			
				m_rebounds[1] = cv::Point2i(xb1,yb1);
				cout << "DOWN/LEFT: NO HITS" << endl;

			}
			//////////////////////////////////
			else if(ty1<tx){//hits bottom wall
				xb1 = m_mx*ty1 + currentX;
				yb1 = m_ybot;
			
				m_rebounds[1] = cv::Point2i(xb1,yb1);
				cout << "DOWN/LEFT: HITS" << endl;		
			}//end hits bottom
				
		}//end else if down/left
	
}//end moving left

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
	if(m_mx>0){ //moving right
		tx1 = (m_xright - currentX)/m_mx;
		ty = (m_ytop - currentY)/m_my;
		ty1 = (m_ybot - currentY)/m_my;

		xb0 = m_mx*tx1 + currentX;//calculates first impact
		yb0 = m_my*tx1 + currentY;

		ROIbound[0]=ROIbounds(xb0,yb0);

		cout << "Goal PointR: " << m_goalPnt[0].x << "," << m_goalPnt[0].y << endl;

		if(m_my<0){//moving up and right
		
			if(ty < tx1){ // moving right hits top wall
	
				xb1 = m_mx*ty + currentX;
				yb1 = m_ytop;
			
				m_rebounds[1] = cv::Point2i(xb1,yb1);
				cout << "UP/RIGHT: HITS" << endl;
			}
///////////////////////////////////////////////////////////////////////////////////////////
			else if(ty > tx1){ //moving right no bounce

				//xb1 = m_mx*tx1 + currentX;
				xb1 = m_xright;
				yb1 = m_my*tx1 + currentY;
			
				m_rebounds[1] = cv::Point2i(xb1,yb1);
				cout << "UP/RIGHT: NO hits" << endl;
			}

		}
/////////////////////////////////////////////////////////////////////////////////////////////////
		else if(m_my>0){//moving down and right
			if(ty1 < tx1){ //moving right hits bottom wall 
	
				xb1 = m_mx*ty1 + currentX;
				yb1 = m_ybot;

				m_rebounds[1] = cv::Point2i(xb1,yb1);//first bounce	
				cout << "DOWN/RIGHT: HITS" << endl;	

			}
			/////////////////////////////////////////////	
			else if(ty1 > tx1){ // no bounce
			
				xb1 = m_xright;
				yb1 = m_my*tx1 + currentY;

				m_rebounds[1] = cv::Point2i(xb1,yb1);	
				cout << "DOWN/RIGHT: NO HITS" << endl;
			}
		}//end if right/down 

	}//end moving right if


}//end function
////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////
cv::Point2i LeastSquares::ROIbounds(int p,int n){
	
	int y_diff = 0;
	cv:: Point2i Newbounds[2];
	Newbounds[0].x=p;

//	cout << "Before ROIbounds Loop" << endl;
	
	if( n < m_ytop){//if ball goes passed the top boundary
    		        //adjust point by getting difference
		y_diff =(2*m_ytop-n);
		m_goalPnt[0] = cv::Point2i(p,y_diff);
		cout << "Goal Point --> y < ytop: " << m_goalPnt[0].y <<endl;

		Newbounds[0].y= y_diff;
	}
	else if ( n > m_ybot){//if ball goes passed the bottom boundary
			 //adjust point by getting difference
		y_diff =(2*m_ybot-n);
		m_goalPnt[0] = cv::Point2i(p,y_diff);
		cout << "Goal Point --> y > ybot: " << m_goalPnt[0].y << endl;
		//n = y_diff;
		Newbounds[0].y=y_diff;
	}
	else{ //stays within ROI, use points
		m_goalPnt[0]=cv::Point2i(p,n);
		Newbounds[0] = m_goalPnt[0];
		cout << "Goal Point --> y is neither above or below ROI: " << m_goalPnt[0].y << endl;
	}
//	cout << "Outside of ROIbounds Loop" << endl;

	return Newbounds[0];


}

cv::Point2i LeastSquares::getRebound(int n) //determines where the rebound is
{
	return m_rebounds[n];

//return; //returns a Point2i (contour)

}
cv::Point2i LeastSquares::getGoal() //determines where the rebound is
{

	return m_goalPnt[0];

}
