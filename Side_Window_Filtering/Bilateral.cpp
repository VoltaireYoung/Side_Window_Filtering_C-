#include "used_headers.h"
#include "basic_image_processing.h"

#include <math.h>

//Structure
#include "filter.h"

#define PI acos(-1)

using namespace cv;
using namespace std;

Mat bilateral_filtering(Mat*, float, float, filtering_information);
Mat get_gaussian_kernal_spatial(filtering_information, float);
Mat get_gaussian_kernal_range(Mat*, filtering_information, float);

Mat bilateral_with_SWF(Mat* input_image, vector<filtering_information> informations, int number_of_filters, float sigma_for_spatial, float sigma_for_range) {
	Mat result;

	int current_filter_index;

	//轉換成32 bits 浮點數
	Mat original_image;
	if ((*input_image).depth() != 5) {
		(*input_image).convertTo(original_image, CV_32F);
	}
	else {
		(*input_image).copyTo(original_image);
	}

	vector<Mat> candidates;

	Size current_keranl_size;
	Point current_kernal_anchor;

	Mat current_candidate_spatial;
	Mat current_candidate_range;

	Mat current_kernal_for_spatial;
	Mat current_kernal_for_range;

	Mat current_candidate;

	for (current_filter_index = 0; current_filter_index < number_of_filters; current_filter_index++) {
		printf("Current filter index : %d\n", current_filter_index);
		current_candidate = bilateral_filtering(input_image, sigma_for_spatial, sigma_for_range, informations[current_filter_index]);

		candidates.push_back(current_candidate);
	}

	if (number_of_filters == 1) {
		return current_candidate;
	}

	result = get_best(input_image, candidates, number_of_filters);

	return result;
}
/*
Mat bilateral_filtering(Mat* input_image, float sigma_for_spatial, float sigma_for_range, filtering_information input_kernal_information) {
	Mat result;

	Mat padded_range_image;
	if ((*input_image).depth() != CV_32F) {
		(*input_image).convertTo(padded_range_image, CV_32F);
		(*input_image).convertTo(result, CV_32F);
	}
	else {
		(*input_image).copyTo(padded_range_image);
		(*input_image).copyTo(result);
	}

	Mat weight;
}

Mat get_weight(Mat* input_image , filtering_information input_kernal_information) {
	Mat result;

	// Pad image
	Mat padded_image;
	if ((*input_image).depth() != CV_32F) {
		(*input_image).convertTo(padded_image, CV_32F);
		(*input_image).convertTo(result, CV_32F);
	}
	else {
		(*input_image).copyTo(padded_image);
		(*input_image).copyTo(result);
	}
	padded_image = image_padding_by_OpenCV(&padded_image, input_kernal_information.kernal_size, input_kernal_information.anchor);

	int kernal_row, kernal_column;
	
}
*/


Mat bilateral_filtering(Mat* input_image, float sigma_for_spatial, float sigma_for_range, filtering_information input_kernal_information) {
	Mat result;

	Mat padded_range_image;
	if ((*input_image).depth() != CV_32F) {
		(*input_image).convertTo(padded_range_image, CV_32F);
		(*input_image).convertTo(result, CV_32F);
	}
	else {
		(*input_image).copyTo(padded_range_image);
		(*input_image).copyTo(result);
	}

	padded_range_image = image_padding_by_OpenCV(&padded_range_image, input_kernal_information.kernal_size, input_kernal_information.anchor);
	Mat spatial_kernal = get_gaussian_kernal_spatial(input_kernal_information, sigma_for_spatial);
	Mat current_range_block;
	Mat range_kernal;
	Mat current_range_channel;
	Mat current_range_block_channel;
	Mat weight;
	Scalar total_weight;
	Mat weighted_pixel_value;
	Scalar sum_of_weighted_pixel_value;

	int current_result_row, current_result_column, current_result_channel;
	
	vector<Mat> range_kernal_channels;
	vector<Mat> current_range_block_channels;


	//Vec3f* current_result_pixel;
	for (current_result_row = 0; current_result_row < (*input_image).rows; current_result_row++) {
		//current_result_pixel = result.ptr<Vec3f>(current_result_row);
		for (current_result_column = 0; current_result_column < (*input_image).cols; current_result_column++) {
			current_range_block = padded_range_image(Rect(current_result_column, current_result_row, input_kernal_information.kernal_size.width, input_kernal_information.kernal_size.height));
			range_kernal = get_gaussian_kernal_range(&current_range_block, input_kernal_information, sigma_for_range);

			split(range_kernal, range_kernal_channels);
			split(current_range_block, current_range_block_channels);

			for (current_result_channel = 0; current_result_channel < 3; current_result_channel++) {
				current_range_channel = range_kernal_channels.at(current_result_channel);
				weight = spatial_kernal + current_range_channel;
				cv::exp(weight, weight);
				//weight = spatial_kernal.mul(current_range_channel);
				total_weight = sum(weight);

				current_range_block_channel = current_range_block_channels.at(current_result_channel);
				weighted_pixel_value = weight.mul(current_range_block_channel);
				sum_of_weighted_pixel_value = sum(weighted_pixel_value);

				result.at<Vec3f>(current_result_row, current_result_column)[current_result_channel] = (float)(sum_of_weighted_pixel_value[0] / total_weight[0]);
			}
		}
	}

	return result;
}

