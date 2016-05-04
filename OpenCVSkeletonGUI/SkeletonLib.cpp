//
//  SkeletonLib.cpp
//  OpenCVSkeletonGUI
//
//  Created by Vitaly Ishkulov on 10/12/13.
//  Copyright (c) 2013 Vitaly Ishkulov. All rights reserved.
//

#include "SkeletonLib.h"
using namespace cv;

Mat extractSkeleton(Mat initImage) {
    
    
    Mat img0;
    cvtColor(initImage, img0, CV_RGB2GRAY);
    Mat t0;
    img0.copyTo(t0);
    Mat kernel = getStructuringElement(MORPH_CROSS, Size(3,3));
    erode(img0, t0, kernel);
    dilate(t0, t0, kernel); //initial erode + dilatation on image copy t0
    Mat result = img0 - t0;

    while (countNonZero(img0 > 0)) {
        erode(img0, img0, kernel);
        
        erode(img0, t0, kernel);
        dilate(t0, t0, kernel);
        result += (img0 - t0);
    }

    return result;
}

Mat blurImage(Mat initImage, int blurStrenght) {
    int odd = blurStrenght%2 == 0 ? blurStrenght + 1 : blurStrenght;
    Size kernelSize = Size(odd, odd);
    Mat result;
    GaussianBlur(initImage, result, kernelSize, 0);
    return result;
}

/*
 //Size imSize = initImage.size();
 Mat imagesArray[51];
 imagesArray[0] = initImage;
 Mat nextImage;
 
 vector<int> compression_params;
 compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
 compression_params.push_back(60);
 
 for (int i = 0; i < 50; i++) {
 int odd = i%2==0 ? i+1 : i;
 //                int odd = 2*i+1;
 GaussianBlur(imagesArray[i], nextImage, Size(odd,odd), 0);
 imagesArray[i+1] = nextImage;
 imwrite("/Users/IVA/Documents/Dropbox/Documents/University/Computer Vision/OpenCVSkeletonGUI/OpenCVSkeletonGUI/output/img.jpg", imagesArray[0], compression_params);
 }
 //VideoWriter::VideoWriter("result.avi", CV_FOURCC('M','P','4','V'), 15, imSize, true);
 return imagesArray[50];
*/