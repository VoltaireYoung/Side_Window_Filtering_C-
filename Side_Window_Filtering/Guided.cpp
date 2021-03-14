#include "used_headers.h"
#include "basic_image_processing.h"

//Structure
#include "filter.h"

using namespace cv;
using namespace std;

Mat guided_filtering(Mat*, Mat*, filtering_information, float);
Mat Not_MY(Mat, Mat, filtering_information, double);

Mat guided_with_SWF(Mat* input_image, Mat* guidance_image, vector<filtering_information> informations, int number_of_filters, float eps) {
	Mat result;

	int current_filter_index;

	vector<Mat> candidates;

	Size current_keranl_size;
	Point current_kernal_anchor;

	Mat current_candidate;

	vector<Mat>for_split_input_image;
	vector<Mat>for_split_guidance_image;
	vector<Mat> for_merge;

	split(*input_image, for_split_input_image);
	split(*guidance_image, for_split_guidance_image);

	int current_channel;
	Mat current_channel_result;

	Mat for_show;
	Mat temp;

	for (current_filter_index = 0; current_filter_index < number_of_filters; current_filter_index++) {

		//Mat new_Mat = Mat::zeros((*input_image).rows, (*input_image).cols, CV_32F);
		
		for (current_channel = 0; current_channel < 3; current_channel++) {
			current_channel_result = Not_MY(for_split_input_image.at(current_channel), for_split_guidance_image.at(current_channel), informations[current_filter_index], (double)eps);
			for_merge.push_back(current_channel_result);
		}

		merge(for_merge, current_candidate);


		//current_candidate = guided_filtering(input_image, guidance_image, informations[current_filter_index], eps);

		current_candidate.convertTo(current_candidate, CV_32F);
		candidates.push_back(current_candidate);
		
		for_merge.clear();
	}

	if (number_of_filters == 1) {
		return current_candidate;
	}

	result = get_best(input_image, candidates, number_of_filters);

	return result;
}

Mat guided_filtering(Mat* input_image , Mat* guidance_image , filtering_information current_information , float eps) {
	//轉換成32 bits 浮點數
	Mat current_input_image;
	if ((*input_image).depth() != 5) {
		(*input_image).convertTo(current_input_image, CV_32F);
	}
	else {
		(*input_image).copyTo(current_input_image);
	}

	Mat current_guidance_image;
	if ((*guidance_image).depth() != 5) {
		(*guidance_image).convertTo(current_guidance_image, CV_32F);
	}
	else {
		(*guidance_image).copyTo(current_guidance_image);
	}

	//P : inputimage ; I : guided image
	Mat mean_I, mean_P;
	Mat correlation_II, correlation_IP;
	Mat variance_I, covariance_IP;
	Mat a, b;
	Mat mean_a, mean_b;

	Mat result;

	// mean_I guidance image
	boxFilter(current_guidance_image, mean_I,
		      current_information.depth,
		      current_information.kernal_size,
		      current_information.anchor ,
		      true ,
		      current_information.border_type);

	// mean_P input image
	boxFilter(current_input_image, mean_P,
			  current_information.depth,
			  current_information.kernal_size,
			  current_information.anchor,
			  true,
			  current_information.border_type);

	//correlation input image * input image
	boxFilter(current_guidance_image.mul(current_guidance_image), correlation_II,
			  current_information.depth,
			  current_information.kernal_size,
			  current_information.anchor,
			  true,
			  current_information.border_type);

	//correlation guided image * input image
	boxFilter(current_guidance_image.mul(current_input_image), correlation_IP,
			  current_information.depth,
			  current_information.kernal_size,
			  current_information.anchor,
			  true,
			  current_information.border_type);

	//variance I
	variance_I = correlation_II - mean_I.mul(mean_I);

	//covariance IP
	covariance_IP = correlation_IP - mean_I.mul(mean_P);

	//a and b
	a = covariance_IP / (variance_I + Scalar(eps , eps , eps));
	b = mean_P - a.mul(mean_I);

	/*
	int r, c, cc;
	Vec3f t , tt;
	for (r = 400; r < 512; r++) {
		for (c = 400; c < 512; c++) {
			t = a.at<Vec3f>(r, c);
			printf("R - C : %d - %d\n", r, c);
			for (cc = 0; cc < 3; cc++) {
				printf("%f - %f - %f\n", t[0], t[1], t[2]);
			}
		}
	}
	*/

	//mean_a and mean_b
	boxFilter(a, mean_a,
			  current_information.depth,
			  current_information.kernal_size,
			  current_information.anchor,
			  true,
			  current_information.border_type);

	boxFilter(b, mean_b,
			  current_information.depth,
			  current_information.kernal_size,
			  current_information.anchor,
			  true,
			  current_information.border_type);

	// result
	result = mean_a.mul(current_guidance_image) + mean_b;

	/*
	int r, c, cc;
	for (r = 400; r < 512; r++) {
		for (c = 400; c < 512; c++) {
			printf("R - C : %d - %d\n", r, c);
			for (cc = 0; cc < 3; cc++) {
				//printf("%d - %f\n", (*input_image).at<Vec3b>(r, c)[cc] , result.at<Vec3f>(r, c)[cc]);
				printf("%f - %d\n", result.at<Vec3f>(r, c)[cc], (*input_image).at<Vec3b>(r, c)[cc]);

			}
		}
	}
	*/
	

	return result;
}

