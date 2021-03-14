#include "used_headers.h"
#include "basic_image_processing.h"
#include "side_window_filtering.h"
#include "filter.h"

#include "tranditional_median_filter.h"

#include "csv_IO.h"


void data_generator(int, int, string, string);
vector<filtering_information> get_SWF_filter_information(int);

void test_box(Mat*);


using namespace std;

int main(int argc, char* agrv[]) {
	string test_image_path = "D:\\New_Result\\Data\\Clean\\Boss_Base";
	string save_path = "D:\\New_Result\\Smoothing\\4.Guided";
	
	data_generator(atoi(agrv[1]), atoi(agrv[2]), test_image_path, save_path);

	system("Pause");
	return 0;
}

void data_generator(int start , int end , string data_path , string save_path) {
	string current_image_path;
	string current_save_path;

	int current_image_index;

	Mat current_image;
	Mat current_result;
	Mat current_for_png;

	filtering_information gI = filtering_information();
	gI.border_type = BORDER_REPLICATE;
	gI.anchor = Point(3, 3);
	gI.depth = CV_32F;
	gI.kernal_size = Size(7, 7);
	vector<filtering_information> te;
	te.push_back(gI);

	vector<filtering_information> kernal_information = get_SWF_filter_information(7);
	
	int iteration = 1;
	int current_iteration;

	for (current_image_index = start; current_image_index <= end; current_image_index++) {
		printf("Current image : %d\n", current_image_index);
		current_image_path = data_path + "\\" + to_string(current_image_index) + ".png";

		current_image = imread(current_image_path.c_str());


		current_image.copyTo(current_result);
		
		for (current_iteration = 1; current_iteration <= iteration; current_iteration++) {
			//current_result = gaussian_with_SWF(&current_result, kernal_information, 8, (float)5);
			//current_result = box_with_SWF(&current_result, kernal_information, 8);
			//current_result = guided_with_SWF(&current_result, &current_result, kernal_information, 8, (float)0.1);
			current_result = bilateral_with_SWF(&current_image, kernal_information, 8, 7, 15);

			write_csv_with_float_Mat(save_path + "\\SWF\\csv", to_string(current_image_index), Size(512, 512), current_result);

			current_result.convertTo(current_for_png, current_image.depth());
			imwrite(save_path + "\\SWF\\png\\" + to_string(current_image_index) + ".png", current_for_png);
		}
		
		
		//string save_path_2 = "D:\\New_Result\\Denoise\\3.Bilateral\\SWF\\Sigma_10";
		current_image.copyTo(current_result);
		for (current_iteration = 1; current_iteration <= iteration; current_iteration++) {
			//current_result = gaussian_with_SWF(&current_result, te, 1, (float)5);
			//boxFilter(current_result, current_result, CV_32F, Size(10, 10), Point(-1, -1), true, BORDER_REPLICATE);
			//current_result = guided_with_SWF(&current_result, &current_result, te, 1, (float)0.1);
			current_result = bilateral_with_SWF(&current_image, te, 1, 7, 15);

			write_csv_with_float_Mat(save_path + "\\Normal\\csv", to_string(current_image_index), Size(512, 512), current_result);

			current_result.convertTo(current_for_png, current_image.depth());
			imwrite(save_path + "\\Normal\\png\\" + to_string(current_image_index) + ".png", current_for_png);
		}
	}
}

vector<filtering_information> get_SWF_filter_information(int radius) {
	vector<filtering_information> result;

	//Left
	filtering_information current_information = filtering_information();
	current_information.depth = CV_32F;
	current_information.kernal_size = Size(radius, radius * 2 + 1);
	current_information.anchor = Point(radius - 1, radius);
	current_information.border_type = BORDER_REPLICATE;
	result.push_back(current_information);

	//Right
	current_information = filtering_information();
	current_information.depth = CV_32F;
	current_information.kernal_size = Size(radius, radius * 2 + 1);
	current_information.anchor = Point(0, radius);
	current_information.border_type = BORDER_REPLICATE;
	result.push_back(current_information);

	//Top
	current_information = filtering_information();
	current_information.depth = CV_32F;
	current_information.kernal_size = Size(radius * 2 + 1, radius);
	current_information.anchor = Point(radius, radius - 1);
	current_information.border_type = BORDER_REPLICATE;
	result.push_back(current_information);

	//Buttom
	current_information = filtering_information();
	current_information.depth = CV_32F;
	current_information.kernal_size = Size(radius * 2 + 1, radius);
	current_information.anchor = Point(radius, 0);
	current_information.border_type = BORDER_REPLICATE;
	result.push_back(current_information);

	//Left top
	current_information = filtering_information();
	current_information.depth = CV_32F;
	current_information.kernal_size = Size(radius, radius);
	current_information.anchor = Point(radius - 1, radius - 1);
	current_information.border_type = BORDER_REPLICATE;
	result.push_back(current_information);

	//Right top
	current_information = filtering_information();
	current_information.depth = CV_32F;
	current_information.kernal_size = Size(radius, radius);
	current_information.anchor = Point(0, radius - 1);
	current_information.border_type = BORDER_REPLICATE;
	result.push_back(current_information);

	//Left Buttom
	current_information = filtering_information();
	current_information.depth = CV_32F;
	current_information.kernal_size = Size(radius, radius);
	current_information.anchor = Point(radius - 1, 0);
	current_information.border_type = BORDER_REPLICATE;
	result.push_back(current_information);

	//Right Buttom
	current_information = filtering_information();
	current_information.depth = CV_32F;
	current_information.kernal_size = Size(radius, radius);
	current_information.anchor = Point(0, 0);
	current_information.border_type = BORDER_REPLICATE;
	result.push_back(current_information);

	return result;
}

void test_box(Mat* input_image) {
	Mat current_image;
	(*input_image).convertTo(current_image, CV_32F);
	Mat result_image;
	
	Mat temp;
	(*input_image).convertTo(temp , CV_32F);

	Vec3f test;
	test = temp.at<Vec3f>(300, 300);
	printf("%f - %f - %f\n", test[0], test[1], test[2]);

	temp = temp + Scalar(10, 10, 10);
	test = temp.at<Vec3f>(300, 300);
	printf("%f - %f - %f\n", test[0], test[1], test[2]);

	temp = temp + temp;
	test = temp.at<Vec3f>(300, 300);
	printf("%f - %f - %f\n", test[0], test[1], test[2]);
}