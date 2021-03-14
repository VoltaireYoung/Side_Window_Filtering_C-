#include "used_headers.h"
#include "basic_image_processing.h"

#include <math.h>

//Structure
#include "filter.h"

#define PI acos(-1)

using namespace cv;
using namespace std;

Mat get_gaussian_kernal(filtering_information, float);

Mat gaussian_with_SWF(Mat* input_image, vector<filtering_information> informations, int number_of_filters , float sigma) {
	Mat result;

	int current_filter_index;

	//轉換成32 bits 浮點數
	Mat original_image;
	if ((*input_image).depth() != 5) {
		(*input_image).convertTo(original_image, CV_32F);
	}
	else {
		original_image = *input_image;
	}

	vector<Mat> candidates;

	Size current_keranl_size;
	Point current_kernal_anchor;

	Mat current_kernal;

	Mat current_candidate;

	for (current_filter_index = 0; current_filter_index < number_of_filters; current_filter_index++) {
		
		//Mat new_Mat = Mat::zeros((*input_image).rows, (*input_image).cols, CV_32F);
		//Mat current_candidate;

		current_kernal = get_gaussian_kernal(informations[current_filter_index], sigma);

		filter2D(original_image, current_candidate,
			informations[current_filter_index].depth,
			current_kernal,
			informations[current_filter_index].anchor,
			0,
			informations[current_filter_index].border_type);
		

		candidates.push_back(current_candidate);
	}

	if (number_of_filters == 1) {
		return current_candidate;
	}
	result = get_best(input_image, candidates, number_of_filters);

	return result;
}


Mat get_gaussian_kernal(filtering_information input_kernal_information , float sigma) {
	Mat result = Mat(Size(input_kernal_information.kernal_size.width, input_kernal_information.kernal_size.height), CV_32F);

	float molecular;
	float denominator = 2 * PI * pow(sigma , 2);
	float power;

	float center_x = (float)input_kernal_information.anchor.x;
	float center_y = (float)input_kernal_information.anchor.y;

	float normalization = (float)0;

	float current_kernal_row, current_kernal_column;
	for (current_kernal_row = 0; current_kernal_row < input_kernal_information.kernal_size.height; current_kernal_row++) {
		for (current_kernal_column = 0; current_kernal_column < input_kernal_information.kernal_size.width; current_kernal_column++) {
			power = ((-1) * (pow(center_x - current_kernal_column , 2) + pow(center_y - current_kernal_row , 2))) / (2 * pow(sigma , 2));
			result.at<float>(current_kernal_row, current_kernal_column) = exp(power) / denominator;
			normalization += result.at<float>(current_kernal_row, current_kernal_column);
		}
	}

	for (current_kernal_row = 0; current_kernal_row < input_kernal_information.kernal_size.height; current_kernal_row++) {
		for (current_kernal_column = 0; current_kernal_column < input_kernal_information.kernal_size.width; current_kernal_column++) {
			result.at<float>(current_kernal_row, current_kernal_column) /= normalization;
		}
	}

	return result;
}