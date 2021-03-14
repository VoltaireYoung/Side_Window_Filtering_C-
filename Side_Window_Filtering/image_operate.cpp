#include "used_headers.h"

using namespace cv;
using namespace std;

Mat get_best(Mat* input_image , vector<Mat> candidates , int number_of_candidate) {
	Mat result = Mat::zeros((*input_image).rows, (*input_image).cols, CV_32FC3);

	Mat current_candidate;

	int current_candidate_index;
	int current_row, current_column, current_channel;
	
	Mat difference_map((*input_image).rows, (*input_image).cols, CV_32FC3, Scalar(256.0, 256.0, 256.0));
	
	float temp;

	Mat original_image;
	if ((*input_image).depth() != 5) {
		(*input_image).convertTo(original_image, CV_32F);
	}
	else {
		//original_image = *input_image;
		(*input_image).copyTo(original_image);
	}

	Vec3f* pointer_for_input_image;
	Vec3f* pointer_for_result;
	Vec3f* pointer_for_current_candidate;
	Vec3f* pointer_for_difference;

	Vec3f current_input_image_pixel_values;
	Vec3f current_result_pixel_values;
	Vec3f current_candidate_pixel_values;
	Vec3f current_difference_values;

	for (current_candidate_index = 0; current_candidate_index < number_of_candidate; current_candidate_index++) {
		current_candidate = candidates[current_candidate_index];

		for (current_row = 0; current_row < (*input_image).rows; current_row++) {
			pointer_for_input_image = original_image.ptr<Vec3f>(current_row);
			pointer_for_result = result.ptr<Vec3f>(current_row);
			pointer_for_current_candidate = candidates[current_candidate_index].ptr<Vec3f>(current_row);
			pointer_for_difference = difference_map.ptr<Vec3f>(current_row);

			for (current_column = 0; current_column < (*input_image).cols; current_column++) {
				current_input_image_pixel_values = pointer_for_input_image[current_column];
				current_result_pixel_values = pointer_for_result[current_column];
				current_candidate_pixel_values = pointer_for_current_candidate[current_column];
				current_difference_values = pointer_for_difference[current_column];

				for (current_channel = 0; current_channel < (*input_image).channels(); current_channel++) {
					temp = (float)((float)(current_input_image_pixel_values[current_channel]) - current_candidate_pixel_values[current_channel]);
					temp = abs(temp);

					if (temp < current_difference_values[current_channel]) {
						difference_map.at<Vec3f>(current_row, current_column)[current_channel] = temp;
						result.at<Vec3f>(current_row, current_column)[current_channel] = current_candidate_pixel_values[current_channel];
						//current_difference_values[current_channel] = temp;
						//current_result_pixel_values[current_channel] = current_candidate_pixel_values[current_channel];
					}
				}
			}
		}
	}

	return result;
}

Mat image_padding_by_OpenCV(Mat* input_image, Size kernal_information , Point anchor) {
	Mat padded_image;

	int top, bottom, left, right;
	int borderType = BORDER_REPLICATE;  ////¼g¦ºªº!!!!!

	// Initialize arguments for the filter
	//top = (int)(kernal_information[1]); bottom = top;
	//left = (int)(kernal_information[0]); right = left;
	top = anchor.y;
	bottom = kernal_information.height - anchor.y - 1;
	left = anchor.x;
	right = kernal_information.width - anchor.x - 1;

	copyMakeBorder(*input_image, padded_image, top, bottom, left, right, borderType);

	return padded_image;
}

/*
Mat get_best(Mat* input_image , Mat* candidates , int number_of_candidates) {
	Mat result;
	(*input_image).copyTo(result);

	
	int current_candidate_index;
	int current_row, current_column, current_channel;
	
	Vec3b* input_image_row_pointer;
	Vec3b* result_row_pointer;
	Vec3b* candidate_row_pointer;

	Vec3b current_pixel_values;

	for (current_candidate_index = 0; current_candidate_index < number_of_candidates; current_candidate_index++) {
		for (current_row = 0; current_row < (*input_image).rows; current_row++) {
			input_image_row_pointer = (*input_image).ptr<Vec3b>(current_row);
			result_row_pointer = result.ptr<Vec3b>(current_row);
			candidate_row_pointer = candidates[current_candidate_index].ptr<Vec3b>(current_row);

			for(current_column)
		}
	}

	int result_depth = result.depth();

	Vec3b* result_row_pointer;
	Vec3b** candidates_row_pointers = (Vec3b**)malloc(sizeof(Vec3b*) * number_of_candidates);

	int current_row, current_column, current_channel;
	int current_candidate_index;

	
}
*/
