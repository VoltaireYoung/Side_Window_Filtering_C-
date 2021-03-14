#include "used_headers.h"
#include "filter.h"

using namespace cv;
using namespace std;

Mat bilateral_with_SWF(Mat*, vector<filtering_information>, int, float, float);
Mat bilateral_filtering(Mat*, float, float, filtering_information);
Mat get_gaussian_kernal_spatial(filtering_information, float);
Mat get_gaussian_kernal_range(Mat*, filtering_information, float);