//
//  ContoursFinder.cpp
//  ShapesDetector
//
//  Created by Vitaly Ishkulov on 12/17/13.
//  Copyright (c) 2013 Vitaly Ishkulov. All rights reserved.
//
#include <OpenCV2/opencv.hpp>
#include "ContoursFinder.h"

using namespace cv;
using namespace std;

Mat drawShapes2(Mat image) {
    Mat newI;
    image.convertTo(newI, CV_8UC1);
    return image;
}

string convertInt(int number)
{
    stringstream ss;//create a stringstream
    ss << number;//add number to the stream
    return ss.str();//return a string with the contents of the stream
}

Mat drawShapes(Mat image) {
    Mat original = image;
    cvtColor(original, image, CV_RGB2GRAY);
    std::cout<<image.channels();
	blur(image,image,Size(5,5));
    
	Point2f srcPoints[4];
        found_contour(image,srcPoints);
        //cout<<"contour"<<endl;
        normalize_image(image,srcPoints);
    //
    return image;
    //
    
	vector<vector<Point2f>> shapes;
	detectShapes(image,shapes);
	
	Point2f srcImage[4];
	srcImage[0] = Point(0,0);
	srcImage[1] = Point(image.cols,0);
	srcImage[2] = Point(image.cols,image.rows);
	srcImage[3] = Point(0,image.rows);
    
	vector<Point2f> src,dest;
	for(int i=0; i<4; i++)
	{
		src.push_back(srcImage[i]);
		dest.push_back(srcPoints[i]);
	}
    
	Mat H = findHomography(src,dest,CV_RANSAC);
	
	for(int i=0; i<shapes.size(); i++)
	{
		vector<Point2f> draw;
		Point2f center;
		float radius;
		minEnclosingCircle(shapes[i],center,radius);
		shapes[i].push_back(center);
		
		perspectiveTransform(shapes[i],draw,H);
		
		int size = (int)draw.size();
		center = draw[--size];
		draw.pop_back();
		for(int k=0; k<size; k++)
		{
			Point2f a = draw[k];
			Point2f b = draw[(k+1)%size];
			
			line(original,a,b,Scalar(255,255,255),20);
		}
        //cv::putText(original, convertInt(size), center, FONT_HERSHEY_SIMPLEX, 2.0, Scalar(0,0,0), 6);
        cv::putText(original,convertInt(size),center,FONT_HERSHEY_SIMPLEX,2,Scalar(0,0,0),6);
	}
	resize(original,original,Size(1000,750));
    return original;
}