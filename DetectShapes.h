#ifndef _DETECTSHAPES_
#define _DETECTSHAPES_

#include <iostream>
#include <OpenCV2/opencv.hpp>

using namespace cv;

void getPolygon(vector<Point2f> contour,vector<Point2f> &polygon);
void detectShapes(Mat src,vector<vector<Point2f>> &shapes);

#endif // _DETECTSHAPES_
