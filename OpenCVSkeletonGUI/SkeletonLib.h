//
//  SkeletonLib.h
//  OpenCVSkeletonGUI
//
//  Created by Vitaly Ishkulov on 10/12/13.
//  Copyright (c) 2013 Vitaly Ishkulov. All rights reserved.
//

#ifndef __OpenCVSkeletonGUI__SkeletonLib__
#define __OpenCVSkeletonGUI__SkeletonLib__

#include <iostream>
#include <OpenCV2/opencv.hpp>
using namespace cv;

Mat extractSkeleton(Mat initImage);
Mat blurImage(Mat initImage, int blurStrenght);

#endif /* defined(__OpenCVSkeletonGUI__SkeletonLib__) */
