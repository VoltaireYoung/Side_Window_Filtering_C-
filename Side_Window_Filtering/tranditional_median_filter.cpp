#include "used_headers.h"
#include "basic_image_processing.h"

//Structure
#include "filter.h"

//Vec3b get_median(Mat*, Point, Size*, Point*);

/*
Mat my_medianFilter(Mat* input_image, int depth, Size kernal_size, Point anchor , int border_type) {
	Mat result = Mat::zeros((*input_image).rows, (*input_image).cols, CV_8UC3);

	Mat padded_image = image_padding_by_OpenCV(input_image, kernal_size , anchor);

	int current_row, current_column;
	int row_count, column_count;

	for (current_row = anchor.y, row_count = 0; row_count < (*input_image).rows; current_row++, row_count++) {
		for (current_column = anchor.x, column_count = 0; column_count < (*input_image).cols; current_column++, column_count++) {
			//printf("%d - %d\n", current_row, current_column);
			result.at<Vec3b>(row_count, column_count) = get_median(&padded_image, Point(current_column, current_row), &kernal_size, &anchor);
		}
	}
	
	return result;
}


Vec3b get_median(Mat* input_image ,Point coordination , Size* kernal_size , Point* anchor) {
	Vec3b result;
	Vec3b current_pixel;

	int current_row, current_column, current_channel;
	int row_count, column_count;

	int total_neighbors = (*kernal_size).width * (*kernal_size).height;

	int index = 0;

	//int* neighbors = (int*)malloc(sizeof(int) * ((*kernal_size).width * (*kernal_size).height));
	int** neighbors = (int**)malloc(sizeof(int*) * 3);
	for (current_channel = 0; current_channel < 3; current_channel++) {
		neighbors[current_channel] = (int*)malloc(sizeof(int) * total_neighbors);
	}

	for (current_row = coordination.y - (*anchor).y, row_count = 0; row_count < (*kernal_size).height; current_row++, row_count++) {
		for (current_column = coordination.x - (*anchor).x, column_count = 0; column_count < (*kernal_size).width; column_count++, current_column++ , index++) {
			current_pixel = (*input_image).at<Vec3b>(current_row, current_column);
			neighbors[0][index] = current_pixel[0];
			neighbors[1][index] = current_pixel[1];
			neighbors[2][index] = current_pixel[2];
		}
	}

	for (current_channel = 0; current_channel < 3; current_channel++) {
		sort(neighbors[current_channel], neighbors[current_channel] + total_neighbors);
	}

	result[0] = neighbors[0][(int)(total_neighbors / 2)];
	result[1] = neighbors[1][(int)(total_neighbors / 2)];
	result[2] = neighbors[2][(int)(total_neighbors / 2)];

	return result;
}
*/

Vec3b get_median(Mat*, Point, Size*, Point* , int**);

Mat my_medianFilter(Mat* input_image, int depth, Size kernal_size, Point anchor, int border_type) {
	Mat result = Mat::zeros((*input_image).rows, (*input_image).cols, CV_8UC3);

	Mat padded_image = image_padding_by_OpenCV(input_image, kernal_size, anchor);

	int current_row, current_column;
	int row_count, column_count;

	int** neighbors = (int**)malloc(sizeof(int*) * 3);
	for (int current_channel = 0; current_channel < 3; current_channel++) {
		neighbors[current_channel] = (int*)malloc(sizeof(int) * (kernal_size.width * kernal_size.height));
	}

	for (current_row = anchor.y, row_count = 0; row_count < (*input_image).rows; current_row++, row_count++) {
		for (current_column = anchor.x, column_count = 0; column_count < (*input_image).cols; current_column++, column_count++) {
			//printf("%d - %d\n", current_row, current_column);
			result.at<Vec3b>(row_count, column_count) = get_median(&padded_image, Point(current_column, current_row), &kernal_size, &anchor , neighbors);
		}
	}

	return result;
}

Vec3b get_median(Mat* input_image, Point coordination, Size* kernal_size, Point* anchor , int** neighbors) {
	Vec3b result;
	Vec3b current_pixel;

	int current_row, current_column, current_channel;
	int row_count, column_count;

	int total_neighbors = (*kernal_size).width * (*kernal_size).height;

	int index = 0;

	//int* neighbors = (int*)malloc(sizeof(int) * ((*kernal_size).width * (*kernal_size).height));
	/*
	int** neighbors = (int**)malloc(sizeof(int*) * 3);
	for (current_channel = 0; current_channel < 3; current_channel++) {
		neighbors[current_channel] = (int*)malloc(sizeof(int) * total_neighbors);
	}
	*/

	Vec3b row_pointer;

	for (current_row = coordination.y - (*anchor).y, row_count = 0; row_count < (*kernal_size).height; current_row++, row_count++) {
		for (current_column = coordination.x - (*anchor).x, column_count = 0; column_count < (*kernal_size).width; column_count++, current_column++, index++) {
			current_pixel = (*input_image).at<Vec3b>(current_row, current_column);
			neighbors[0][index] = current_pixel[0];
			neighbors[1][index] = current_pixel[1];
			neighbors[2][index] = current_pixel[2];
		}
	}

	for (current_channel = 0; current_channel < 3; current_channel++) {
		sort(neighbors[current_channel], neighbors[current_channel] + total_neighbors);
	}

	/*
	result[0] = neighbors[0][(int)(total_neighbors / 2)];
	result[1] = neighbors[1][(int)(total_neighbors / 2)];
	result[2] = neighbors[2][(int)(total_neighbors / 2)];
	*/

	int t = (*kernal_size).width * (*kernal_size).height;
	

	return result;
}