#include "normalizer.h"
#include "exceptions.h"
#include <OpenCV2/opencv.hpp>
#include "iostream"

using namespace cv;
using namespace std;

void SortFour(vector<Point> points,Point2f src[]);

///ищет контур листа, принимает одноканальное изображение, результат записывает в массив contour[]
void found_contour(Mat image, Point2f contour[])
{
	Mat bin_image;
	threshold(image,bin_image,100,255,THRESH_BINARY);

	Mat temp;
	resize(bin_image,temp,Size(1000,750));
	//imshow("bin",temp);
	//waitKey(0);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	
	findContours(bin_image, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	
	vector<Point> polygon;
	approxPolyDP(contours[0],polygon,arcLength(contours[0],true)*0.02,true);
	cout<<"polygon size: "<<polygon.size()<<endl;
	if (polygon.size()!=4)	throw new NumOfVertException();
	
	SortFour(polygon,contour);
}
//нормализация области srcPoints[] из изображения src
//результат записывает обратно в src
void normalize_image(Mat &src,Point2f srcPoints[])
{
	//примерное соотношение A4
	int w = 900;
	int h = 630;
	
	Point2f destP[4];
	destP[0] = Point2f(0,0);
	destP[1] = Point2f(w,0);
	destP[2] = Point2f(w,h);
	destP[3] = Point2f(0,h);

	//получаем матрицу для перспективного преобразования
	Mat transform = getPerspectiveTransform(srcPoints,destP);

	Mat result = Mat(w,h,1);
	//перспективное преобразование
	warpPerspective(src,src,transform,Size(w,h),INTER_LANCZOS4);
}
void SortFour(vector<Point> points,Point2f src[])
{
	double min = points[0].x+points[0].y;
	int index = 0;
	for(int i=1; i<points.size(); i++)
	{
		double temp = points[i].x+points[i].y;
		if (temp<min)
		{
			min = temp;
			index = i;
		}
	}
	double area = contourArea(points,true);
	for(int i=0; i<points.size(); i++)
	{
		if (area<0)
		{
			src[i] = points[(index-i)%points.size()];
		}
		else
		{
			src[i] = points[(index+i)%points.size()];
		}
	}
}