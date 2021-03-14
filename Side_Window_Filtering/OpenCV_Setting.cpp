#include<stdio.h>
#include<stdlib.h>

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/types.hpp>

#include "OpenCV_Setting.h"

using namespace cv;
using namespace std;

void test_opencv_setting() {
	char test_image_fileName[] = "C:\\Users\\user\\Desktop\\1.png";
	Mat test_image = imread(test_image_fileName);
	imshow("Test OpenCV Setting", test_image);

	waitKey(500);
}