Mat get_gaussian_kernal_spatial(filtering_information input_kernal_information, float sigma) {
	Mat result = Mat(Size(input_kernal_information.kernal_size.width, input_kernal_information.kernal_size.height), CV_32F);

	float power;

	float center_x = (float)input_kernal_information.anchor.x;
	float center_y = (float)input_kernal_information.anchor.y;

	float current_kernal_row, current_kernal_column;
	for (current_kernal_row = 0; current_kernal_row < input_kernal_information.kernal_size.height; current_kernal_row++) {
		for (current_kernal_column = 0; current_kernal_column < input_kernal_information.kernal_size.width; current_kernal_column++) {
			power = ((-1) * (pow(center_x - current_kernal_column, 2) + pow(center_y - current_kernal_row, 2))) / (2 * pow(sigma, 2));
			result.at<float>(current_kernal_row, current_kernal_column) = power;
		}
	}

	return result;
}

Mat get_gaussian_kernal_range(Mat* current_block, filtering_information input_kernal_information, float sigma) {
	//Mat result = Mat(Size(input_kernal_information.kernal_size.width, input_kernal_information.kernal_size.height), CV_32F);
	Mat result;
	(*current_block).copyTo(result);

	float power;

	Vec3f center_pixel = (*current_block).at<Vec3f>(input_kernal_information.anchor.y, input_kernal_information.anchor.x);

	float current_kernal_row, current_kernal_column, current_kernal_channel;
	Vec3f current_pixel;

	for (current_kernal_row = 0; current_kernal_row < input_kernal_information.kernal_size.height; current_kernal_row++) {
		for (current_kernal_column = 0; current_kernal_column < input_kernal_information.kernal_size.width; current_kernal_column++) {
			current_pixel = (*current_block).at<Vec3f>(current_kernal_row, current_kernal_column);

			for (current_kernal_channel = 0; current_kernal_channel < 3; current_kernal_channel++) {
				power = ((-1) * (pow(current_pixel[current_kernal_channel] - center_pixel[current_kernal_channel], 2))) / (2 * pow(sigma, 2));
				//result.at<float>(current_kernal_row, current_kernal_column)[current_kernal_channel] = power;
				result.at<Vec3f>(current_kernal_row, current_kernal_column)[current_kernal_channel] = power;
			}
		}
	}

	return result;
}


/*
Mat get_gaussian_kernal_spatial(filtering_information input_kernal_information, float sigma) {
	Mat result = Mat(Size(input_kernal_information.kernal_size.width, input_kernal_information.kernal_size.height), CV_32F);

	float molecular;
	float denominator = 2 * PI * pow(sigma, 2);
	float power;

	float center_x = (float)input_kernal_information.anchor.x;
	float center_y = (float)input_kernal_information.anchor.y;

	float current_kernal_row, current_kernal_column;
	for (current_kernal_row = 0; current_kernal_row < input_kernal_information.kernal_size.height; current_kernal_row++) {
		for (current_kernal_column = 0; current_kernal_column < input_kernal_information.kernal_size.width; current_kernal_column++) {
			power = ((-1) * (pow(center_x - current_kernal_column, 2) + pow(center_y - current_kernal_row, 2))) / (2 * pow(sigma, 2));
			result.at<float>(current_kernal_row, current_kernal_column) = exp(power) / denominator;
		}
	}

	return result;
}
*/

/*
Mat get_gaussian_range(Mat* current_range_block, float sigma_for_range, filtering_information input_kernal_information) {
	Mat result;
	(*current_range_block).copyTo(result);

	int current_row, current_column, current_channel;

	Vec3f* current_row_pointer;
	Vec3f anchor = (*current_range_block).at<Vec3f>(input_kernal_information.anchor.y, input_kernal_information.anchor.x);

	float temp;
	for (current_row = 0; current_row < input_kernal_information.kernal_size.height; current_row++) {
		current_row_pointer = (*current_range_block).ptr<Vec3f>(current_row);

		for (current_column = 0; current_column < input_kernal_information.kernal_size.width; current_column++) {
			for (current_channel = 0; current_channel < 3; current_channel++) {
				temp = () / ();
			}
		}
	}
}
*/


