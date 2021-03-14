#include "used_headers.h"
#include "basic_image_processing.h"

//Structure
#include "filter.h"

using namespace cv;
using namespace std;

Mat box_with_SWF(Mat* input_image , vector<filtering_information> informations , int number_of_filters) {
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

	Mat current_candidate;

	for (current_filter_index = 0; current_filter_index < number_of_filters; current_filter_index++) {
		//Mat new_Mat = Mat::zeros((*input_image).rows, (*input_image).cols, CV_32F);

		// 還沒處理 Size 跟 Anchor
		boxFilter(original_image, current_candidate, 
			      informations[current_filter_index].depth, 
			      informations[current_filter_index].kernal_size, 
			      informations[current_filter_index].anchor, 
			      true, 
			      informations[current_filter_index].border_type);

		candidates.push_back(current_candidate);
	}

	if (number_of_filters == 1) {
		return current_candidate;
	}

	result = get_best(input_image , candidates , number_of_filters);

	return result;
}
