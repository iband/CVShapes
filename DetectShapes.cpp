#include "DetectShapes.h"
using namespace cv;
using namespace std;


double const AngleShapes[] = {64,94,112,124,139,148}; 

Point2f NormalizePoint(Mat src,Point2f a);
Point2f RotateVector(Point2f a,Point2f b, double cosA,double sinA);

bool IsInternalAngle(int i, vector<Point> polygon,double area)
{
	int size = (int)polygon.size();
	Point a = polygon[((i-1)+size)%size];
	Point b = polygon[(i)%size];
	Point c = polygon[(i+1)%size];

	Point v1 = a-b;
	Point v2 = c-b;

	double SignSin = v1.x*v2.y - v1.y*v2.x;
	
	return (area*SignSin<0);
}


void getPolygon(vector<Point> contour,vector<Point> &polygon)
{
	polygon.clear();
	vector<Point> tempPolygon;
	approxPolyDP(contour,tempPolygon,arcLength(contour,true)*0.02,true);
	
	int size = (int)tempPolygon.size();
	double area = contourArea(tempPolygon,true);
	if (area>0)
	{
		for(int i=size-1; i>=0; i--)
		{
			polygon.push_back(tempPolygon[i]);
		}
	}
	else
	{
		for(int i=0; i<size; i++)
		{
			polygon.push_back(tempPolygon[i]);
		}
	}

}
double Length(Point a)
{
	return sqrt(double(a.x*a.x + a.y*a.y));
}
double SinAngle(int i, vector<Point> polygon)
{
	int size = (int)polygon.size();
	Point a = polygon[(i-1+size)%size];
	Point b = polygon[(i)%size];
	Point c = polygon[(i+1)%size];

	Point v2 = c-b;
	Point v1 = a-b;

	return (v1.x*v2.y - v1.y*v2.x)/(Length(v1)*Length(v2));
}
int TypeShape(double cos)
{
	double angle = fabs(acos(cos)*180/CV_PI);
	cout<<"current angle: "<<angle<<endl;
	int i=0;
	while(AngleShapes[i++]<angle);

	return (i-1);
//	return 360/(180-SinShapes[i-1]+4);
}

void detectShapes(Mat src,vector<vector<Point2f>> &shapes)
{
	Mat bin_image;
	threshold(src,bin_image,120,255,THRESH_BINARY_INV);
    
    //
    //imshow("123", bin_image);
    //

	Mat temp;
	resize(bin_image,temp,Size(1000,750));

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(bin_image, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<Point> polygon;
	shapes.clear();
	bool DetectShapes[] = {false,false,false,false,false,false};
	

	for (int i = 0; i < contours.size(); i++)
	{
		getPolygon(contours[i],polygon);
		int size = (int)polygon.size();
		double area = contourArea(polygon,true);
		
		cout<<"size: "<<size<<endl;
		for(int i=0; i<size; i++)
		{
			if (IsInternalAngle(i,polygon,area))	
			{
				int startIndex = i;
				if (!IsInternalAngle(i+1,polygon,area))
				{
					startIndex = i-1;
				}
				Point a, b,c;
				a = polygon[(startIndex+size)%size];
				b = polygon[(startIndex+1)%size];
				c = polygon[(startIndex+2)%size];			

				Point v1 = (a-b);
				Point v2 = (c-b);
				double tempCos = (v1.x*v2.x + v1.y*v2.y)/(Length(v1)*Length(v2));

				int index = TypeShape(tempCos);
				
				if (size>2 && fabs(area)>1000 && !DetectShapes[index])
				{
					DetectShapes[index] = true;
				
					double angle = AngleShapes[index]-4;//180 - 360/n;
					int n = 360/(180-angle);
					double rad = (double)angle/180*CV_PI;
				
					double cos1 = cos(rad);
					double sin1 = sin(rad);
				
					vector<Point2f> shape;
					shape.push_back(NormalizePoint(src,a));
					shape.push_back(NormalizePoint(src,b));
					for(int i=0; i<n-2; i++)
					{
						Point nextPoint = RotateVector(a,b,cos1,sin1);
						shape.push_back(NormalizePoint(src,nextPoint));
						a=b;
						b=nextPoint;
					}
					shapes.push_back(shape);
					cout<<n<<endl;
				}
			}
		}
	}
}

Point2f RotateVector(Point2f a,Point2f b, double cosA,double sinA)
{
	Point side = a-b;
	Point2f direction = Point2f(side.x*cosA -side.y*sinA,side.x*sinA+side.y*cosA);
	return direction+b;
}
Point2f NormalizePoint(Mat src,Point2f a)
{
	return a;
	//return Point2f(a.x/src.cols(),a.y/src.rows());
}