Mat Not_MY(Mat I, Mat p, filtering_information current_information, double eps)
{
	Mat _I;
	I.convertTo(_I, CV_64FC1, 1.0 / 255);
	I = _I;

	Mat _p;
	p.convertTo(_p, CV_64FC1, 1.0 / 255);
	p = _p;

	//[hei, wid] = size(I);  
	int hei = I.rows;
	int wid = I.cols;

	Mat mean_I;
	boxFilter(I, mean_I, CV_64FC1, current_information.kernal_size, current_information.anchor, current_information.border_type);

	//mean_p = boxfilter(p, r) ./ N;  
	Mat mean_p;
	boxFilter(p, mean_p, CV_64FC1, current_information.kernal_size , current_information.anchor , current_information.border_type);

	//mean_Ip = boxfilter(I.*p, r) ./ N;  
	Mat mean_Ip;
	boxFilter(I.mul(p), mean_Ip, CV_64FC1, current_information.kernal_size, current_information.anchor, current_information.border_type);

	//cov_Ip = mean_Ip - mean_I .* mean_p; % this is the covariance of (I, p) in each local patch.  
	Mat cov_Ip = mean_Ip - mean_I.mul(mean_p);

	//mean_II = boxfilter(I.*I, r) ./ N;  
	Mat mean_II;
	boxFilter(I.mul(I), mean_II, CV_64FC1, current_information.kernal_size, current_information.anchor, current_information.border_type);

	//var_I = mean_II - mean_I .* mean_I;  
	Mat var_I = mean_II - mean_I.mul(mean_I);

	//a = cov_Ip ./ (var_I + eps); % Eqn. (5) in the paper;     
	Mat a = cov_Ip / (var_I + eps);

	//b = mean_p - a .* mean_I; % Eqn. (6) in the paper;  
	Mat b = mean_p - a.mul(mean_I);

	//mean_a = boxfilter(a, r) ./ N;  
	Mat mean_a;
	boxFilter(a, mean_a, CV_64FC1, current_information.kernal_size, current_information.anchor, current_information.border_type);

	//mean_b = boxfilter(b, r) ./ N;  
	Mat mean_b;
	boxFilter(b, mean_b, CV_64FC1, current_information.kernal_size, current_information.anchor, current_information.border_type);

	//q = mean_a .* I + mean_b; % Eqn. (8) in the paper;  
	Mat q = mean_a.mul(I) + mean_b;

	return (q * 255);
}