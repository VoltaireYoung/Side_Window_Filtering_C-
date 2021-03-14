#include "used_headers.h"

using namespace cv;
using namespace std;

bool read_image(char* image_file_path, Mat* image) {

	//Ū�ɮ� &  �P�_�O�_Ū�ɲ��`
	*image = imread(image_file_path);
	if (!(*image).data) {
		printf("Read image error\n");
		return false;
	}

	return true;
}

bool show_image(char* title , Mat* image) {
	imshow(title, *image);
	return true;
}