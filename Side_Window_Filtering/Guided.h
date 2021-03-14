#include "used_headers.h"
#include "filter.h"

using namespace cv;
using namespace std;

Mat guided_with_SWF(Mat*, Mat*, vector<filtering_information>, int, float);
Mat guided_filtering(Mat*, Mat*, filtering_information, float);
Mat Not_MY(Mat, Mat, filtering_information, double);