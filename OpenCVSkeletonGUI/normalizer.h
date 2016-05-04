#ifndef _NORMALAZER_
#define _NORMALAZER_

#include <OpenCV2/opencv.hpp>
using namespace cv;

void found_contour(Mat image,Point2f contour[]);
void normalize_image(Mat &src,Point2f srcP[]);

#endif // _NORMALAZER_