/*
Mat bilateral_filtering(Mat* input_image, float sigma_for_spatial, float sigma_for_range, filtering_information input_kernal_information) {
	Mat padded_range_image;
	Mat padded_spatial_image;
	Mat result = Mat(Size(input_kernal_information.kernal_size.width, input_kernal_information.kernal_size.height), CV_32F);

	float molecular;
	float denominator = 2 * PI * pow(sigma_for_range, 2);
	float power;

	int current_kernal_row, current_kernal_column, current_kernal_channel;
	int padded_row, padded_column;
	int row_count, column_count;

	int row_pointer;

	// get filtered spatial
	Mat kernal_for_spatial = get_gaussian_kernal_for_bilateral(input_kernal_information, sigma_for_spatial);
	filter2D(*input_image, padded_spatial_image,
		     input_kernal_information.depth,
		     kernal_for_spatial,
		     input_kernal_information.anchor,
		     0,
		     input_kernal_information.border_type);

	if ((*input_image).depth() != CV_32F) {
		(*input_image).convertTo(padded_range_image, CV_32F);
	}
	else {
		(*input_image).copyTo(padded_range_image);
	}

	// padded range image & filtered spatail image
	padded_range_image = image_padding_by_OpenCV(&padded_range_image, input_kernal_information.kernal_size, input_kernal_information.anchor);
	padded_spatial_image = image_padding_by_OpenCV(&padded_spatial_image, input_kernal_information.kernal_size, input_kernal_information.anchor);

	vector<Vec3f*> row_pointers_range;
	vector<Vec3f*> row_pointers_spatial;
	row_pointers_range.clear();
	row_pointers_spatial.clear();
	for (row_pointer = 0; row_pointer < input_kernal_information.kernal_size.height; row_pointer++) {
		row_pointers_range.push_back(padded_range_image.ptr<Vec3f>(row_pointer));
		row_pointers_spatial.push_back(padded_spatial_image.ptr<Vec3f>(row_pointer));
	}

	float temp_for_B , temp_for_G , temp_for_R;
	float temp_weight_B, temp_weight_G, temp_weight_R;
	float temp;

	for (row_count = 0; row_count < (*input_image).rows; row_count++) {
		for (column_count = 0; column_count < (*input_image).cols; column_count++) {
			temp_for_B = 0;
			temp_for_G = 0;
			temp_for_R = 0;

			temp_weight_B = 0;
			temp_weight_G = 0;
			temp_weight_R = 0;

			for (current_kernal_row = 0; current_kernal_row < input_kernal_information.kernal_size.height; current_kernal_row++) {
				for (current_kernal_column = 0; current_kernal_column < input_kernal_information.kernal_size.width; current_kernal_column++) {
					
				}
			}
		}

		
		row_pointers_range.erase(row_pointers_range.begin);
		row_pointers_spatial.erase(row_pointers_spatial.begin);
		row_pointers_range.push_back(padded_range_image.ptr<Vec3f>(row_pointer));
		row_pointers_spatial.push_back(padded_spatial_image.ptr<Vec3f>(row_pointer));
		row_pointer++;
		
	}


	return result;
}
*/

/*
Mat bilateral_filtering(Mat* padded_image, Mat* spatial, float sigma_for_range, filtering_information input_kernal_information) {
	Mat result = Mat(Size(input_kernal_information.kernal_size.width, input_kernal_information.kernal_size.height), CV_32F);

	float molecular;
	float denominator = 2 * PI * pow(sigma_for_range, 2);
	float power;

	Vec3f* pointer_for_padded_image;
	Vec3f* pointer_for_result;
	Vec3f* pointer_for_spatial;

	int current_kernal_row, current_kernal_column, current_kernal_channel;
	int padded_row, padded_column;
	int row_count, column_count;

	for (current_kernal_row = 0; current_kernal_row < input_kernal_information.kernal_size.height; current_kernal_row++) {
		for (current_kernal_column = 0; current_kernal_column < input_kernal_information.kernal_size.width; current_kernal_column++) {

		}
	}


	return result;
}
*/

/*
Mat get_range_mat(Mat* padded_image, float sigma_for_range, filtering_information input_kernal_information) {
	Mat result = Mat(Size(input_kernal_information.kernal_size.width, input_kernal_information.kernal_size.height), CV_32F);

	vector<Vec3f*>kernal_pointers;

	float molecular;
	float denominator = 2 * PI * pow(sigma_for_range, 2);
	float power;

	Vec3f* pointer_for_padded_image;
	Vec3f* pointer_for_result;
	Vec3f* pointer_for_spatial;

	int current_kernal_row, current_kernal_column, current_kernal_channel;
	int padded_row, padded_column;
	int row_count, column_count;

	for (current_kernal_row = 0; current_kernal_row < input_kernal_information.kernal_size.height; current_kernal_row++) {


		for (current_kernal_column = 0; current_kernal_column < input_kernal_information.kernal_size.width; current_kernal_column++) {

		}
	}


return result;
}
*/